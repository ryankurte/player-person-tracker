/*
* Command and control class.
* Integrates all other classes.
* Controls the robot, implements algorithms etc.
* 
* CS726 Project 1
* Ryan Kurte, rkur014
*/

#ifndef COLLISION_H_
#define COLLISION_H_

#define _USE_MATH_DEFINES		//Enables use of M_PI
#define NUM_SQUARES 150      	//Number of squares in grid.
#define GRID_SIZE 10         	//Size of grid squares in meters.
#define SQUARE_SIZE GRID_SIZE / NUM_SQUARES

//Robot operation defines.
#define MAX_LASER_RANGE 3.5		//Maximum range for lazer data to be considered relevant.
#define HYSTERESIS 4			//Cycles to continue move when person is lost.
#define TURN_HYSTERESIS 10      //Cycles to continue turning when lost.
#define VFH_THRESHOLD 0.3		//Closest distance robot will ever move to.
#define TURN_THRESHOLD 0.5		//Distance at which robot will only turn.

#include <libplayerc++/playerc++.h>
#include <stdint.h>
#include <math.h>

#include "blobdetect.h"
#include "persondetect.h"
#include "matrix.h"
#include "position.h"

#ifndef NO_RENDERER
#include "gridrenderer.h"
#endif

using namespace PlayerCc;

enum RobotStateEnum { IDLE, FOLLOWING, APPROACHING, AVOIDING, LOST_LEFT, LOST_RIGHT};

class Collision {
private:
    
    Matrix* map;	//Custom Environment and collision map. Persistant.
    Matrix* view;	//Temporary map, cleared each cycle.
    
    player_pose2d_t pose;		//Pose estimate.
    player_pose2d_t prevPose;	//Previous pose estimate.
    RobotStateEnum state;		//Robot following state.
    person_t prevPerson;		//Person previously being followed.
    
#ifndef NO_RENDERER
    GridRenderer* renderer;		//Grid Renderer.
#endif

    BlobDetect* blob;			//Blob detector.
    PersonDetect* person;		//Person detector.

    //Variables.
    double squareSize;
    double gridSize;
    double collideSize;
    unsigned numSquares;
    int watchCount;
    int followCount;

    //Pointers to interaction proxies.
    Position2dProxy*    vfhPosition;	//VFH position driver.
    LaserProxy*         laser;			//Laser Drived.
    SpeechProxy*        speech;			//Speech Driver.

	void UpdatePose();	//Update robot position.
	void Render();		//Render information.

	bool UpdateMap();	//Update view and map using laser data. Returns true on successful update.
	bool UpdatePath();	//Update path. Move to the nearest set of feet.
	
	void GoTo(double mag, double relAngle);	//Go to a point relative to the robot.
	void StopMove();							//Stop moving.
    
    void ComputeLocalPoint(double range, double bearing);   //Process a point to local map from the laser scanner.
    void ComputeWorldPoint(double range, double bearing);   //Process a point to world map from the laser scanner.

public:
    Collision(PlayerClient* robot);		//Constructor. Consumes reference to robot to provide proxies.
    
    int Start();	//Start the collision and person tracking class..	
    int Stop();		//Stop the collision and person tracking class.
    void Update();	//Update the collision and person tracking class.
};

#endif
