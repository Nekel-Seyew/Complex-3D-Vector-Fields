#version 150

in vec3 vpos;
in vec3 InterPNormal;
out vec4 color;

uniform int clickingflag;
uniform vec3 PickingColor;
uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
uniform mat4 ModelView;
uniform mat4 Projection;
uniform vec4 LightPosition;
uniform float Shininess;

void main()
{
vec3 L = normalize( (ModelView * LightPosition).xyz - vpos );
    vec3 E = normalize( -vpos );
    vec3 H = normalize( L + E );  //halfway vector

// Compute terms in the illumination equation
    vec4 ambient = AmbientProduct;

    float dr = max( dot(L, InterPNormal), 0.0 );
    vec4  diffuse = dr *DiffuseProduct;

    float sr = pow( max(dot(InterPNormal, H), 0.0), Shininess );
    vec4  specular = sr * SpecularProduct;

    if( dot(L, InterPNormal) < 0.0 ) {
	specular = vec4(0.0, 0.0, 0.0, 1.0);
    }
if (clickingflag != 0){
	color = vec4(PickingColor, 1.0);
	}
	else{
    color = ambient + diffuse + specular;
    color.a = 1.0;
	}
	
}
