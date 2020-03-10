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

	a3_Demo_Curves_initialize.c
	Demo mode implementations: curves & interpolation initialization.

	********************************************
	*** INITIALIZATION FOR CURVE INTERP MODE ***
	********************************************
*/

//-----------------------------------------------------------------------------

#include "../a3_Demo_Curves.h"

typedef struct a3_DemoState a3_DemoState;
//#include "../a3_DemoState.h"


//-----------------------------------------------------------------------------

void a3curves_init(a3_DemoState const* demoState, a3_Demo_Curves* demoMode)
{
	demoMode->render = curves_renderPhong;
	demoMode->display = curves_displayTexture;
	demoMode->activeCamera = curves_cameraSceneViewer;

	demoMode->pipeline = curves_forward;
	demoMode->pass = curves_passComposite;

	demoMode->targetIndex[curves_passShadow] = curves_shadow_fragdepth;
	demoMode->targetIndex[curves_passScene] = curves_scene_finalcolor;
	demoMode->targetIndex[curves_passComposite] = curves_composite_finalcolor;
	demoMode->targetIndex[curves_passBright_2] = curves_bright_finalcolor;
	demoMode->targetIndex[curves_passBlend] = curves_display_finalcolor;

	demoMode->targetCount[curves_passShadow] = curves_target_shadow_max;
	demoMode->targetCount[curves_passScene] = curves_target_scene_max;
	demoMode->targetCount[curves_passComposite] = curves_target_composite_max;
	demoMode->targetCount[curves_passBright_2] = curves_target_bright_max;
	demoMode->targetCount[curves_passBlend] = curves_target_display_max;

	demoMode->interp = curves_interpNone;
}


//-----------------------------------------------------------------------------
