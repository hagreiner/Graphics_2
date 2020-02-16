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
	
	drawTexture_blurGaussian_fs4x.glsl
	Draw texture with Gaussian blurring.
*/

#version 410

// ****TO-DO: 
//	0) copy existing texturing shader
//	1) declare uniforms for pixel size and sampling axis
//	2) implement Gaussian blur function using a 1D kernel (hint: Pascal's triangle)
//	3) sample texture using Gaussian blur function and output result

layout (location = 0) out vec4 rtFragColor;
in vec2 vTexCoord;
uniform sampler2D uImage00;

uniform vec2 direction; //pixel size
uniform vec2 a3vec2_x; //axis x
uniform vec2 a3vec2_y; //axis y

vec4 blurGaussianTwo(sampler2D image, vec2 center, vec2 direction){
	vec4 c = vec4(0.0);
	c += texture(image, center) * 2.0;
	c += texture(image, center + direction);
	c += texture(image, center - direction);
	return (c/4.0);
}

void main()
{
	// DUMMY OUTPUT: all fragments are OPAQUE CYAN
	vec4 color = blurGaussianTwo(uImage00, vTexCoord, direction);
	
	rtFragColor = color;
}
