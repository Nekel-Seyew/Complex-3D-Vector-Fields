#pragma once
class HeatMapCenters{
private:
	inline float SQR(float a);

public:
	HeatMapCenters();
	~HeatMapCenters();

	float Temperature(float x, float y, float z, float tempmax);
	

	//internal struct, because we can.
	typedef struct centers{
			float xc, yc, zc;       // center location
			float a;                // amplitude
	}Cent_t;

	//everyone gets one
	Cent_t Centers[4];

};

