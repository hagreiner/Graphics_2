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
	
	drawPhong_multi_deferred_fs4x.glsl
	Draw Phong shading model by sampling from input textures instead of 
		data received from vertex shader.
*/

#version 410

#define MAX_LIGHTS 4

// ****TO-DO: 
//	0) copy original forward Phong shader
//	1) declare g-buffer textures as uniform samplers
//	2) declare light data as uniform block
//	3) replace geometric information normally received from fragment shader 
//		with samples from respective g-buffer textures; use to compute lighting
//			-> position calculated using reverse perspective divide; requires 
//				inverse projection-bias matrix and the depth map
//			-> normal calculated by expanding range of normal sample
//			-> surface texture coordinate is used as-is once sampled

in vec4 vTexcoord;
vec2 textCoord = vec2(vTexcoord);

uniform sampler2D uTex_dm; //0.1
uniform sampler2D uTex_sm; //0.1
//postion, normal, texcoord, depth
uniform sampler2D uImage01; 
uniform sampler2D uImage02; 
uniform sampler2D uImage03; 
uniform sampler2D uImage07; 


vec2 tcCord = texture(uImage03, textCoord).xy; 
vec4 tcMap = texture(uImage03, textCoord); 
vec4 normalMap = texture(uImage02, textCoord);
vec4 posSample = texture(uImage01, textCoord);
vec4 depthMap = texture(uImage07, textCoord); 

vec4 specMap = texture(uTex_sm, tcCord);
vec4 diffuseMap = texture(uTex_dm, tcCord);

layout (location = 0) out vec4 rtFragColor;
layout (location = 1) out vec4 rtPosition;
layout (location = 2) out vec4 rtNormal;
layout (location = 3) out vec4 rtAtlasCoordinate;
layout (location = 4) out vec4 rtDiffuseMapSample;
layout (location = 5) out vec4 rtSpecularMapSample;
layout (location = 6) out vec4 rtDiffuseLightTotal;
layout (location = 7) out vec4 rtSpecularLightTotal;

//0.2
const int size = 12;
uniform vec4 uLightPos[size];
uniform int uLightCt;
uniform float uLightSz[size];
uniform float uLightSzInvSq[size];
uniform vec4 uLightCol[size];;

void main()
{
	float diffuseCoef = 0.0;
	float specularCoef = 0.0;
	vec3 color = vec3(0.0, 0.0, 0.0);
	vec3 diffuseTotal;
	vec3 specTotal;
	
	// https://www.tomdalling.com/blog/modern-opengl/07-more-lighting-ambient-specular-attenuation-gamma/
	float attenuation = 0.0;
	vec3 N = vec3(normalize(normalMap) * 2 - 0.35).rgb;
	
	for (int index=0; index < size && index < uLightCt; index++){
		vec3 L = normalize(uLightPos[index] - posSample).xyz;
		vec3 R = reflect(-L, N);
		vec3 V = normalize(posSample.xyz);
		float lightDistance = length(uLightPos[index] - posSample);
	
		diffuseCoef = max(dot(N, L), 0);
		specularCoef = pow(max(dot(R, V), 0), 30);
	
		attenuation = 1 / (1 + uLightSzInvSq[index] * (lightDistance * lightDistance));
		
		diffuseTotal += diffuseCoef * uLightCol[index].xyz;
		specTotal += specularCoef * uLightCol[index].xyz;
	
		color += attenuation * ((diffuseCoef * diffuseMap.xyz * uLightCol[index].xyz * 0.7) + 
		(specularCoef * specMap.xyz * uLightCol[index].xyz * 0.7));
	}
	
	rtFragColor = vec4(color, 1.0);
	rtDiffuseMapSample = diffuseMap;
	rtSpecularMapSample = specMap;
	rtDiffuseLightTotal = vec4(diffuseTotal, 1.0);
	rtSpecularLightTotal = vec4(specTotal, 1.0);

	rtPosition = posSample;
	rtNormal = vec4(N, 1.0);
	rtAtlasCoordinate = tcMap;
}
