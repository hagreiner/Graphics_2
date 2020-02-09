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
	
	drawPhong_multi_shadow_mrt_fs4x.glsl
	Draw Phong shading model for multiple lights with MRT output and 
		shadow mapping.
*/

#version 410

// ****TO-DO: 
//	0) copy existing Phong shader
//	1) receive shadow coordinate
//	2) perform perspective divide
//	3) declare shadow map texture
//	4) perform shadow test

layout (location = 0) out vec4 rtFragColor; // 0.5

//0.6 
layout(location = 1) out vec4 rtFragColorPosition;	
layout(location = 2) out vec4 rtFragColorNormal;	
layout(location = 3) out vec4 rtFragColorTexCoord;	
layout(location = 4) out vec4 rtFragColorDiffuse;
layout(location = 5) out vec4 rtFragColorSpec;
layout(location = 6) out vec4 rtFragColorDiffuseTotal;
layout(location = 7) out vec4 rtFragColorSpecTotal;

uniform sampler2D uTex_dm; //0.1
uniform sampler2D uTex_sm; //0.1

//0.2
const int size = 12;
uniform vec4 uLightPos[size];
uniform int uLightCt;
uniform float uLightSz[size];
uniform float uLightSzInvSq[size];
uniform vec4 uLightCol[size];

//0.3
in vec4 vNormal;
in vec2 vTexCoord;
in vec4 vViewPosition;

//1 
in vec4 vShadowCoord;
uniform sampler2D uTex_shadow;
uniform sampler2D uTex_proj;

void main()
{
	vec4 projScreen = vShadowCoord / vShadowCoord.w; //2

	vec4 diffuseMap = texture(uTex_dm, vTexCoord);
	vec4 specMap = texture(uTex_sm, vTexCoord);
	vec4 shadowMap = texture(uTex_dm, vTexCoord); //3

	float shadowSample = texture(uTex_dm, projScreen.xy).r; //3
	bool fragShadowed = (projScreen.z > shadowSample); //4

	float diffuseCoef = 0.0;
	float specularCoef = 0.0;
	vec3 color = vec3(0.0, 0.0, 0.0);
	vec3 diffuseTotal;
	vec3 specTotal;
	
	// https://www.tomdalling.com/blog/modern-opengl/07-more-lighting-ambient-specular-attenuation-gamma/
	float attenuation = 0.0;
	vec3 N = normalize(vNormal).xyz;

	for (int index=0; index < size && index < uLightCt; index++){
		vec3 L = normalize(uLightPos[index] - vViewPosition).xyz;
		vec3 R = reflect(-L, N);
		vec3 V = normalize(vViewPosition.xyz);
		float lightDistance = length(uLightPos[index] - vViewPosition);

		diffuseCoef = max(dot(N, L), 0);
		specularCoef = pow(max(dot(R, V), 0), 30);

		diffuseTotal += diffuseCoef * uLightCol[index].xyz;
		specTotal += specularCoef * uLightCol[index].xyz;

		attenuation = 1 / (1 + uLightSzInvSq[index] * (lightDistance * lightDistance));

		color += attenuation * ((diffuseCoef * diffuseMap.xyz * uLightCol[index].xyz) + 
		(specularCoef * specMap.xyz * uLightCol[index].xyz));
	}
	
	//4
	if (fragShadowed) {
		diffuseTotal *= 0.2; 
		specTotal *= 0.2;
	}

	rtFragColor = vec4(color, 1.0);

	//0.6
	rtFragColorPosition = vViewPosition;
	rtFragColorNormal = vec4(N, 1.0);	
	rtFragColorTexCoord = vec4(vTexCoord, 0.0, 1.0);	
	rtFragColorDiffuse = diffuseMap;
	rtFragColorDiffuseTotal = vec4(diffuseTotal, 1.0);

	rtFragColorSpec = specMap;
	rtFragColorSpecTotal = vec4(specTotal, 1.0);
}
