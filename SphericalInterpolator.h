/**
@file
@author Alex Davis

@section LICENSE

Copyright 2016 Alex Davis

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

@section DESCRIPTION

This is a helper class which contains a static method to perform a spherical interpolation.

*/
#pragma once
#include "vector3d.h"

/**
This is a helper class which contains a static method to perform a spherical interpolation.
*/
class SphericalInterpolator {
protected:
	float xyz[3];
public:
	/**
	Default Constructor
	*/
	SphericalInterpolator();
	/**
	Default Destructor
	*/
	~SphericalInterpolator();
	
	/**
	Performs an interpolation of vectors, creating a new one
	@param  vectorInput a series of vectors around the vector.
	@param  relativePos the postition of the current vector.
	@returns a new 3-wide array containing the values of the array.
	*/
	static float* Interpolate(float vectorInput[8][3], float relativePos[3]);
};