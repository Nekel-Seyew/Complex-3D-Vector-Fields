#version 330 compatibility

in  vec4 vPosition;
in  vec4 vNormal;
out vec3 InterPNormal;
out vec3 vpos;

//uniform int clickingflag;
//uniform vec3 PickingColor;
uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
uniform mat4 ModelView;
uniform mat4 Projection;
uniform vec4 LightPosition;
uniform float Shininess;

void main()
{
    // Transform vertex  position into eye coordinates
	vpos = (ModelView * vPosition).xyz;
 

    // Transform vertex normal into eye coordinates
    InterPNormal = normalize( ModelView*vNormal ).xyz;

    //To correctly transform normals
    // vec3      InterPNormal = (normalize (transpose (inverse (ModelView))*vNormal).xyz

    
    gl_Position = Projection * ModelView * vPosition;

	
}
