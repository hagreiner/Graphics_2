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
in vec2 vTexCoord; //0.2
layout (location = 2) in vec4 aNormals;
uniform mat4 uMV_nrm;

vec4 outlineColor = vec4(1.0, 1.0, 0.0, 1.0);
float outlineSize = 0.8;

void main(void)
{
	//float a = texture(uTex_dm, vTexCoord + (outlineSize, 0)).a;
	//float b = texture(uTex_dm, vTexCoord - (outlineSize, 0)).a;
	//float c = texture(uTex_dm, vTexCoord - (0, outlineSize)).a;
	//float d = texture(uTex_dm, vTexCoord + (0, outlineSize)).a;
	//
	//float result = a + b + c + d;

	//rtFragColor = texture(uTex_dm, vTexCoord);


	vec4 col = texture2D(uTex_dm, vTexCoord);
	if (col.a > 0.5)
		rtFragColor = col;
	else {
		float a = texture2D(uTex_dm, vec2(vTexCoord.x + outlineSize, vTexCoord.y)).a +
			texture(uTex_dm, vec2(vTexCoord.x, vTexCoord.y - outlineSize)).a +
			texture(uTex_dm, vec2(vTexCoord.x - outlineSize, vTexCoord.y)).a +
			texture(uTex_dm, vec2(vTexCoord.x, vTexCoord.y + outlineSize)).a;
		if (col.a < 1.0 && a > 0.0)
			rtFragColor = vec4(1.0, 0.0, 0.0, 1.0);
		else
			rtFragColor = col;
	}
}

//https://gist.github.com/xoppa/33589b7d5805205f8f08