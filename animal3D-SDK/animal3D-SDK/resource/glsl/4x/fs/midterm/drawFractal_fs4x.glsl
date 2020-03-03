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

const int iterations = 100;
const vec2 center = vec2(0.0);
const vec3 color_1 = vec3(1.0, 1.0, 0.5);
const vec3 color_2 = vec3(1.0, 0.2, 0.0);
const vec3 color_3 = vec3(1.0, 0.5, 0.0);
const vec3 color_4 = vec3(1.0, 1.0, 0.0);
const vec3 color_5 = vec3(1.0, 0.5, 1.0);
const vec3 color_6 = vec3(1.0, 1.0, 1.0);
const vec3 color_7 = vec3(1.0, 1.0, 0.0);
const vec3 color_8 = vec3(0.5, 1.0, 0.5);
float zoom = 25;

void main()
{
    float time = pow(2, float(uTime)/2.0);
    zoom /= time;
    float xOffset = u2DPosition.x;
    float yOffset = u2DPosition.y;

    float realTemp  = (vTexCoord.x - xOffset) * zoom; 
    float imagTemp  = (vTexCoord.y - yOffset) * zoom; 
    float RealFloat = realTemp;
    float ImaginaryFloat = imagTemp;

    float combinedFloat = 0.0;
    int index;

    for (index = 0; index < iterations && combinedFloat < 4.0; ++index) {
        float altReal = realTemp;
        
        realTemp = (altReal * altReal) - (imagTemp * imagTemp) + RealFloat;
        imagTemp = 2.0 * (altReal * imagTemp) + ImaginaryFloat;

        combinedFloat = (realTemp * realTemp) + (imagTemp * imagTemp);
    }

    // color apply
    vec3 color;
    if (combinedFloat < 5.0) { color = mix(color_3, color_4, fract(index*0.1)); }
    else if (combinedFloat > 10.0 && combinedFloat < 20.0 ) { color = mix(color_4, color_5, float(index)*0.1); }
    else if (combinedFloat > 20.0) { color = mix(color_7, color_8, float(index)*0.04); }
    else { color = mix(color_1, color_2, float(index)*0.04); }

    color = clamp(color, 0.0, 1.0);
    rtFragColor = vec4(color, 1.0);
}
//https://community.khronos.org/t/simple-mandelbrot-shader/62721/11