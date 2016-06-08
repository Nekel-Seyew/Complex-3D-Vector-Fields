/**
@file
@author Corinne Brucks

@section LICENSE

Copyright 2016 Corinne Brucks

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

@section DESCRIPTION

This is a helper class which contains a static method to convert from an HSV color scheme to an RGB color scheme.

*/
#pragma once
/**
A Helper Class containing a static method to convert from an HSV color scheme to an RGB color scheme.
*/
class color
{
public:
	/**
	Default Constructor.
	*/
	color();
	/**
	Default Destructor
	*/
	~color();
	/// routine to convert HSV to RGB.
	///
	/// Reference:  Foley, van Dam, Feiner, Hughes,
	///		"Computer Graphics Principles and Practices,"
	///@param  hsv a 3-wide input array storing HSV values
	///@param  rgb a 3-wide output array storing RGB values
	static void  HsvRgb(float hsv[3], float rgb[3]);
};

