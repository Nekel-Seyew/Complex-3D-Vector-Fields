#pragma once
class vector3d {
protected:
	float xyz[3];
public:
	vector3d(float a, float b, float c);
	vector3d(float xyz[3]);
	vector3d();
	~vector3d();
	
	inline float x();
	inline float y();
	inline float z();

	//class stuff
	static void  Cross(float[3], float[3], float[3]);
	static float Dot(float[3], float[3]);
	static float Unit(float[3], float[3]);
};

