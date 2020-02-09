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
	
	passLightingData_shadowCoord_transform_vs4x.glsl
	Vertex shader that prepares and passes lighting data. Outputs transformed 
		position attribute and all others required for lighting. Also computes 
		and passes shadow coordinate.
*/

#version 410

// ****TO-DO: 
//	0) copy previous lighting data vertex shader
//	1) declare MVPB matrix for light
//	2) declare varying for shadow coordinate
//	3) calculate and pass shadow coordinate

layout (location = 0) in vec4 aPosition;
uniform mat4 uMV; //0.1
out vec4 vViewPosition; //0.2

uniform mat4 uP; //0.4

layout (location = 2) in vec4 aNormals; //0.6
uniform mat4 uMV_nrm; //0.7
out vec4 vNormal; //0.8

layout (location = 8) in vec2 aTexCoord; //0.10
out vec2 vTexCoord; //0.10

uniform mat4 uMVPB; //1
out vec4 vShadowCoord; //2

void main()
{
	// DUMMY OUTPUT: directly assign input position to output position
	//gl_Position = aPosition;
	vViewPosition = uMV * aPosition; //0.3
	gl_Position = uP * vViewPosition; //0.5

	vNormal =  uMV_nrm * aNormals; //0.9
	vTexCoord = aTexCoord; //0.10

	vShadowCoord = uMVPB * vNormal;
}
