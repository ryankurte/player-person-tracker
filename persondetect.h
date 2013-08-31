/*
* Person detection class.
* Detects people in a vector of blobs.
* 
* CS726 Project 1
* Ryan Kurte, rkur014
*/

#ifndef PERSON_DETECT_H_
#define PERSON_DETECT_H_

#define DETECTION_X 	3		//Person Detection distance [from front]
#define DETECTION_Y 	3		//Person detection distance [side to side]

#define BOUND_X			1.5
#define BOUND_Y			2

#define MAX_LEG_DIST 	0.5		//Distance two blobs must be within to be detected as a person.

#include <vector>
#include <cmath>
#include <stdint.h>
#include <stdio.h>

#include "person.h"
#include "blob.h"

using namespace std;

//Class.
class PersonDetect {
private:
	double gridSize;
	int gridSquares;
	double scaleFactor;
	vector<person_t> people;
	
public:
	PersonDetect(double gridSizeIn, int gridSquaresIn);	//constructor.
	int DetectPeople(vector<blob_t> blobs);				//Detect people given a vector of blobs.
    vector<person_t> GetPeople();						//Fetch the vector of person data.
    int Count();										//Return the number of people located.
    void Clear();										//Clear array of people.
	person_t GetClosest();								//Returns the closest person.
	int CountWithinBound();								//Returns number of people within bounds.
	person_t GetClosestWithinBound();					//Returns the closest person within bounds.
	person_t GetClosestToCenter();						//Returns closest person to center.
};


#endif
