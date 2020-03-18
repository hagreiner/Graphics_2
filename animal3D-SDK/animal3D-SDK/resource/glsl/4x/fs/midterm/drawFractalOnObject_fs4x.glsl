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

layout (location = 0) out vec4 rtFragColor; // 0.5

//0.6 
layout(location = 1) out vec4 rtFragColorPosition;	
layout(location = 2) out vec4 rtFragColorNormal;	
layout(location = 3) out vec4 rtFragColorTexCoord;	
layout(location = 4) out vec4 rtFragColorShadowCoord;
layout(location = 5) out vec4 rtFragColorShadowTest;
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

void main()
{
	vec4 diffuseMap = texture(uTex_dm, vTexCoord);
	vec4 specMap = texture(uTex_sm, vTexCoord);

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

		attenuation = 1 / (1 + uLightSzInvSq[index] * (lightDistance * lightDistance));
		
		diffuseTotal += diffuseCoef * uLightCol[index].xyz;
		specTotal += specularCoef * uLightCol[index].xyz;

		color += attenuation * ((diffuseCoef * diffuseMap.xyz * uLightCol[index].xyz * 0.7) + 
		(specularCoef * specMap.xyz * uLightCol[index].xyz * 0.7));
	}
	color = vec3(1.0, 0.0, 0.0);
	rtFragColor = vec4(color, 1.0);
	//rtFragColor = vec4(1.0, 0.0, 0.0, 1.0);

	rtFragColorPosition = vViewPosition;
	rtFragColorNormal = vec4(N, 1.0);	
	rtFragColorTexCoord = vec4(vTexCoord, 0.0, 1.0);	
	
	rtFragColorShadowCoord = vec4(1.0, 0.0, 0.0, 1.0);;
	rtFragColorShadowTest = vec4(1.0, 0.0, 0.0, 1.0);;

	rtFragColorSpecTotal = vec4(specTotal, 1.0);
	rtFragColorDiffuseTotal = vec4(diffuseTotal, 1.0);
}