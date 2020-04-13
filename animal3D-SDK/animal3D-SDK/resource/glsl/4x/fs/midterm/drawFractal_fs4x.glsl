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
in vec4 vViewPosition;

uniform int uHeight;
uniform int uWidth;

uniform double uTime;
uniform vec2 u2DPosition;
uniform vec3 uColorFractal1;
uniform vec3 uColorFractal2;
uniform vec3 uColorFractal3;
uniform vec3 uColorFractal4;
uniform vec2 uZoom;
const int size = 12;
uniform vec4 uLightPos[size];

const int iterations = 25;
float zoom = 100;
float BailLimit = 50.0;
float Bias = 2.0;


float convertDE(in vec3 pos, inout int iter){
    vec3 z = pos;
	float dr = 1.0;
	float r = 0;
	float power = 8.0;
	
	for (int i = 0; i < iterations; i++) {
		r = length(z);
		if (r > BailLimit) {break;}
		
		//float theta = acos(z.z / r);
		float theta = atan(sqrt(z.x * z.x + z.y * z.y), z.z);
		float phi = atan(z.y, z.x);
		
		dr = max(dr * float(Bias), pow(r, power - 1.0) * power * dr + 1.0);
		//dr = pow(r, power - 1.0) * power * dr + 1.0;
		
		float zr = pow(r, power);
		theta = theta * power;
		phi = phi * power;

		z = zr * vec3(sin(theta) * cos(phi), sin(phi) * sin(theta), cos(theta));
		z += pos;
	}
	return 0.5 * log(r) * r / dr;
}


vec3 calculateNormals(in vec3 pos) {
    vec3 eps = vec3(0.005,0.0,0.0);
	//return normalize( vec3(
    //   convertDE(pos+eps.xyy) - convertDE(pos-eps.xyy),
    //   convertDE(pos+eps.yxy) - convertDE(pos-eps.yxy),
    //   convertDE(pos+eps.yyx) - convertDE(pos-eps.yyx)
    //));
	return eps;
}


void main()
{    
    float zOffset = -2.5;
	float xOffset = zOffset * 0.5 - 0.25;
	vec3 offset = vec3(xOffset, xOffset, -0.5);

	vec2 uv = vTexCoord;
	float screenRatio = float(uWidth) / float(uHeight);
	uv.x *= screenRatio;
	vec3 la = vec3(0.0, 0.0, 1.0);
	
	vec3 position;
	vec3 L = vec3(0.0);
	vec3 color = vec3(1.0, 0.7, 0.2);
	vec3 color2 = vec3(0.0, 0.7, 0.2);
	for (int index=0; index < size; index++){
		L += normalize(uLightPos[index] - vViewPosition).xyz;
	}
	vec3 lights = normalize(L - position);
	
    vec3 mandelbulbPos = vec3(uv, zOffset);
	vec3 cameraPos = normalize(la - mandelbulbPos);
	vec3 renderDistance = normalize(cameraPos + vec3(uv, 0.0));
	
	vec3 mandelbulb;
	int iter;
	float totalDistance = 0.0;
	float zDistance = 250.0;

	for (int i = 0; i < iterations * 4; i++) {
		if (zDistance > 0.0001){
			mandelbulb = mandelbulbPos + renderDistance * totalDistance;
			zDistance = convertDE((mandelbulb + offset), iter);
			totalDistance += zDistance;
		}
	}

	vec3 ramp = mix(color, color2, float(iter) / float(iterations));
    rtFragColor = vec4(mandelbulb, 1.0); // output color
}
