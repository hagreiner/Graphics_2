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
	
	drawTexture_outline_fs4x.glsl
	Draw texture sample with outlines.
*/

#version 410

// ****TO-DO: 
//	0) copy existing texturing shader
//	1) implement outline algorithm - see render code for uniform hints

out vec4 rtFragColor;

uniform sampler2D uTex_dm; //0.1
in vec2 vTexCoord[9]; //0.2

float offset = 1.0 / 128.0;
vec2 offsetVec = vec2(0.5, 0.5);
vec4  outLineColor = vec4(1.0, 1.0, 1.0, 1.0);
uniform vec2 localOffset[9];

void main(void)
{
	vec4 sampleMin[9];
	vec4 minValue = vec4(1.0);
	
	for (int i = 0; i < 9; i++){
		sampleMin[i] = texture(uTex_dm, vec2(vTexCoord[0].st + localOffset[i]));
		minValue = min(sampleMin[i], minValue);
	}
	rtFragColor = minValue;
}

//https://gist.github.com/xoppa/33589b7d5805205f8f08