/*
 * 2D Dynamically allocated array type.
 * Uses malloc for runtime reinitialization.
 * Provides 2D accessors for ease.
 * 
 * CS726 Project 1 2013
 * Ryan Kurte, rkur014
*/

#ifndef MATRIX_H_
#define MATRIX_H_

#include <iostream>
#include <exception>
#include <stdexcept>

class Matrix {
private:
    unsigned xSize, ySize;
    //Array pointer.
    int* array;

public:
    bool initialized;
    
    Matrix(unsigned x, unsigned y);		//Constructor.
    ~Matrix();							//Destructor
    
    //Accessors.
    int& operator() (unsigned x, unsigned y);
    int  operator() (unsigned x, unsigned y) const; 
    unsigned GetXSize();    //Number of columns.
    unsigned GetYSize();    //Number of rows.
    void Clear();           //Clear the matrix.

    //Fetch a sub matrix of the matrix.
    Matrix* GetSubMatrix(unsigned x, unsigned y, unsigned xCount, unsigned yCount);
    
};

#endif
