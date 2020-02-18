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
	
	drawTexture_blendScreen4_fs4x.glsl
	Draw blended sample from multiple textures using screen function.
*/

#version 410

// ****TO-DO: 
//	0) copy existing texturing shader
//	1) declare additional texture uniforms
//	2) implement screen function with 4 inputs
//	3) use screen function to sample input textures

uniform sampler2D uImage00;
uniform sampler2D uImage01;
uniform sampler2D uImage02;
uniform sampler2D uImage03;
in vec2 vTexCoord;

layout (location = 0) out vec4 rtFragColor;

vec4 inverse(in vec4 inverted){
	inverted[0] = 1 - inverted[0];
	inverted[1] = 1 - inverted[1];
	inverted[2] = 1 - inverted[2];
	inverted[3] = 1 - inverted[3];

	return inverted;
}

void main()
{
	// DUMMY OUTPUT: all fragments are OPAQUE YELLOW
	vec4 input_0 = texture(uImage00, vTexCoord);
	vec4 input_1 = texture(uImage01, vTexCoord);
	vec4 input_2 = texture(uImage02, vTexCoord);
	vec4 input_3 = texture(uImage03, vTexCoord);

	vec4 tempResult = inverse(input_0) * inverse(input_1) * inverse(input_2) * inverse(input_3);

	//rtFragColor = vec4(inverse(tempResult).rgb, 1.0);
	rtFragColor = inverse(tempResult);
}
