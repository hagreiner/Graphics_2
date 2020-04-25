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
in vec4 vNormal;

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
uniform int uLightCt;
uniform float uLightSz[size];
uniform float uLightSzInvSq[size];
uniform vec4 uLightCol[size];

int iterations = 25;
float zoom = 100;
float BailLimit = 50.0;
float Bias = 2.0;

uniform sampler2D uTex_ramp_sm;


vec3 rotation(in vec3 pos, in float rot) {
	float rotCos = cos(rot);
	float rotSin = sin(rot);

	mat3 rotX = mat3(1.0, 0.0, 0.0, 0.0, rotCos, rotSin, 0.0, -rotSin, rotCos);
	mat3 rotY = mat3(rotCos, 0.0, -rotSin, 0.0, 1.0, 0.0, rotSin, 0.0, rotCos);
	mat3 rotZ = mat3(rotCos, rotSin, 0.0, -rotSin, rotCos, 0.0, 0.0, 0.0, 1.0);
	
	return rotX * rotY * rotZ * pos;
}


float convertDE(in vec3 pos, inout int iter) {
	pos = rotation(pos, (float(uTime)));
    vec3 z = pos;
	float dr = 1.0;
	float r = 0;
	float power = 8.0;
	
	for (int iter = 0; iter < iterations; iter++) {
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


vec3 calculateNormals(in vec3 pos, in int iter) {
    vec3 eps = vec3(0.0005,0.0,0.0); //using 2d
	//normalize later
	return vec3(
       convertDE(pos+eps.xyy, iter) - convertDE(pos-eps.xyy, iter),
       convertDE(pos+eps.yxy, iter) - convertDE(pos-eps.yxy, iter),
       convertDE(pos+eps.yyx, iter) - convertDE(pos-eps.yyx, iter)
    );
}


vec3 lighting(vec3 normal, float coord){
	vec3 position;
	vec3 color;
	vec3 L = vec3(0.0);
	vec3 R = vec3(0.0);
	float specCoef, diffuseCoef, attenuation = 0.0;
	
	vec3 color_0 = vec3(0.1, 0.2, 0.7);
	vec3 color_1 = vec3(0.1, 0.6, 0.5);
	vec3 ramp = mix(color_0, color_1, coord);

	for (int index=0; index < size; index++){
		L = normalize(uLightPos[index] - vViewPosition).xyz;
		R = reflect(-L, normal);
		vec3 V = normalize(-vViewPosition.xyz);
		float lightDistance = length(uLightPos[index] - vViewPosition);
		
		attenuation = 1 / (1 + uLightSzInvSq[index] * (lightDistance * lightDistance));
		
		specCoef = pow(max(dot(R, V), 0), 30);
		diffuseCoef = max(dot(normal, L), 0);

		color += attenuation * ((specCoef * uLightCol[index].xyz * 0.5)  + (diffuseCoef * uLightCol[index].xyz));
	}
	return color;
}


void main() {    
	float minimumVal = 0.0001;
	float time = pow(2, float(uZoom.x)/2.0);
    zoom /= time;
    float xOffset = u2DPosition.x * (time * 0.1) + 0.5;
    float yOffset = u2DPosition.y * (time * 0.1) + 0.5;

	float xTemp  = (vTexCoord.x - xOffset) * zoom; 
    float yTemp  = (vTexCoord.y - yOffset) * zoom; 

	vec2 uv = vec2(xTemp, yTemp);
	float screenRatio = float(uWidth) / float(uHeight);
	uv.x *= screenRatio;
	vec3 la = vec3(0.0, 0.0, 1.0);
	
    float zOffset = -2.5;
	vec3 mandelbulbPos = vec3(uv, zOffset);
	vec3 cameraPos = normalize(la - mandelbulbPos);
	vec3 renderDistance = normalize(cameraPos + vec3(uv, 0.0));
	
	vec3 mandelbulb;
	int iter;
	float totalDistance = 0.0;
	float zDistance = 250.0;
	vec3 normals;

	for (int i = 0; i < (iterations * 4); i++) {
		if (zDistance > minimumVal){
			mandelbulb = mandelbulbPos + renderDistance * totalDistance;
			zDistance = convertDE(mandelbulb, iter);
			totalDistance += zDistance;
			normals += calculateNormals(mandelbulb, iter);
		}
	}
	float coord = float(iter) / float(iterations);
	normals = normalize(normals);
	vec3 lights = lighting(normals, coord);
	vec3 color = normals * vec3(0.2, 0.2, 0.5);
	
    rtFragColor = vec4(color  + lights, 1.0); // output color
}
