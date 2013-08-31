/*
* Renderer class.
* Provides methods to render and display pertinent information to robot operation.
* Based on the Allegro5.0 library.
* 
* CS726 Project 1
* Ryan Kurte, rkur014
*/

#ifndef GRIDRENDERER_H
#define GRIDRENDERER_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
 
#include "allegro5/allegro.h"
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_primitives.h"

#include "matrix.h"
#include "blob.h"
#include "person.h"

#define DEFAULT_X 640
#define DEFAULT_Y 640
#define ROBOT_X 40
#define ROBOT_Y 60

#define BLOB_BOX 20

class GridRenderer {
private:
    
    ALLEGRO_DISPLAY *display;           //Window.
    int xRes, yRes;                     //Window Size.
    unsigned gridX, gridY;              //Grid size [number of squares].
    double xSize, ySize;				//Window size in M.
    double xSquareSize, ySquareSize;    //Size of squares.
    bool alive;

    void DrawTile(unsigned x, unsigned y, int state);   //Draw a tile given index and state information.
    void DrawRobot(double x, double y, double r);
    void DrawBlob(float x, float y);
	void DrawPerson(float x, float y);

public:
    GridRenderer(unsigned x, unsigned y);         //Constructor.   Set window size.         
    int Start(unsigned x, unsigned y, double xSizeIn, double ySizeIn);            //Start the GridRenderer window. Set grid size.

    void Clear();                                 //Clear the render window.
    void Update(Matrix* m);                       //Update the map rendering.
    void Update(std::vector<blob_t> blobs);    	  //Update blob ID rendering.     
    void Update(std::vector<person_t> people);	  //Update person rendering.         
    void Stop();                                  //Stop the GridRenderer window.

};

#endif
