/*
* Command and control class.
* Integrates all other classes.
* Controls the robot, implements algorithms etc.
* 
* CS726 Project 1
* Ryan Kurte, rkur014
*/

#include "collision.h"

//Constructor.
//Attached proxies for device access.
Collision::Collision(PlayerClient* robot) {
    //Create and attach device proxies.
    //Attach position proxies.
    vfhPosition = new Position2dProxy(robot, 1);        //VFH position controller.
    //Attach laser proxy.
    laser = new LaserProxy(robot, 0);                  //LODO laser.
    //Attach speech proxy.

    //Set grid variables.
    squareSize = (double)GRID_SIZE / NUM_SQUARES;
    gridSize = (double)GRID_SIZE;
    numSquares = (double)NUM_SQUARES;
    collideSize = squareSize / 2;
    watchCount = 0;

    //Create matrix for occupancy map.
    map  = new Matrix(NUM_SQUARES, NUM_SQUARES);
    view = new Matrix(NUM_SQUARES, NUM_SQUARES);
#ifndef NO_RENDERER
    //Create renderer.
    renderer = new GridRenderer(600, 600);
#endif
    //Create blob detector.
    blob = new BlobDetect();
    
    //Create person detector.
    person = new PersonDetect(gridSize, numSquares);

    //Display.
    std::cout << "Grid size is: " << gridSize << " m" << std::endl;
    std::cout << "Square size is: " << squareSize << " m" << std::endl;
    
    //Reset bot position to zero.
    vfhPosition->ResetOdometry();
    pose.px = 0;
    pose.py = 0;
    pose.pa = 0;
}

//Start the collision avoidance and navigation routines.
int Collision::Start() {
#ifndef NO_RENDERER
    //Start render window.
    renderer->Start(map->GetXSize(), map->GetYSize(), gridSize, gridSize);
#endif
    //Enable motors.
    vfhPosition->SetMotorEnable (true);
    //Sleep to allow system time to wake.
    sleep(2.0);
	state = IDLE;
	followCount = 0;
	//Reset odometry.
	vfhPosition->SetOdometry(0.0, 0.0, 0.0);
    //speech->Say("Ready for Duty.");

    return 0;
}

//Stop.
int Collision::Stop() {
    vfhPosition->SetMotorEnable (false);
    //speech->Say("Shutting down.");
    sleep(2.0); 
    return 0;
}

void Collision::Update() {
    //Update Robot.
    UpdatePose();
    
    //Update occupancy grid.
    //If the map has updated.
    if(UpdateMap()) {
		//Attempt blob detection on new data.
		blob->LocateBlobs(view);
		//Attempt person detection.
		person->DetectPeople(blob->GetBlobs());
		//Update person tracking & robot path.
		UpdatePath();
	}
    //Render Output.
    Render();
}

//Render data.
void Collision::Render() {
#ifndef NO_RENDERER
    //Clear screen.
    renderer->Clear();
	//Render blobs.
    renderer->Update(blob->GetBlobs());
    renderer->Update(person->GetPeople());
    //Render map.
    renderer->Update(view);
#endif
}

//Update bot pose.
void Collision::UpdatePose() {
    //Update robot position from VFH driver.
    pose.px = vfhPosition->GetXPos();
    pose.py = vfhPosition->GetYPos();
    pose.pa = vfhPosition->GetYaw();
}

//Update the path of the robot.
//Uses location of people to plan and move robot.
bool Collision::UpdatePath() {
	person_t p;
	//If a person is seen.
	if(person->Count()) {
		//Get person object.
		//Check within narrow window for person.
		//This way, someone straight ahead will have priority.
		if(followCount > 3) {
			//If there is, grab this person.
			p = person->GetClosestToCenter();
		//Otherwise,
		} else {
			//Get the nearest person.
			p = person->GetClosest();
		}
		
		//In the FOLLOWING state.
		if(state == FOLLOWING) {
			//Update GOTO.
			GoTo(p.mag, p.angle);
			followCount ++;
		
		//In the other states.	
		} else {
			printf("Target Acquired\r\n");
			//Update GOTO.
			GoTo(p.mag, p.angle);
			//Set the state to following.
			state = FOLLOWING;
			//speech->Say("Following");
		}
			
		//If person located and path set, reset watch value.
		prevPerson = p;
		watchCount = 0;
		return true;
		
	} else {
		//If no person was located.
		//Increment watch counter.
		watchCount ++;
		//Reset following count.
		followCount = 0;
		
		//If the person has just been lost.
		if((state == FOLLOWING)&&(watchCount == 2)) {
			//See if they were at the edges of the FOV.
			if((prevPerson.y<0.4)) {
				//And on the left.
				if(((prevPerson.x > 0.2)&&(prevPerson.x < 1.0))&&(prevPerson.y < 1)) {
					//Person was lost on the left.
					printf("Lost Right\r\n");
					//Turn left a bit to try and catch lost person.
					GoTo(0, M_PI/2);
					state = LOST_LEFT;
				//If they were on the right.
				} else if(((prevPerson.x < -0.2)&&(prevPerson.x < 1.0))&&(prevPerson.y > -1)) {
					//Person was lost on the right.
					printf("Lost Left\r\n");
					//Turn left a bit to try and catch lost person.
					GoTo(0, -M_PI/2);
					state = LOST_RIGHT;
				}
			}
		//If they have been lost to the side.
		} else if((state == LOST_LEFT)||(state == LOST_RIGHT)) {
			//Keep on trucking until a hysteresis value is reached.
			//printf("Cycle: %d\r\n", watchCount);
			if(watchCount > TURN_HYSTERESIS) {
				//Then, given a person has not been recaptured, stop.
				StopMove();
				state = IDLE;
			//Otherwise, keep turning.
			} else if(state == LOST_LEFT) {
				GoTo(0, M_PI/2);
			} else if(state == LOST_RIGHT) {
				GoTo(0, -M_PI/2);
			}
		//If the person has been lost for a number of cycles.
		//Stop moving and reset.
		}else if((state == FOLLOWING)&&(watchCount > HYSTERESIS)) {
			printf("Target Lost\r\n");
			//Stop moving.
			StopMove();
			//Set IDLE mode.
			state = IDLE;
			//speech->Say("IDLE");
		}
		return false;
	}
}

