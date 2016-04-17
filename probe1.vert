#version 330 compatibility
//in  vec4 vPosition;
uniform float VectorMax;
uniform float VectorMin;
out vec4  vColor;
out float vLightIntensity;
out float vecMag; 
out vec3 vECposition;
out vec3 tnorm;

const vec3 LIGHTPOS = vec3( 0., 0., 10. );

void
main( )
{
	vecMag = gl_Vertex[3];
	vec4 modVert = vec4(gl_Vertex.xyz, 1.);
	vec3 tnorm = normalize( gl_NormalMatrix * gl_Normal );
	vec3 vECposition = ( gl_ModelViewMatrix * modVert ).xyz;
	vLightIntensity  = abs( dot( normalize(LIGHTPOS - vECposition), tnorm ) );
	vColor = gl_Color;
	gl_Position = gl_ModelViewProjectionMatrix * modVert;
}