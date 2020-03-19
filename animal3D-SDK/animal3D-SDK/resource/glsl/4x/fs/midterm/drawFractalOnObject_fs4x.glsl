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

layout (location = 0) out vec4 rtFragColor;

uniform sampler2D uTex_dm;
uniform sampler2D uTex_sm;

const int size = 12;
uniform vec4 uLightPos[size];
uniform int uLightCt;
uniform float uLightSz[size];
uniform float uLightSzInvSq[size];
uniform vec4 uLightCol[size];

in vec4 vNormal;
in vec2 vTexCoord;
in vec4 vViewPosition;

//custom
uniform double uTime;
uniform vec2 u2DPosition;
uniform vec3 uColorFractal1;
uniform vec3 uColorFractal2;
uniform vec3 uColorFractal3;
uniform vec3 uColorFractal4;
uniform vec2 uZoom;

vec3 fractalObject(){    
	const int iterations = 50;
	float zoom = 100;

    float time = pow(2, float(uZoom.x)/2.0);
    zoom /= time;
    float xOffset = u2DPosition.x * (time * 0.1) + 0.5;
    float yOffset = u2DPosition.y * (time * 0.1) + 0.5;

    float realTemp  = (vTexCoord.x - xOffset) * zoom; 
    float imagTemp  = (vTexCoord.y - yOffset) * zoom; 
    float RealFloat = realTemp;
    float ImaginaryFloat = imagTemp;

    float combinedFloat = 0.0;
    int index;

    for (index = 0; index < iterations && combinedFloat < 4.0; ++index) {
        float altReal = realTemp;
        
        realTemp = (altReal * altReal) - (imagTemp * imagTemp) + RealFloat;
        imagTemp = 2.0 * (altReal * imagTemp) + ImaginaryFloat;

        combinedFloat = (realTemp * realTemp) + (imagTemp * imagTemp);
    }

    // color apply
    vec3 color;
    if (combinedFloat < 3.0) { color = mix(uColorFractal1, uColorFractal2, fract(float(index)*0.1)); }

    else { 
    color = mix(uColorFractal3, uColorFractal4, fract(float(index)*0.1)); 
    }

    color = clamp(color, 0.0, 1.0);
    return color;
}

void main()
{
	//vec4 diffuseMap = texture(uTex_dm, vTexCoord);
	vec4 diffuseMap = vec4(fractalObject(), 1.0);
	vec4 specMap = vec4(fractalObject(), 1.0);

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
		vec3 V = normalize(-vViewPosition.xyz);
		float lightDistance = length(uLightPos[index] - vViewPosition);

		diffuseCoef = max(dot(N, L), 0);
		specularCoef = pow(max(dot(R, V), 0), 30);

		attenuation = 1 / (1 + uLightSzInvSq[index] * (lightDistance * lightDistance));
		
		diffuseTotal += diffuseCoef * uLightCol[index].xyz;
		specTotal += specularCoef * uLightCol[index].xyz;

		color += attenuation * ((diffuseCoef * diffuseMap.xyz * uLightCol[index].xyz * 0.7) + 
		(specularCoef * specMap.xyz * uLightCol[index].xyz * 0.7));
	}
	rtFragColor = vec4(color, 1.0);
}