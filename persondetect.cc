//Person Detection Methods.

#include "persondetect.h"

//Constructor.
PersonDetect::PersonDetect(double gridSizeIn, int gridSquaresIn) {
	//Grid size required to determine robot relative positions.
	gridSize = gridSizeIn;
	gridSquares = gridSquaresIn;
	scaleFactor = gridSize/gridSquares;
}

//Detect a person in an array of blobs.
int PersonDetect::DetectPeople(vector<blob_t> blobs) {
	int personCount = 0;
	//Clear person vector.
	people.clear();
	
	int count = blobs.size();
	//Iterate through blobs.
	for(int i=0; i<count; i++) {
		//Check other blobs.
		for(int j=0; j<count; j++) {
			//If they are not the same blob.
			if(i != j) {
				//Calculate robot relative position.
				blob_t a;
				a.x = (blobs[i].x);
				a.y = (blobs[i].y);
				blob_t b;
				b.x = (blobs[j].x);
				b.y = (blobs[j].y);
				
				//Calculate the difference between them.
				double xDiff   = std::abs(a.x - b.x)*scaleFactor;
				double yDiff   = std::abs(a.y - b.y)*scaleFactor;
				double absDiff = std::sqrt(std::pow(xDiff, 2) + std::pow(yDiff, 2));
				
				//If that is within a defined threshold.
				if(absDiff < MAX_LEG_DIST) {
					//Create a person object.
					person_t p;
					//Average the location of the two points, and set person data.
					p.x 	= -((a.x + b.x)*scaleFactor/2 - gridSize/2);
					p.y 	= (a.y + b.y)*scaleFactor/2 - gridSize/2;
					p.angle = std::atan(p.x/p.y);
					p.mag 	= std::sqrt(std::pow(p.x, 2) + std::pow(p.y, 2));
					
					//Check they are within the detection window.
					if(((p.x>=-DETECTION_X/2)&&(p.x<=DETECTION_X/2))&&((p.y>=0)&&(p.y<=DETECTION_Y))) {
						//Add to person array.
						people.push_back(p);
						personCount++;
						//printf("Person at: %.2f %.2f %.2f\r\n", p.x, p.y, p.angle);
					}
				}
			}
		}
	}
	//Return number of located people.
	return personCount;
}

//Get vector of people.
vector<person_t> PersonDetect::GetPeople() {
	return people;
}

//Get number of people located.
int PersonDetect::Count() {
	return people.size();
}

//Clear the list of known people.
void PersonDetect::Clear() {
	people.clear();
}

//Fetch number of people within bounds.
int PersonDetect::CountWithinBound() {
	int count = 0;
	for(unsigned i=0; i<people.size(); i++) {
		//Check if person is within bound.
		if(((people[i].x>=-BOUND_X/2)&&(people[i].x<=BOUND_X/2))&&((people[i].y>=0)&&(people[i].y<=BOUND_Y))) {
			count ++;
		}
	}
	return count;
}

//Fetch the closest person within the bounds.
person_t PersonDetect::GetClosestWithinBound() {
	int minDist  = 999;		//Minimum distance found.
	int minIndex = 0;		//Location of Minimum.
	//Iterate through available.
	for(unsigned i=0; i<people.size(); i++) {
		//Check if person is within bound.
		if(((people[i].x>=-BOUND_X/2)&&(people[i].x<=BOUND_X/2))&&((people[i].y>=0)&&(people[i].y<=BOUND_Y))) {
			//Update index and minimum if found.
			//If this one is closer.
			if(people[i].mag < minDist) {
				//Copy index.
				minDist = people[i].mag;
				minIndex = i;
			}
		}
	}
	return people[minIndex];
}

//Fetch the closest to the center person.
//For use when following to stop following other people.
person_t PersonDetect::GetClosestToCenter() {
	int minAngle = 100;
	int index = 0;
	for(unsigned i=0; i<people.size(); i++) {
		//Update index and minimum if found.
		//If this one is closer.
		if(std::abs(people[i].angle) < minAngle) {
			//Copy index.
			minAngle = people[i].angle;
			index = i;
		}
	}
	return people[index];
	
}

//Return closest person.
//This will segfault if there are no people detected.
person_t PersonDetect::GetClosest() {
	unsigned i;				//Iterator.
	int minDist  = 999;		//Minimum distance found.
	int minIndex = 0;		//Location of Minimum.
	
	//If there is only one, return.
	if(people.size() == 1) {
		return people[0];
	//Otherwise, find the closest.
	} else {
		//Iterate through people.
		for(i=0; i<people.size(); i++) {
			//If this one is closer.
			if(people[i].mag < minDist) {
				//Copy index.
				minDist = people[i].mag;
				minIndex = i;
			}
		}
		return people[minIndex];
	}
}
