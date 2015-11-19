#ifndef MAT4_H
#define MAT4_H

#include <stdio.h>
#include <math.h>
#include "vector.h"

typedef float Mat4[4][4];

void	SetIdentity( Mat4 );
void	SetScale( Mat4, float, float, float );
void	SetTranslate( Mat4, float, float, float );
void	SetRotateX( Mat4, float );
void	SetRotateY( Mat4, float );
void	SetRotateZ( Mat4, float );
void	MultMatrix( Mat4, Mat4,  Mat4 );
void	MultVector( Mat4, Vec3,  Vec3 );

#endif
