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
	
	drawPhong_multi_mrt_fs4x.glsl
	Draw Phong shading model for multiple lights with MRT output.
*/

#version 410

// ****TO-DO: 
//	1) declare uniform variables for textures; see demo code for hints
//	2) declare uniform variables for lights; see demo code for hints
//	3) declare inbound varying data
//	4) implement Phong shading model
//	Note: test all data and inbound values before using them!
//	5) set location of final color render target (location 0)
//	6) declare render targets for each attribute and shading component

layout (location = 0) out vec4 rtFragColor; // 5

//6 
layout(location = 1) out vec4 rtFragColorPosition;	
layout(location = 2) out vec4 rtFragColorNormal;	
layout(location = 3) out vec4 rtFragColorTexCoord;	
layout(location = 4) out vec4 rtFragColorDiffuse;
layout(location = 5) out vec4 rtFragColorSpec;
layout(location = 6) out vec4 rtFragColorDiffuseTotal;
layout(location = 7) out vec4 rtFragColorSpecTotal;

uniform sampler2D uTex_dm; //1
uniform sampler2D uTex_sm; //1

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
	vec4 specMap = texture(uTex_sm, vTexCoord);

	float diffuseCoef = 0.0;
	float specularCoef = 0.0;
	vec3 color = vec3(0.0, 0.0, 0.0);
	vec3 diffuseTotal;
	vec3 specTotal;
	
	// https://www.tomdalling.com/blog/modern-opengl/07-more-lighting-ambient-specular-attenuation-gamma/
	float attenuation = 0.0;

	for (int index=0; index < size && index < uLightCt; index++){
		vec3 N = normalize(vNormal).xyz;
		vec3 L = normalize(uLightPos[index] - vViewPosition).xyz;
		float lightDistance = length(uLightPos[index] - vViewPosition);

		diffuseCoef = max(dot(N, L), 0);
		specularCoef = pow(max(dot(R, V), 0), 30);

		diffuseTotal += diffuseCoef * diffuseMap.xyz;
		specTotal += specularCoef * specMap.xyz;

		attenuation = 1 / (1 + uLightSzInvSq[index] * (lightDistance * lightDistance));

		color += attenuation * ((diffuseCoef * diffuseMap.xyz * uLightCol[index].xyz) + 
		(specularCoef * specMap.xyz * uLightCol[index].xyz));
	}

	rtFragColor = vec4(color, 1.0);

	//6
	rtFragColorPosition = vViewPosition;
	rtFragColorNormal = vNormal;	
	rtFragColorTexCoord = vec4(vTexCoord, 0.0, 1.0);	
	rtFragColorDiffuse = diffuseMap;
	rtFragColorDiffuseTotal = vec4(diffuseTotal, 1.0);

	rtFragColorSpec = specMap;
	rtFragColorSpecTotal = vec4(specTotal, 1.0);
}