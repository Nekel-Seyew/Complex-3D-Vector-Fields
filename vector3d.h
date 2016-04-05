#pragma once
#include <cfloat>
class vector3d{
public:
	//constructors
	vector3d(float a, float b, float c);
	vector3d(float a, float b, float c, int mode);
	vector3d(float* v, int mode);
	vector3d(vector3d* in);

	//methods
	float* xyz(); //rectangular
	float* rtz(); //cylindrical
	float* rtp(); //spherical

	float magnitude();
	float magsqr();

	enum MODE {
		rect = 0,
		cylindrical = 1,
		spherical = 2
	};

	static double distance_sqr(vector3d* a, vector3d* b);

	//needed for hashtables
	struct VectorHash {
		//made this hash up, should be good though....
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
	struct VectorEqual {
		//works because long long int is bigger than any value of float,
		//thus this converts all numbers into whole number int space 
		//which we can then compare
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

private:
	//variables
	int    mode; //0-rect, 1-cylindrival, 2-spherical
	float  abc[3];
	//methods
	float* to_rect();
	float* to_cyl();
	float* to_sph();

	union converter {
		float flt;
		size_t szt;
	};
};

