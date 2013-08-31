#include <libplayerc++/playerc++.h>
#include <signal.h>
#include <iostream>

#include "args.h"
#include "collision.h"
#include "matrix.h"

//Test leg finding function.
void Collision::ComputeLegs() {
	unsigned count = laser->GetCount();
	unsigned step = M_PI / count;
	//Go through laser data and build a gradient [derivative] function.
	double grad[count-1];
	for(unsigned i=0; i<count-1; i++) {
		grad[i] = laser->GetRange(i+1) - laser->GetRange(i);
	}
	//Go through laser data and build second derivative.
	double grad2[count-2];
	for(unsigned i=0; i<count-2; i++) {
		grad2[i] = grad[i+1] - grad[1];
	}
	//Legs are therefore +ve zero crossings.
	
	#define GRAD_WINDOW 10
	//Go through points, fitting lines.
	for(unsigned i=0; i<count-2-GRAD_WINDOW; i++) {
		double sumX  = 0;
		double sumY  = 0;
		double sumX2 = 0;
		double sumXY = 0;
		//Iterate through 10 points.
		for(int j=i; j<GRAD_WINDOW; j++) {
			sumX  += i;
			sumY  += grad2[i];
			sumX2 += std::pow(grad2[i], 2);
			sumXY += i + grad2[i];
		}
		//compute line.
		double xMean = SumX / Count
        double yMean = SumY / Count
        double slope = (SumXY - SumX * YMean) / (SumX2 - SumX * XMean)
        double yInt = YMean - Slope * XMean
	}
	
}

void DisplayMatrix(Matrix* m);

int main(int argc, char **argv) {
    
    Matrix* m = new Matrix(10, 10);
    BlobDetect b = BlobDetect();
    
    //populate.
    for(int y=0; y<m->GetYSize(); y++) {
		for(int x=0; x<m->GetXSize(); x++) {
			if((x>3)&&(x<6)) {
				if((y>2)&&(y<6)) {
					(*m)(x, y) = x + y*m->GetXSize();
				}
			}
		}
	}
	
	
	
	DisplayMatrix(m);
	int x = 2;
	int y = 2;
	
	b.LocateBlobs(m);

    return 0;
}


void DisplayMatrix(Matrix* m) {
	printf("Matrix: \r\n");
	for(int y=0; y<m->GetYSize(); y++) {
		for(int x=0; x<m->GetXSize(); x++) {
			printf("%3d,", (*m)(x,y));
		}
		printf("\r\n");
	}
	
}








