/**
@file
@author Kyle Sweeney

@section LICENSE

Copyright 2016 Kyle Sweeney

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

The Headerfile for the vector3d class: one of the fundamental data-pieces for the entire project.

*/
#pragma once
#include <cfloat>
/**
A class which represents a 3-Dimensional vector value.
*/
class vector3d{
public:
	//constructors
	/**
	A constructor which takes in an A,B,C values, and defaults to rectangular coordinates.
	@param  a "x" value
	@param  b "y" value
	@param  c "z" value
	*/
	vector3d(float a, float b, float c);
	/**
	A constructor which takes in an A,B,C values, and the mode for the vector
	@param  a "x" value
	@param  b "y" value
	@param  c "z" value
	@param  mode the mode the pased in data value is in.
	*/
	vector3d(float a, float b, float c, int mode);
	/**
	A constructor which takes in a 3-wide float array and mode.
	@param  v the 3-item float array.
	@param  mode the mode the passed in data is in.
	*/
	vector3d(float* v, int mode);
	/**
	A constructor which makes a copy of the passed in value.
	@param  in the vector to copy.
	*/
	vector3d(vector3d* in);
	/**
	A copy constructor.
	@param  in the original to copy.
	*/
	vector3d(const vector3d& in);
	/**
	The default constructor.
	*/
	vector3d();
	/**
	The default destructor.
	*/
	~vector3d();

	//methods
	/**
	Converts the vector to be in rectangular coordinates and returns the 3-wide float array of values.
	@returns the 3-wide float array of values. Does not create memory.
	*/
	float* xyz(); //rectangular
	/**
	Converts the vector to be in cylindrical coordinates and returns the 3-wide float array of values.
	@returns the 3-wide float array of values. Does not create memory.
	*/
	float* rtz(); //cylindrical
	/**
	Converts the vector to be in spherical coordinates and returns the 3-wide float array of values.
	@returns the 3-wide float array of values. Does not create memory.
	*/
	float* rtp(); //spherical

	/**
	Calculates the magnitude of this vector.
	@returns the magnitude of this vector.
	*/
	float magnitude();
	/**
	Calculates the square of this vector.
	@returns the square of the magnitude of this vector.
	*/
	float magsqr();

	/**
	The mode a vector could be in: Rectangular, Cylindrical, or Spherical coordinates.
	*/
	enum MODE {
		rect = 0,
		cylindrical = 1,
		spherical = 2
	};

	/**
	Calculates the square of the distance between the two passed in points.
	@returns the square of the distance.
	*/
	static double distance_sqr(vector3d* a, vector3d* b);

	/**
	An internal Struct which allows vectors to be hashed into a hash-table.
	*/
	struct VectorHash {
		/**
		Generates a hash for a given vector3d.
		@param  k the vector to be hashed.
		@returns the hash-value
		*/
		size_t operator()(const vector3d& k) const {
			size_t h = 11;
			vector3d::converter x;
			x.flt = k.abc[0];
			h += x.szt * 13;
			x.flt = k.abc[1];
			h += x.szt * 15;
			x.flt = k.abc[2];
			h += x.szt * 17;
			return h;
		}
	};
	/**
	Calculates whther or not the two passed in vectors are, at a bit level, the same vector.

	Does not take mode into account, only the current state of the float arrays inside the vectors.
	*/
	struct VectorEqual {
		//works because long long int is bigger than any value of float,
		//thus this converts all numbers into whole number int space 
		//which we can then compare
		/**
		Calculates whether or not the two vectors, on a bit leve, are the same
		@param  lhs left vector
		@param  rhs right vector
		@returns whether or not they are the same vector.
		*/
		bool operator()(const vector3d& lhs, const vector3d& rhs) const { 
			long long int lhsx = (long long int)(lhs.abc[0] * FLT_MAX);
			long long int rhsx = (long long int)(rhs.abc[0] * FLT_MAX);
			long long int lhsy = (long long int)(lhs.abc[1] * FLT_MAX);
			long long int rhsy = (long long int)(rhs.abc[1] * FLT_MAX);
			long long int lhsz = (long long int)(lhs.abc[2] * FLT_MAX);
			long long int rhsz = (long long int)(rhs.abc[2] * FLT_MAX);

			return (lhsz == rhsx) && (lhsy == rhsy) && (lhsz == rhsz);
		}
	};

	/**
	Converts this vector to be exactly equal to the passed in vector value.
	@param  in the vector to copy
	*/
	void set_this_to_be_passed_in_value(vector3d* in);
	/**
	Converts this vector to be exactly equal to the passed in 3-wide float array value. Also assumes to be rectangular coordinates.
	@param  in the 3-wide float array to copy
	*/
	void set_this_to_be_passed_in_value(float* in);
	//sets all values to be 0
	/**
	Sets every internal value to 0.
	*/
	void nullify();

	/*operators*/
	//returns self
	/**
	Scalar multiplies this vector by passed in float.
	@param  rhs scalar value
	@returns self
	*/
	vector3d& operator*=(const float rhs);
	/**
	Scalar divides this vector by passed in float.
	@param  rhs scalar value
	@returns self
	*/
	vector3d& operator/=(const float rhs);
	/**
	Adds the passed in vector value to this vector.
	@param  rhs vector value
	@returns self
	*/
	vector3d& operator+=(const vector3d& rhs);
	//returns new
	/**
	Subtracts the passed in vector value from this vector.
	@param  rhs the right hand vector.
	@returns new vector equal to this - rhs.
	*/
	vector3d* operator-(const vector3d& rhs);
	/**
	Adds the passed in vector value to this vector.
	@param  rhs the right hand vector.
	@returns new vector equal to this + rhs.
	*/
	vector3d* operator+(const vector3d& rhs);
	//returns const
	//dot operator
	/**
	Performs a dot product on this vector and another vector.
	@param  rhs right hand vector
	@returns the dot product of this and other vector.
	*/
	float operator*(const vector3d& rhs);

	/**
	Performs a cross product on the two vectors, from v1 to v2.
	@param  v1 left hand vector
	@param  v2 right hand vector
	@returns new vector equal to v1 x v2
	*/
	static vector3d* Cross(vector3d* v1, vector3d* v2);
	
	/**
	Normalizes the vector, turning it into a unit vector.
	*/
	void unitize();

private:
	//variables
	int    mode; //0-rect, 1-cylindrival, 2-spherical
	float  abc[3];
	float  length;
	//methods
	float* to_rect();
	float* to_cyl();
	float* to_sph();

	union converter {
		float flt;
		size_t szt;
	};
};

