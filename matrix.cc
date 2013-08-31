#include "matrix.h"

//Matrix constructor.
Matrix::Matrix(unsigned x, unsigned y) {
    initialized = false;
    //Check sizes are valid.
    if (x == 0 || y == 0) {
        //Fail.
        throw std::out_of_range("Cannot create matrix with zero index.");
    } else {
        //Set sizes.
        xSize = x;
        ySize = y;
        //Allocate data.
        array = new int[x * y];
        //Clear.
        Clear();
        //Set initialized.
        initialized = true;
    }
}

//Matrix destructor.
Matrix::~Matrix()
{
  delete[] array;
}

void Matrix::Clear() {
    for(unsigned i=0; i<xSize*ySize; i++) {
        array[i] = 0;
    }
}

int& Matrix::operator() (unsigned x, unsigned y) {
    //Check indes is in bounds.
    if (x >= xSize || y >= ySize) {
        //Error, out of bounds.
        throw std::out_of_range("Index out of bounds");
    } else {
        //Otherwise, return data.
        return array[y * xSize + x];
    }
}

int Matrix::operator() (unsigned x, unsigned y) const {
    //Check indes is in bounds.
    if (x >= xSize || y >= ySize) {
        //Error, out of bounds.
        throw std::out_of_range("Index out of bounds");
    } else {
        //Otherwise, return data.
        return array[y * xSize + x];
    }
}

unsigned Matrix::GetYSize() {
    return ySize;
}

unsigned Matrix::GetXSize()  {
    return xSize;
}

//Return a sub matrix.
Matrix* Matrix::GetSubMatrix(unsigned xStart, unsigned yStart, unsigned xCount, unsigned yCount) {
    //Create Matrix.
    Matrix* subMatrix;
    //Check bounds.
    if((xStart+xCount <= xSize)&&(yStart+yCount <= ySize)) {
        subMatrix = new Matrix(xCount, yCount);
        //Iterate through.
        for(unsigned y=0; y<yCount; y++) {
            for(unsigned x=0; x<xCount; x++) {
                //Set value.
                (*subMatrix)(x, y) = (*this)(x+xStart, y+yStart);
            }   
        }
    } else {
		throw std::out_of_range("Index out of bounds");
	}
    //Return the map.
    return subMatrix;
}


