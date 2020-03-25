/*
	Copyright 2011-2020 Daniel S. Buckstein

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

		http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/

/*
	animal3D SDK: Minimal 3D Animation Framework
	By Daniel S. Buckstein
	
	drawTexture_fs4x.glsl
	Draw texture sample.
*/

#version 410


out vec4 rtFragColor;
in vec2 vTexCoord;
uniform double uTime;
uniform vec2 u2DPosition;
uniform vec2 uZoom;

const int iterations = 25;
float zoom = uZoom.x;

void main()
{
	vec2 coordinates = vTexCoord; //make view easier
	float lineScale = 0.1;
	vec3 color = vec3(0.0);

	// position offset
	vec2 positionOffset = u2DPosition;
	float xOffset = positionOffset.x + 0.5;
	float yOffset = positionOffset.y + 0.5; 
	coordinates = vec2(coordinates.x - xOffset, coordinates.y - yOffset) * zoom;
	
	float angle = 2.0/3.0 * 3.14;
	vec2 normalized = vec2(sin(angle), cos(angle));

	// coordinate flip over axis
	int index;
	coordinates.x = abs(coordinates.x); //flip
	coordinates.x += 0.5; //flip
	coordinates.y = abs(coordinates.y); //flip

	// repeat and split the pattern
    for (index = 0; index < iterations; ++index) {
		coordinates *= 3.0;
		coordinates.x -= 1.5+(float(uTime)/100);	//separate over times
		lineScale *= 3.0;
		
		coordinates.x = abs(coordinates.x) - 0.5; //flip
		coordinates -= normalized * min(0.0, dot(coordinates, normalized)) * 2; 
	}

	// apply lines width change over distance
	float distance = length(coordinates - vec2(clamp(coordinates.x, -1.0, 1.0), 0));   
	color += smoothstep(0.01, 0.0, distance/lineScale);
	coordinates /= lineScale;
	
	// add coordinates to output color
	color.rb += coordinates;
	rtFragColor = vec4(color, 1.0);
}
//https://www.youtube.com/watch?v=il_Qg9AqQkE&t=1410s
//https://www.khanacademy.org/math/geometry-home/geometry-volume-surface-area/koch-snowflake/v/koch-snowflake-fractal