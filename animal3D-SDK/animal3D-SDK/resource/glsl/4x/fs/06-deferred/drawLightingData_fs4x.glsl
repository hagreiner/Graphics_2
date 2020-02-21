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
	
	drawLightingData_fs4x.glsl
	Draw attribute data received from vertex shader directly to targets.
*/

#version 410

// ****TO-DO: 
//	1) declare varyings to receive attribute data
//	2) declare render targets to display attribute data
//	3) copy attribute data from varying to respective render targets, 
//		transforming data accordingly

layout (location = 1) out vec4 rtViewPosition;
layout (location = 2) out vec4 rtViewNormal;
layout (location = 3) out vec4 rtAtlasTexcoord;

in vbLightingData {
	vec4 vViewPosition;
	vec4 vViewNormal;
	vec4 vTexcoord;
	vec4 vBiasedClipCoord;
};

void main()
{
	// DUMMY OUTPUT: all fragments are OPAQUE RED, GREEN AND BLUE
	rtViewPosition = vViewPosition / vViewPosition.w; //perpective div (see shadows lab)

	vec4 biasEdits = vBiasedClipCoord / vBiasedClipCoord.w;
	rtViewNormal = normalize(vViewNormal) * 2 * vViewPosition.w; // not correct
	
	rtAtlasTexcoord = vTexcoord;
}
