#include "SphericalInterpolator.h"

SphericalInterpolator::SphericalInterpolator()
{

}


SphericalInterpolator::~SphericalInterpolator()
{

}

//is static
//the vectors must be in the order of xyz 000,100,010,001,101,011,110,111
float* SphericalInterpolator::Interpolate(float vectorInput[8][3], float relativePos[3])
{
	float* vectorFinal = new float[3];
	int i;
	for (i = 0; i < 3; i++) {
		vectorFinal[i] =	vectorInput[0][i] *  (1 - relativePos[0]) * (1 - relativePos[1]) * (1 - relativePos[2]) +
							vectorInput[1][i] * (relativePos[0]) * (1 - relativePos[1]) * (1 - relativePos[2]) +
							vectorInput[2][i] * (1 - relativePos[0]) * relativePos[1] * (1 - relativePos[2]) +
							vectorInput[3][i] * (1 - relativePos[0]) * (1 - relativePos[1]) * relativePos[2] +
							vectorInput[4][i] * relativePos[0] * (1 - relativePos[1]) * relativePos[2] +
							vectorInput[5][i] * (1 - relativePos[0]) * relativePos[1] * relativePos[2] +
							vectorInput[6][i] * relativePos[0] * relativePos[1] * (1 - relativePos[2]) +
							vectorInput[7][i] * relativePos[0] * relativePos[1] * relativePos[2];
	}
	return vectorFinal;
}