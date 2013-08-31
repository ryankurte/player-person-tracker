/*
* Blob detection algorithm.
* For use with matrix files as defined in matrix.h.
* 
* CS726 Project 1
* Ryan Kurte, rkur014
*/

#ifndef BLOB_DETECT_H_
#define BLOB_DETECT_H_

//Window blob detection constants.
#define WINDOW_SIZE 6           //Size of scanning window for detection.
#define MIN_BLOB_WEIGHT 2       //Smallest blob to be detected.

#include <cmath>
#include <cstdio>
#include <vector>

#include "matrix.h"
#include "blob.h"

class BlobDetect {
private:
    std::vector<blob_t> blobs;                          //Array for blob storage.
    int blobCount;
    int BlockLocateBlobs(Matrix* map);                  //Window based method for locating blobs.

          				
    int CheckWeight(Matrix* map, int x, int y);         //Fetch the weight of a group in the matrix.
    void BlobDetected(float x, float y, float weight);  //Handle a detected blob.

public:
    BlobDetect();   								//Constructor.
    int LocateBlobs(Matrix* map);                   //Locate blobs in a map.
    std::vector<blob_t> GetBlobs();                 //Return a list of blobs.
    void Clear();                                   //Clear all the blobs.
    int Count();									//Return the number of located blobs.
    
    int CountOccupied(Matrix* map); 				//Count occupied cells of a group in the matrix.
    int DetectBlob(Matrix* map, int x, int y);		//Detect a blob in a given quadrant.
};  

#endif
