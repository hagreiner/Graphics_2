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
	
	drawLambert_multi_fs4x.glsl
	Draw Lambert shading model for multiple lights.
*/

#version 410

// ****TO-DO: 
//	1) declare uniform variable for texture; see demo code for hints
//	2) declare uniform variables for lights; see demo code for hints
//	3) declare inbound varying data
//	4) implement Lambert shading model
//	Note: test all data and inbound values before using them!

out vec4 rtFragColor;
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
	vec4 TextureSample = texture(uTex_dm, vTexCoord);
	//rtFragColor = uLightPos[1];

	// - find lighting num (Lambert= N * L)
	//vec3 N = normalize(vNormal);
	//vec3 L = normalize(lightPos–passPosition);
	//float perFragShading= MY_DIFFUSE_FUNC (N, L)
	float lambertCoef = 0.0;
	vec3 color = vec3(0.0, 0.0, 0.0);
	// https://www.tomdalling.com/blog/modern-opengl/07-more-lighting-ambient-specular-attenuation-gamma/
	float attenuation = 0.0;

	for (int index=0; index < size && index < uLightCt; index++){
		vec3 N = normalize(vNormal).xyz;
		vec3 L = normalize(uLightPos[index] - vViewPosition).xyz;
		float lightDistance = length(uLightPos[index] - vViewPosition);
		lambertCoef = max(dot(N, L), 0);
		attenuation = 1 / (1 + uLightSzInvSq[index] * (lightDistance * lightDistance));
		color += attenuation * (lambertCoef * TextureSample.xyz * uLightCol[index].xyz);
	}

	// - apply lighting num to color
	rtFragColor = vec4(color, 1.0);
}
