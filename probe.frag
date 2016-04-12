#version 150 compatibility

//uniform float		uMin;
//uniform float		uMax;
//uniform float		uTol;
//uniform sampler3D	TexUnit;

in vec4  vColor;
in float vLightIntensity;
in vec3 vECposition;



vec3
Rainbow( float t )
{
	t = clamp( t, 0., 1. );

	vec3 rgb;

	// b -> c
	rgb.r = 0.;
	rgb.g = 4. * ( t - (0./4.) );
	rgb.b = 1.;

	// c -> g
	if( t >= (1./4.) )
	{
		rgb.r = 0.;
		rgb.g = 1.;
		rgb.b = 1. - 4. * ( t - (1./4.) );
	}

	// g -> y
	if( t >= (2./4.) )
	{
		rgb.r = 4. * ( t - (2./4.) );
		rgb.g = 1.;
		rgb.b = 0.;
	}

	// y -> r
	if( t >= (3./4.) )
	{
		rgb.r = 1.;
		rgb.g = 1. - 4. * ( t - (3./4.) );
		rgb.b = 0.;
	}

	return rgb;
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
	float		uMin = 0.;
	float		uMax = 100.;
	float		uTol = 1.0;
	//uniform sampler3D	TexUnit;
	vec3 stp = ( vECposition.xyz + 1. ) / 2.;
	// maps [-1.,1.] to [0.,1.]

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
	float scalar = rgba.r;

	if( scalar < uMin )
		discard;

	if( scalar > uMax )
		discard;

	// want to do gaps every 10 degrees
	// now see how close we are to a 10 degree place:

	float scalar10 = float( 10*int( (scalar+5.)/10. ) );
	if( abs( scalar - scalar10 ) > uTol )
		discard;
	float SMIN =   0.;
	float SMAX = 120.;
	float t = ( scalar - SMIN ) / ( SMAX - SMIN );
	vec3 rgb = Rainbow( t );

	//gl_FragColor = vec4( rgb, 1. );
	gl_FragColor = vec4(0., 1., 0., 1);
}
