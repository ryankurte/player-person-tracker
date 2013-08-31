//Blob detection functions.

#include "blobdetect.h"

#define CENTER_WEIGHT 12
#define RING_WEIGHT   2

//Constructor. Allocates and clears memory.
BlobDetect::BlobDetect() {
    //Clear memory.
    Clear();
}

//Locate blobs in a map.
//This iterates across the map, using a window to locate blobs.
//Blobs are weighted so center is more valuable than around the center.
int BlobDetect::LocateBlobs(Matrix* map){
    //Grab the window size.
    int xSize = map->GetXSize();
    int ySize = map->GetYSize();
    int count = 0;
    
	//Clear blob array.
	Clear();
    
    //Iterate.
    for(int y=0; y<(ySize-WINDOW_SIZE); y++) {
        for(int x=0; x<(xSize-WINDOW_SIZE); x++) {
            //Check window for blob.
            count += DetectBlob(map, x, y);
        }
    }
    return count;
}

//Detect blobs using a window at the given location.
//Returns blob weight.
int BlobDetect::DetectBlob(Matrix* map, int x, int y) {
	int found = 0;
	//Fetch the submatricies.
	Matrix* window = map->GetSubMatrix(x, y, 6, 6);
	Matrix* boundary = window->GetSubMatrix(1,1,4,4);
	Matrix* center 	 = boundary->GetSubMatrix(1,1,2,2);

	//Check the center of the matrix. 2x2. 4 cells.
	int centerCount = CountOccupied(center);
	//Check second ring. 4x4. 12 cells.
	int boundaryCount = CountOccupied(boundary) - centerCount;
	//Check the boundary of the matrix. 6x6. 20 cells
	int windowCount = CountOccupied(window) - centerCount - boundaryCount;

	//If the center has a weight.
	if(centerCount >= 2) {

		//Get position.
		double blobX = x + WINDOW_SIZE/2 + 0.5;
		double blobY = y + WINDOW_SIZE/2 + 0.5;

		//If there is nothing in the first ring
		if(boundaryCount == 0) {
			//Blob located.
			BlobDetected(blobX, blobY, centerCount*CENTER_WEIGHT);
			found = 1;
			//printf("Center blob detected x: %.2f y: %.2f.\r\n", blobX, blobY);
		//If there is nothing in the second ring.   
		} else if(windowCount == 0) {
			//And most of the center is filled, and most of the boundary is empty.
			if((centerCount >= 2)&&(boundaryCount <= 3)) {
				//Larger blob detected.
				BlobDetected(blobX, blobY, centerCount*CENTER_WEIGHT + boundaryCount*RING_WEIGHT);
				found =  1;
				//printf("Large blob detected x: %.2f y: %.2f.\r\n", blobX, blobY);
			}
		}
	}
	//Deallocate memory used by submatricies.
	delete window;
	delete boundary;
	delete center;
	//Return indicating whether blob was found.
	return found;
}

//Fetch the amount of occupied cells of an x by y submatrix in a matrix.
int BlobDetect::CountOccupied(Matrix* map) {
    int count = 0;

    int xSize = map->GetXSize();
    int ySize = map->GetYSize();

    //Iterate down matrix.
    for(int y=0; y<ySize; y++) {
        //Iterate across.
        for(int x=0; x<xSize; x++) {
            //If cell is occupied, increase count.
            if((*map)(x,y)) { count ++; }
        }
    }

    //Return count.
    return count;
}

//Handle a detected blob.
void BlobDetect::BlobDetected(float x, float y, float weight){
    bool blobFound = false;
    //printf("Blob detected at: %.2f %.2f weight: %.2f\r\n", x, y, weight);
    //Check for near blobs.
    //Iterate through blobs.
    for(unsigned i=0; i<blobs.size(); i++) {
        //If the blob is valid.
        if(blobs[i].valid) {
            //Check how close they are.
            float xDist = std::abs(blobs[i].x - x);
            float yDist = std::abs(blobs[i].y - y);

            //If they are within range of each other.
            if(((xDist<=1.5)&&(yDist<=0.5))||((yDist<=1.5)&&(xDist<=0.5))) {
			//if(false) {
                //Merge.
                blobFound = true;
                //Set heigher weight.
                if(weight > blobs[i].weight) { blobs[i].weight = weight; }
                //Merge locations.
                blobs[i].x = (blobs[i].x + x)/2;
                blobs[i].y = (blobs[i].y + y)/2;
				//printf("Merged blobs at: %.2f %.2f weight: %.2f\r\n", blobs[i].x , blobs[i].y, weight);
            }
        }   
    }

    //If not matched.
    if(!blobFound) {
        //Create object.
        blob_t b;
        b.x = x;
        b.y = y;
        b.weight = weight;
        b.valid = true;
        //Add to vector.
        blobs.push_back(b);
    }
}

//Fetch the weighting of an x by y submatrix in a matrix.
//Note that weighting is calculated in a circular manner, that the same blob
// if detected in two windows will have differing value.
int BlobDetect::CheckWeight(Matrix* map, int x, int y) {
    int count = 0;

    int xSize = map->GetXSize();
    int ySize = map->GetYSize();

    int xStart = xSize/2 - x/2;
    int yStart = ySize/2 - y/2;

    int xEnd = xSize/2 + x/2 - 1;
    int yEnd = ySize/2 + y/2 - 1;

    //Iterate down matrix.
    for(int y=yStart; y<yEnd; y++) {
        //Iterate across.
        for(int x=xStart; x<xEnd; x++) {
            //If cell is occupied, increase count.
            if((*map)(x,y)) { count ++; }
        }
    }

    //Return count.
    return count;
}

//Clear the blob vector.
void BlobDetect::Clear() {
    blobs.clear();
    blobCount = 0;
}

//Return the amount of blobs detected.
int BlobDetect::Count() {
    return blobs.size();
}

//Fetch blobs.
std::vector<blob_t> BlobDetect::GetBlobs() {
    return blobs;
}

