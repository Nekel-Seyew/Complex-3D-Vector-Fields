#ifndef VEC3_H
#define VEC3_H

#include <stdio.h>
#include <math.h>

typedef float Vec3[3];

float	Dot( Vec3, Vec3 );
void	Cross( Vec3, Vec3, Vec3 );
float	Unit( Vec3, Vec3 );
char *	WhoAmI( );

#endif
