/*
    Grid rendering functions.
    Requires access to liballegro things.
*/

#include "gridrenderer.h"

//Constructor.
GridRenderer::GridRenderer(unsigned x, unsigned y) { 
    //Set render sizes.
    xRes = x;
    yRes = y;
   
}

//Start the GridRenderer.
//Set window size.
int GridRenderer::Start(unsigned x, unsigned y, double xSizeIn, double ySizeIn) {

    //Get matrix dimensions.
    gridX = x;
    gridY = y;  
    //Get screen dimensions.
    xSize = xSizeIn;
    ySize = ySizeIn;  

    //Calculate grid square size.
    //  size    =       res    /    grid size
    xSquareSize = (float)xRes / gridX;
    ySquareSize = (float)yRes / gridY;

    //Check allegro loaded.   
    if (!al_init()) {
        //std::cout << "Allegro: failed to init" << std::endl;
        return 1;
    }
    // Create the window.   
    display = al_create_display(xRes, yRes);

    al_init_primitives_addon();
	
    alive = true;

    //Return.
    return 0;
}

void GridRenderer::Clear() {
    //Clear display.
    al_clear_to_color(al_map_rgb(127, 127, 127));
}

//Update the render window.
void GridRenderer::Update(Matrix* m) {
    //Iterate through matrix and display squares.
    for(unsigned y=0; y<gridY; y++) {
        for(unsigned x=0; x<gridX; x++) {
            //Fetch value.
            int val = (*m)(x, y);
            if(val) {
                //Display.
                //nb, drawing is left to right, then downw.
                //Matrix is left to right, then up.
                DrawTile(gridX-1-x, gridY-1-y, val); 
            }
        }
    }
    //Flip buffer to display output.
    al_flip_display();
}

//Update blob rendering.
void GridRenderer::Update(std::vector<blob_t> blobs) {
    int numBlobs = blobs.size();
    //Iterate through blobs.
    for(int i=0; i<numBlobs; i++) {
        //Double check blob is valid.
        if(blobs[i].valid) {           
            DrawBlob(gridX-blobs[i].x, gridY-blobs[i].y);
        }
    }
}

//Update person rendering.
void GridRenderer::Update(std::vector<person_t> people) {
    int numPeople = people.size();
    //Iterate through people.
    for(int i=0; i<numPeople; i++) {
        DrawPerson(people[i].x, people[i].y);
    }
}

void GridRenderer::DrawPerson(float x, float y) {
	//Draw.
	//Set colour.
	ALLEGRO_COLOR col = al_map_rgb(0, 255, 0);
	float size = BLOB_BOX;
	
	//Convert to render space.
	x = ((x * xRes/xSize + xRes/2));
	y = (yRes - (y * yRes/ySize + yRes/2));

	//Render a box around the blob.
	//Draw tile.
	al_draw_rectangle(x-size, y-size, x+size, y+size, col, 1.0);
}

void GridRenderer::DrawBlob(float x, float y) {
	//printf("rendering blob at: %.2f, %.2f\r\n", x, y);
    //Set blob colour.
    ALLEGRO_COLOR col = al_map_rgb(255, 0, 0);
    float size = BLOB_BOX/2;
    //Convert to render space.
    x = x * xSquareSize + xSquareSize/2;
    y = y * ySquareSize + ySquareSize/2;

    //Render a box around the blob.
    //Draw tile.
    al_draw_rectangle(x-size, y-size, x+size, y+size, col, 1.0);
}

void GridRenderer::Stop() {

}

//Draw a tile at the provided grid location.
void GridRenderer::DrawTile(unsigned xRef, unsigned yRef, int state) {
    //Set square colour.
    ALLEGRO_COLOR col;
    //Occupied. Blue.
    if(state > 0) {
        col = al_map_rgba(0, 0, 255, state);
    //Empty space. White.
    } else if(state == -1) {
        col = al_map_rgb(255, 255, 255);
    //Temporal readings. Green.
    } else if(state == -2) {
        col = al_map_rgb(0, 255, 0);
    }

    //Convert to grid space.
    float x = xRef * xSquareSize;
    float y = yRef * ySquareSize;

    //Draw tile.
    al_draw_filled_rectangle(x, y, x + xSquareSize, y + ySquareSize, col);

}

//Render a square representing the robot.
void GridRenderer::DrawRobot(double x, double y, double r) {
    //Set robot colour.
    ALLEGRO_COLOR col = al_map_rgb(255, 0, 0);
    //Draw tile.
    al_draw_filled_rectangle(x, y, x + ROBOT_X, y + ROBOT_Y, col);
}




