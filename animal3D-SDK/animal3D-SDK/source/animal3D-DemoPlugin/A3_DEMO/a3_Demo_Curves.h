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

	a3_Demo_Curves.h
	Demo mode interface: curves & interpolation.

	********************************************
	*** THIS IS ONE DEMO MODE'S HEADER FILE  ***
	********************************************
*/

#ifndef __ANIMAL3D_DEMO_CURVES_H
#define __ANIMAL3D_DEMO_CURVES_H


//-----------------------------------------------------------------------------

#include "animal3D/animal3D.h"


//-----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#else	// !__cplusplus
	typedef struct a3_Demo_Curves						a3_Demo_Curves;
	typedef enum a3_Demo_Curves_RenderProgramName		a3_Demo_Curves_RenderProgramName;
	typedef enum a3_Demo_Curves_DisplayProgramName		a3_Demo_Curves_DisplayProgramName;
	typedef enum a3_Demo_Curves_ActiveCameraName		a3_Demo_Curves_ActiveCameraName;
	typedef enum a3_Demo_Curves_PipelineName			a3_Demo_Curves_PipelineName;
	typedef enum a3_Demo_Curves_PassName				a3_Demo_Curves_PassName;
	typedef enum a3_Demo_Curves_TargetName				a3_Demo_Curves_TargetName;
	typedef enum a3_Demo_Curves_InterpolationModeName	a3_Demo_Curves_InterpolationModeName;
#endif	// __cplusplus


//-----------------------------------------------------------------------------

	// scene object rendering program names
	enum a3_Demo_Curves_RenderProgramName
	{
		curves_renderPhong,			// Phong shading

		curves_render_max
	};

	// final display modes
	enum a3_Demo_Curves_DisplayProgramName
	{
		curves_displayTexture,		// display simple texture
		curves_displayOutline,		// display texture with outlines

		curves_display_max
	};

	// active camera names
	enum a3_Demo_Curves_ActiveCameraName
	{
		curves_cameraSceneViewer,	// scene viewing camera
		curves_cameraShadowLight,	// shadow map capturing light

		curves_camera_max
	};


	// pipeline names
	enum a3_Demo_Curves_PipelineName
	{
		curves_forward,				// forward lighting pipeline

		curves_pipeline_max
	};

	// render passes
	enum a3_Demo_Curves_PassName
	{
		curves_passShadow,			// capture shadow map

		curves_passScene,			// render scene objects

		curves_passComposite,		// composite for post-processing stage

		curves_passBright_2,		// horizontal blur for bloom (eighth screen size)
		curves_passBlend,			// bloom composite pass

		curves_pass_max
	};

	// render target names
	enum a3_Demo_Curves_TargetName
	{
		curves_shadow_fragdepth = 0,		// fragment depth
		curves_target_shadow_max, 

		curves_scene_finalcolor = 0,		// final display color
		curves_scene_position,				// position attribute
		curves_scene_normal,				// normal attribute
		curves_scene_texcoord,				// texcoord attribute
		curves_scene_shadowcoord,			// shadow coordinate
		curves_scene_shadowtest,			// result of shadow test
		curves_scene_diffuseLight,			// diffuse light total
		curves_scene_specularLight,			// specular light total
		curves_scene_fragdepth,				// fragment depth
		curves_target_scene_max, 

		curves_composite_finalcolor = 0,	// final display color
		curves_target_composite_max,

		curves_bright_finalcolor = 0,		// final display color
		curves_bright_luminance,			// luminance of color
		curves_target_bright_max,

		curves_blur_finalcolor = 0,			// final display color
		curves_target_blur_max,

		curves_display_finalcolor = 0,		// final display color
		curves_target_display_max,
	};


	// interpolation mode
	enum a3_Demo_Curves_InterpolationModeName
	{
		curves_interpNone,
		curves_interpLerp,
		curves_interpBezier,
		curves_interpCatmullRom,
		curves_interpCubicHermite,

		curves_interp_max
	};


//-----------------------------------------------------------------------------

	// demo mode for basic shading
	struct a3_Demo_Curves
	{
		a3_Demo_Curves_RenderProgramName render;
		a3_Demo_Curves_DisplayProgramName display;
		a3_Demo_Curves_ActiveCameraName activeCamera;

		a3_Demo_Curves_PipelineName pipeline;
		a3_Demo_Curves_PassName pass;
		a3_Demo_Curves_TargetName targetIndex[curves_pass_max], targetCount[curves_pass_max];

		a3_Demo_Curves_InterpolationModeName interp;
	};


//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif	// __cplusplus


#endif	// !__ANIMAL3D_DEMO_CURVES_H