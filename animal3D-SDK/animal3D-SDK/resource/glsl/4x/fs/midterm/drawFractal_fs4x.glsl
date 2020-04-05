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

uniform int uHeight;
uniform int uWidth;

uniform double uTime;
uniform vec2 u2DPosition;
uniform vec3 uColorFractal1;
uniform vec3 uColorFractal2;
uniform vec3 uColorFractal3;
uniform vec3 uColorFractal4;
uniform vec2 uZoom;

const int iterations = 1000;
float zoom = 100;

void main()
{    
    // position values
    float time = pow(2, float(uZoom.x)/2.0);
    zoom /= time;
    float xOffset = u2DPosition.x * (time * 0.1) + 0.5;
    float yOffset = u2DPosition.y * (time * 0.1) + 0.5;

    float realTemp  = (vTexCoord.x - xOffset) * zoom; 
    float imagTemp  = (vTexCoord.y - yOffset) * zoom; 
    float RealFloat = realTemp;
    float ImaginaryFloat = imagTemp;

    // needed values for later when applying color
    float combinedFloat = 0.0;
    int index;

    // fractal pattern loop
    for (index = 0; index < iterations && combinedFloat < 4.0; ++index) {
        float altReal = realTemp;
        
        realTemp = (altReal * altReal) - (imagTemp * imagTemp) + RealFloat;
        imagTemp = 2.0 * (altReal * imagTemp) + ImaginaryFloat;

        combinedFloat = (realTemp * realTemp) + (imagTemp * imagTemp);
    }

    // color apply
    vec3 color;
    if (combinedFloat < 3.0) { color = mix(uColorFractal1, uColorFractal2, fract(float(index)*0.1)); }
    else { color = mix(uColorFractal3, uColorFractal4, fract(float(index)*0.1)); }

    color = clamp(color, 0.0, 1.0); // clamp values so it is not blown out
    rtFragColor = vec4(color, 1.0); // output color
}
//https://community.khronos.org/t/simple-mandelbrot-shader/62721/11