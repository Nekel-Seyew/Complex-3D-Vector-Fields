#version 150 compatibility

//uniform float		uMin;
//uniform float		uMax;
//uniform float		uTol;
//uniform sampler3D	TexUnit;
uniform float VectorMax;
uniform float VectorMin;
in vec4  vColor;
in float vLightIntensity;
in vec3 vECposition;
in float vecMag;
in vec3 tnorm;

vec3
 FireColor( void){

	vec3 rgba;
	float range = VectorMax - VectorMin;
	float firstThird = VectorMin + 0.33333 * range;
	float secondThird = VectorMin + 0.66666 * range;
	if (vecMag < firstThird) {
			float interpR = (vecMag - VectorMin) / (firstThird - VectorMin);
			rgba.r = interpR;
			rgba.g = 0.;
			rgba.b = 0.;
	}
		else if (vecMag < secondThird) {
			float interpG = (vecMag - firstThird) / (secondThird - firstThird);
			rgba.r = 1.;
			rgba.g = interpG;
			rgba.b = 0.;

		}
		else {
			float interpB = (vecMag - secondThird) / (VectorMax- secondThird);
			rgba.r = 1.;
			rgba.g = 1.;
			rgba.b = interpB;
		}
		return rgba;	
}



vec3
Rainbow( void )
{
	vec3 rgba;
	float range = VectorMax - VectorMin;
	float firstQuarter = VectorMin + 0.25 * range;
	float secondQuarter = VectorMin + 0.50 * range;
	float thirdQuarter = VectorMin + 0.75 * range;

	if (vecMag < firstQuarter) {
			float interpR = (vecMag - VectorMin) / (firstQuarter - VectorMin);
			rgba.r = 0.;
			rgba.g = interpR;
			rgba.b = 1.;
	}
		else if (vecMag < secondQuarter) {
			float interpG = (vecMag - firstQuarter) / (secondQuarter - firstQuarter);
			rgba.r = 0.;
			rgba.g = 1.;
			rgba.b = 1 - interpG;

		}
		else if (vecMag < thirdQuarter) {
			float interpG = (vecMag - secondQuarter) / (thirdQuarter - secondQuarter);
			rgba.r = interpG;
			rgba.g = 1.;
			rgba.b = 0.;

		}
		else {
			float interpB = (vecMag - thirdQuarter) / (VectorMax- thirdQuarter);
			rgba.r = 1.;
			rgba.g = 1 - interpB;
			rgba.b = 0.;
		}
		return rgba;
}

void HsvRgb(vec3 hsv, vec3 rgb){
	float r, g, b;			// red, green, blue

	// guarantee valid input:

	float h = hsv.r / 60.f;
	while (h >= 6.f)	h -= 6.f;
	while (h <  0.f) 	h += 6.f;

	float s = hsv.g;
	if (s < 0.f)
		s = 0.f;
	if (s > 1.f)
		s = 1.f;

	float v = hsv.b;
	if (v < 0.f)
		v = 0.f;
	if (v > 1.f)
		v = 1.f;


	// if sat==0, then is a gray:

	if (s == 0.0f){
		rgb.r = rgb.g = rgb.b = v;
		return;
	}

	// get an rgb from the hue itself:

	float i = floor(h);
	float f = h - i;
	float p = v * (1.f - s);
	float q = v * (1.f - s*f);
	float t = v * (1.f - (s * (1.f - f)));
	int Inti = int(i);
	switch (Inti){
		case 0:
			r = v;	g = t;	b = p;
			break;

		case 1:
			r = q;	g = v;	b = p;
			break;

		case 2:
			r = p;	g = v;	b = t;
			break;

		case 3:
			r = p;	g = q;	b = v;
			break;

		case 4:
			r = t;	g = p;	b = v;
			break;

		case 5:
			r = v;	g = p;	b = q;
			break;
	}


	rgb.r = r;
	rgb.g = g;
	rgb.b = b;
}
vec3 Color(float VecMag) {
	vec3 hsv, rgb;
	// finally draw the point if it passes all the tests
	float range = (VectorMax - VectorMin);
	vec3 Crgb;
	if (range == 0) {
		hsv.r = 240. - 240.* ((VecMag - VectorMin) / 1.);
		}
	else {
		hsv.r = 240. - 240.* ((VecMag - VectorMin) / range);
	}
	hsv.g = 1.;
	hsv.b = 1.;
	HsvRgb(hsv, rgb);	
	Crgb.r = rgb.r;
	Crgb.g = rgb.g;
	Crgb.b = rgb.b;
	return Crgb;
}

vec3
HeatedObject( float t )
{
	t = clamp( t, 0., 1. );

	vec3 rgb;
	rgb.r = 3. * ( t - (0./6.) );
	rgb.g = 0.;
	rgb.b = 0.;

	if( t >= (1./3.) )
	{
		rgb.r = 1.;
		rgb.g = 3. * ( t - (1./3.) );
	}

	if( t >= (2./3.) )
	{
		rgb.g = 1.;
		rgb.b = 3. * ( t - (2./3.) );
	}

	return rgb;
}



void
main( )
{
	float		uMin = VectorMin;
	float		uMax = VectorMax;
	float		uTol = 1.0;
	//uniform sampler3D	TexUnit;
	vec3 stp = ( vECposition.xyz + 1. ) / 2.;
	// maps [-1.,1.] to [0.,1.]
	float scalar = vecMag; 
	/*
	if( any(    lessThan( stp, vec3(0.,0.,0.) ) ) )
	{
		discard;
	}
	
	if( any( greaterThan( stp, vec3(1.,1.,1.) ) ) )
	{
		discard;
	}

	//vec4 rgba = texture3D( TexUnit, stp );
	vec4 rgba = vec4(1., 0. , 0., 1.);

	if( scalar < uMin )
		discard;

	if( scalar > uMax )
		discard;

	// want to do gaps every 10 degrees
	// now see how close we are to a 10 degree place:

	float scalar10 = float( 10*int( (scalar+5.)/10. ) );
	if( abs( scalar - scalar10 ) > uTol )
		discard;
	*/
	float SMIN =   VectorMin;
	float SMAX = VectorMax;
	float t = ( 1. - VectorMin ) / ( VectorMax - VectorMin );
	//vec3 rgb = Rainbow( t );
	//t = clamp( t, 0., 1. );
	vec3 rgb = Rainbow();
	gl_FragColor = vec4( rgb, 1. );
	//gl_FragColor = vec4(0., VectorMax, 0., 1);
}
