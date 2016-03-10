#version 330 compatibility

out vec4	vECposition;

void
main( )
{
	vECposition = gl_ModelViewMatrix  * gl_Vertex;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}