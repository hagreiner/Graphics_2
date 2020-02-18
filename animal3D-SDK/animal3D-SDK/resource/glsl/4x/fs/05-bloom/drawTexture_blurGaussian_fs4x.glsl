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

uniform vec2 uSize; //pixel size
uniform vec2 uAxis; //axis 

vec4 blurGaussianTwo(in sampler2D image, in vec2 center, in vec2 direction){
	vec4 c = vec4(0.0);
	c += texture(image, center) * 2.0;
	c += texture(image, center + direction);
	c += texture(image, center - direction);
	return (c/4.0);
}

vec4 blurGaussianVersionTwo(in sampler2D image, in vec2 center, in vec2 direction, in float blurNum){	
	//https://community.khronos.org/t/dinamic-gaussian-kernel/53011
	int loopNum = int(blurNum);
	float doublePI = 2 * 3.14;

	vec4 c = texture(image, center);

	for(int i = 1; i <= loopNum; i++)
	{
		float blurCoeff = exp(-float(i * i)/float(2 * blurNum * blurNum));

		c += (texture(image, center - float(i) * direction)) * blurCoeff;
		c += (texture(image, center + float(i) * direction)) * blurCoeff;
	}

	c *= 1/(sqrt(doublePI)*blurNum*0.8);

	return c;
}

void main()
{
	// DUMMY OUTPUT: all fragments are OPAQUE CYAN
	//vec4 color = blurGaussianTwo(uImage00, vTexCoord, uAxis);
	//rtFragColor = vec4(color.rgb, 1.0);

	float blurAmount = 2.0;

	rtFragColor = blurGaussianVersionTwo(uImage00,vTexCoord, uSize, blurAmount);
}
