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
	
	drawPhongVolume_fs4x.glsl
	Draw Phong lighting components to render targets (diffuse & specular).
*/

#version 410

#define MAX_LIGHTS 1024

// ****TO-DO: 
//	0) copy deferred Phong shader
//	1) declare g-buffer textures as uniform samplers
//	2) declare lighting data as uniform block
//	3) calculate lighting components (diffuse and specular) for the current 
//		light only, output results (they will be blended with previous lights)
//			-> use reverse perspective divide for position using scene depth
//			-> use expanded normal once sampled from normal g-buffer
//			-> do not use texture coordinate g-buffer

in vec4 vBiasedClipCoord;
flat in int vInstanceID;

//layout (location = 6) out vec4 rtDiffuseLight;
//layout (location = 7) out vec4 rtSpecularLight;

struct sPointLight {
		vec4 worldPos;					// position in world space
		vec4 viewPos;						// position in viewer space
		vec4 color;						// RGB color with padding
		float radius;						// radius (distance of effect from center)
		float radiusInvSq;					// radius inverse squared (attenuation factor)
		float pad[2];						// padding
};

uniform vbPointLighting {
	sPointLight uLight[MAX_LIGHTS];
};

//in vec4 vTexcoord;

uniform sampler2D uImage01; //bias pos
uniform sampler2D uImage02; //normal
uniform sampler2D uImage03; //coord
uniform sampler2D uImage04; //diffuse
uniform sampler2D uImage05; //spec

uniform mat4 uPB_inv;

//layout (location = 0) out vec4 rtFragColor;
//layout (location = 1) out vec4 rtPosition;
//layout (location = 2) out vec4 rtNormal;
//layout (location = 3) out vec4 rtAtlasCoordinate;
//layout (location = 4) out vec4 rtDiffuseMapSample;
//layout (location = 5) out vec4 rtSpecularMapSample;
layout (location = 6) out vec4 rtDiffuseLightTotal;
layout (location = 7) out vec4 rtSpecularLightTotal;

//0.2
const int size = 12;
uniform vec4 uLightPos[MAX_LIGHTS];
uniform int uLightCt;
uniform float uLightSz[MAX_LIGHTS];
uniform float uLightSzInvSq[MAX_LIGHTS];
uniform vec4 uLightCol[MAX_LIGHTS];

void main()
{
	vec2 textCoord = (vBiasedClipCoord / vBiasedClipCoord.w).xy;
	vec2 tsCord = texture(uImage03, textCoord).xy; 
	vec4 tcMap = texture(uImage03, textCoord); 

	vec3 N = texture(uImage02, textCoord).rgb;

	vec4 biasSample = texture(uImage01, textCoord);
	biasSample = uPB_inv* biasSample;
	biasSample = biasSample / biasSample.w;
	
	vec4 diffuseMap = texture(uImage04, tsCord);
	vec4 specMap = texture(uImage05, tsCord);
	
	vec3 L = normalize(uLight[vInstanceID].viewPos.xyz - biasSample.xyz); 
	vec3 R = reflect(-L, N);
	vec3 V = normalize(-biasSample.xyz);
	float lightDistance = length(biasSample.xyz- uLight[vInstanceID].viewPos.xyz); 
	
	// https://www.tomdalling.com/blog/modern-opengl/07-more-lighting-ambient-specular-attenuation-gamma/
	// https://thebookofshaders.com/glossary/?search=smoothstep
	float attenuation = smoothstep(uLight[vInstanceID].radius, 0, lightDistance);
	
	float diffuseCoef = max(dot(N, L), 0);
	float specularCoef = pow(max(dot(R, V), 0), 30);

	vec4 diffuseTotal = diffuseCoef * uLight[vInstanceID].color;
	vec4 specTotal = specularCoef * uLight[vInstanceID].color;
	
	vec4 color = attenuation * (diffuseTotal + specTotal);
	
	//rtFragColor = color;
	//rtDiffuseMapSample = diffuseMap;
	//rtSpecularMapSample = specMap;
	rtDiffuseLightTotal = diffuseTotal;
	rtSpecularLightTotal = specTotal;

	//rtPosition = biasSample;
	//rtNormal = vec4(N, 1.0);
	//rtAtlasCoordinate = tcMap;
}
