#version 330 compatibility

out vec4  vColor;
out float vLightIntensity;
out vec3 vECposition;
const vec3 LIGHTPOS = vec3( 0., 0., 10. );

void
main( )
{
	vec3 tnorm = normalize( gl_NormalMatrix * gl_Normal );
	vec3 vECposition = ( gl_ModelViewMatrix * gl_Vertex ).xyz;
	vLightIntensity  = abs( dot( normalize(LIGHTPOS - vECposition), tnorm ) );

	vColor = gl_Color;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}