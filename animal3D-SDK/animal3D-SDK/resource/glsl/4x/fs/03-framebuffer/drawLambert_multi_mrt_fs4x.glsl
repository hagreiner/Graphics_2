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
	
	drawLambert_multi_mrt_fs4x.glsl
	Draw Lambert shading model for multiple lights with MRT output.
*/

#version 410

// ****TO-DO: 
//	1) declare uniform variable for texture; see demo code for hints
//	2) declare uniform variables for lights; see demo code for hints
//	3) declare inbound varying data
//	4) implement Lambert shading model
//	Note: test all data and inbound values before using them!
//	5) set location of final color render target (location 0)
//	6) declare render targets for each attribute and shading component

layout (location = 0) out vec4 rtFragColor; // 5

//6 
layout(location = 1) out vec4 rtFragColorPosition;	
layout(location = 2) out vec4 rtFragColorNormal;	
layout(location = 3) out vec4 rtFragColorTexCoord;	
layout(location = 4) out vec4 rtFragColorDiffuse;
layout(location = 6) out vec4 rtFragColorDiffuseTotal;	

uniform sampler2D uTex_dm; //1

//2
const int size = 12;
uniform vec4 uLightPos[size];
uniform int uLightCt;
uniform float uLightSz[size];
uniform float uLightSzInvSq[size];
uniform vec4 uLightCol[size];

//3
in vec4 vNormal;
in vec2 vTexCoord;
in vec4 vViewPosition;

void main()
{
	// DUMMY OUTPUT: all fragments are OPAQUE RED
	vec4 diffuseMap = texture(uTex_dm, vTexCoord);

	float diffuseCoef = 0.0;
	vec3 color = vec3(0.0, 0.0, 0.0);
	vec3 diffuseTotal;
	
	// https://www.tomdalling.com/blog/modern-opengl/07-more-lighting-ambient-specular-attenuation-gamma/
	float attenuation = 0.0;
	vec3 N = normalize(vNormal).xyz;

	for (int index=0; index < size && index < uLightCt; index++){
		vec3 L = normalize(uLightPos[index] - vViewPosition).xyz;

		float lightDistance = length(uLightPos[index] - vViewPosition);

		diffuseCoef = max(dot(N, L), 0);
		diffuseTotal += diffuseCoef * uLightCol[index].xyz;
		attenuation = 1 / (1 + uLightSzInvSq[index] * (lightDistance * lightDistance));

		color += attenuation * (diffuseCoef * diffuseMap.xyz * uLightCol[index].xyz);
	}

	rtFragColor = vec4(color, 1.0);

	//6
	rtFragColorPosition = vViewPosition;
	rtFragColorNormal = vec4(N, 1.0);	
	rtFragColorTexCoord = vec4(vTexCoord, 0.0, 1.0);	
	rtFragColorDiffuse = diffuseMap;
	rtFragColorDiffuseTotal = vec4(diffuseTotal, 1.0);
}