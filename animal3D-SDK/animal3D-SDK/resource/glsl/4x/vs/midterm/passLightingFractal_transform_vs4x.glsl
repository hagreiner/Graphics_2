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
	
	passTexcoord_transform_vs4x.glsl
	Vertex shader that passes texture coordinate. Outputs transformed position 
		attribute and atlas transformed texture coordinate attribute.
*/

#version 410

layout (location = 0) in vec4 aPosition;
uniform mat4 uMV;
out vec4 vViewPosition;

uniform mat4 uP;

layout (location = 2) in vec4 aNormals;
uniform mat4 uMV_nrm;
out vec4 vNormal;

layout (location = 8) in vec2 aTexCoord;
out vec2 vTexCoord;

void main()
{
	vViewPosition = uMV * aPosition;
	gl_Position = uP * vViewPosition;

	vNormal =  uMV_nrm * aNormals;
	vTexCoord = aTexCoord; 
}