//Update the map from the current position.
bool Collision::UpdateMap() {
    //Chect data from laser is new.
    if(laser->IsFresh()) {
        unsigned i;
		double range, bearing;
		//Clear the current view.
		view->Clear();
		
        //Iterate through data from rangefinder and build map.
        unsigned count = laser->GetCount();
        //For each entry.
        double iterator = -M_PI/2;
        for(i = 0; i<count; i++) {
            //Update map and view.
            
            //Fetch the point.
            range = laser->GetRange(i);
            bearing = laser->GetBearing(i);

            //Update the world point.
            ComputeWorldPoint(range, bearing);
            
            //Update local map.
            bearing = iterator;
            iterator += M_PI/count;
            ComputeLocalPoint(range, bearing);
        }
        return true;
    } else {
		return false;
	}
}

//Stop moving.
void Collision::StopMove() {
	//Go to current position.
	//vfhPosition->GoTo(0, 0, 0);
	//Set speed to zero.
	vfhPosition->SetSpeed(0, 0);
	
}

//Goto a robot relative position.
//The robot will stop VFH_THRESHOLD away from the target.
//Within this threshold, the robot will rotate on the spot rather than moving.
void Collision::GoTo(double mag, double relAngle) {
	//Set the tragectory to follow the person.
	//Normalize to never get closer than VFH allows.
	
	//If the distance is within the turn threshold, allow robot to turn only.
	if(mag < TURN_THRESHOLD) {
		//This forces the robot to rotate on the spot when the target is closer than TURN_THRESHOLD
		mag = 0;
	//Otherwise, decrease the magnitude by VFH_THRESHOLD so the robot doesn't ram people.
	//Or run out of space and make VFH freak out.
	} else {
			mag = mag - VFH_THRESHOLD;
	}
	//Ensure this never goes negative.
	if(mag < 0) { mag = 0; }
	
	//Calculate world position.
	double angle = pose.pa - relAngle;
	double newX = pose.px - mag*sin(angle - M_PI/2);
	double newY = pose.py + mag*cos(angle - M_PI/2);
	//Debug print statement.
#ifdef DEBUG		
	printf("Current %.2f %.2f %.2f Going to: %.2f, %.2f, %.2f\r\n", pose.px, pose.py, pose.pa , newX, newY, angle);
#endif
	//Goto position.
	vfhPosition->GoTo(newX, newY, angle);
}

void Collision::ComputeLocalPoint(double range, double bearing) {
    //Check the data is valid, ie. within bounds.
    if(range <= MAX_LASER_RANGE) {
        
        //World space for persistant map.
        //Note that laser data is returned with world bearings.
        double x = range*sin(bearing);
        double y = range*cos(bearing);
 
        //Convert to map space.
        unsigned xRef = x/squareSize + numSquares/2;
        unsigned yRef = y/squareSize + numSquares/2;

        //Check data is within map bounds.
        if(((xRef >= 0)&&(xRef < numSquares))&&((yRef >= 0)&&(yRef < numSquares))) {
            //Update Current view.
            (*view)(xRef, yRef) = 255;
        }
    }
}

//Compute point on persistant world map.
void Collision::ComputeWorldPoint(double range, double bearing) {
    //Check the data is valid, ie. within bounds.
    if(range <= MAX_LASER_RANGE) {
        
        //World space for persistant map.
        //Note that laser data is returned with world bearings.
        double x = pose.px + range*sin(pose.pa + bearing);
        double y = pose.py + range*cos(pose.pa + bearing);
 
        //Convert to map space.
        unsigned xRef = x/squareSize + numSquares/2;
        unsigned yRef = y/squareSize + numSquares/2;
        //Check data is within map bounds.
        if(((xRef >= 0)&&(xRef < numSquares))&&((yRef >= 0)&&(yRef < numSquares))) {
            int val = (*map)(xRef, yRef);
            //Occupancy / persistant map.
            //Check the cell is unknown (0) or statically occupied (positive).
            if((val >= 0) && (val < 252)) {
                //If it is, update the occupancy map with the occupied cell.
                //Presistent map.
                (*map)(xRef, yRef) += 4;
            }
        }
    }
}
