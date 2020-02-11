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

float offset = 1.0 / 128.0;
vec4  outLineColor = vec4(1.0, 1.0, 1.0, 1.0);

void main()
{
	vec4 col = texture(uTex_dm, vTexCoord);
	float a = texture(uTex_dm, vec2(vTexCoord.x + offset, vTexCoord.y)).a +
			texture(uTex_dm, vec2(vTexCoord.x, vTexCoord.y - offset)).a +
			texture(uTex_dm, vec2(vTexCoord.x - offset, vTexCoord.y)).a +
			texture(uTex_dm, vec2(vTexCoord.x, vTexCoord.y + offset)).a;
	col *= outLineColor;
	rtFragColor = col;
}

//https://gist.github.com/xoppa/33589b7d5805205f8f08