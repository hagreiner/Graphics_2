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

// ****TO-DO: 
//	1) declare uniform variable for texture; see demo code for hints
//	2) declare inbound varying for texture coordinate
//	3) sample texture using texture coordinate
//	4) assign sample to output color

out vec4 rtFragColor;
in vec2 vTexCoord;
uniform double uTime;
uniform vec2 u2DPosition;
uniform vec3 uColorFractal1;
uniform vec3 uColorFractal2;
uniform vec3 uColorFractal3;
uniform vec3 uColorFractal4;
uniform vec2 uZoom;

const int iterations = 10;
float zoom = 5;

void main()
{
	vec2 coordinates = vTexCoord; //make view easier -> reset this!!
	float lineScale = 1.0;
	vec3 color = vec3(0.0);
	coordinates = vec2(coordinates.x - 0.5, coordinates.y - 0.5) * zoom; //centering, moving, zooming -> change variables
	
	float angle = 5.0/6.0 * 3.14;
	vec2 normalized = vec2(sin(angle), cos(angle));

	int index;
	coordinates.x = abs(coordinates.x); //flip
	coordinates.x += 0.5; //flip

    for (index = 0; index < iterations; ++index) {
		coordinates *= 3.0;
		coordinates.x -= 1.5;
		lineScale *= 3.0;
		
		coordinates.x = abs(coordinates.x) - 0.5; //flip
		coordinates -= normalized * min(0.0, dot(coordinates, normalized)) * 2; 
	}


	float distance = length(coordinates - vec2(clamp(coordinates.x, -1.0, 1.0), 0));   
	color += smoothstep(0.01, 0.0, distance/lineScale);
	coordinates /= lineScale;
	//color.rg += coordinates;
	rtFragColor = vec4(color, 1.0);
}
//https://www.youtube.com/watch?v=il_Qg9AqQkE&t=1410s