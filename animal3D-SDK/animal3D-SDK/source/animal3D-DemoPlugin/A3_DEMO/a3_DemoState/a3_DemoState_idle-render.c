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
	
	a3_DemoState_idle-render.c/.cpp
	Demo state function implementations.

	****************************************************
	*** THIS IS ONE OF YOUR DEMO'S MAIN SOURCE FILES ***
	*** Implement your demo logic pertaining to      ***
	***     RENDERING THE STATS in this file.        ***
	****************************************************
*/

//-----------------------------------------------------------------------------

#include "../a3_DemoState.h"

#include "../_a3_demo_utilities/a3_DemoRenderUtils.h"


// OpenGL
#ifdef _WIN32
#include <gl/glew.h>
#include <Windows.h>
#include <GL/GL.h>
#else	// !_WIN32
#include <OpenGL/gl3.h>
#endif	// _WIN32


//-----------------------------------------------------------------------------
// RENDER TEXT

void a3shading_render_controls(a3_DemoState const* demoState, a3_Demo_Shading const* demoMode,
	a3f32 const textAlign, a3f32 const textDepth, a3f32 const textOffsetDelta, a3f32 textOffset);
void a3pipelines_render_controls(a3_DemoState const* demoState, a3_Demo_Pipelines const* demoMode,
	a3f32 const textAlign, a3f32 const textDepth, a3f32 const textOffsetDelta, a3f32 textOffset);
void a3curves_render_controls(a3_DemoState const* demoState, a3_Demo_Curves const* demoMode,
	a3f32 const textAlign, a3f32 const textDepth, a3f32 const textOffsetDelta, a3f32 textOffset);


// display current mode controls
void a3demo_render_controls(a3_DemoState const* demoState,
	a3f32 const textAlign, a3f32 const textDepth, a3f32 const textOffsetDelta, a3f32 textOffset)
{
	// display mode info
	a3byte const* modeText[demoState_mode_max] = {
		"LIGHTING & SHADING",
		"LIGHTING PIPELINES",
		"CURVES & INTERPOLATION",
	};

	// text color
	a3vec4 const col = { a3real_half, a3real_zero, a3real_half, a3real_one };

	// demo mode
	a3_DemoState_ModeName const demoMode = demoState->demoMode;

	// demo mode
	//a3textDraw(demoState->text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
	//	"Demo mode (%u / %u) ('</,' prev | next '>/.'): %s", demoMode + 1, demoState_mode_max, modeText[demoMode]);
	
	//midterm
	a3textDraw(demoState->text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"'+' and '-' to zoom in and out", demoMode + 1, demoState_mode_max, modeText[demoMode]);
	a3textDraw(demoState->text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"WASD to move", demoMode + 1, demoState_mode_max, modeText[demoMode]);
	a3textDraw(demoState->text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"!@#$ to tiny move", demoMode + 1, demoState_mode_max, modeText[demoMode]);
	a3textDraw(demoState->text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"'(' and ')' to change scenes", demoMode + 1, demoState_mode_max, modeText[demoMode]);

	// draw controls for specific modes
	switch (demoMode)
	{
	case demoState_shading:
		a3shading_render_controls(demoState, demoState->demoMode_shading, textAlign, textDepth, textOffsetDelta, textOffset);
		break;
	case demoState_pipelines:
		a3pipelines_render_controls(demoState, demoState->demoMode_pipelines, textAlign, textDepth, textOffsetDelta, textOffset);
		break;
	case demoState_curves:
		a3curves_render_controls(demoState, demoState->demoMode_curves, textAlign, textDepth, textOffsetDelta, textOffset);
		break;
	}

	// global controls
	//midterm changes
	textOffset = -0.8f;
	//a3textDraw(demoState->text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
	//	"Toggle text display:        't' (toggle) | 'T' (alloc/dealloc) ");
	//a3textDraw(demoState->text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
	//	"Reload all shader programs: 'P' ****CHECK CONSOLE FOR ERRORS!**** ");
	a3textDraw(demoState->text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"Press 'o' and 'b' to switch colors");
}


// display general controls
void a3demo_render_controls_gen(a3_DemoState const* demoState,
	a3f32 const textAlign, a3f32 const textDepth, a3f32 const textOffsetDelta, a3f32 textOffset)
{
	// boolean text
	a3byte const boolText[2][4] = {
		"OFF",
		"ON ",
	};

	// text color
	a3vec4 const col = { a3real_half, a3real_zero, a3real_half, a3real_one };

	// toggles
	a3textDraw(demoState->text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"GRID (toggle 'g') %s | SKYBOX ('b') %s | HIDDEN VOLUMES ('h') %s", boolText[demoState->displayGrid], boolText[demoState->displaySkybox], boolText[demoState->displayHiddenVolumes]);
	a3textDraw(demoState->text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"WORLD AXES (toggle 'x') %s | OBJECT AXES ('z') %s", boolText[demoState->displayWorldAxes], boolText[demoState->displayObjectAxes]);
	a3textDraw(demoState->text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"TANGENT BASES ('B') %s | WIREFRAME ('F') %s", boolText[demoState->displayTangentBases], boolText[demoState->displayWireframe]);
	a3textDraw(demoState->text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"ANIMATION (toggle 'm') %s", boolText[demoState->updateAnimation]);
	a3textDraw(demoState->text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"STENCIL TEST (toggle 'i') %s", boolText[demoState->stencilTest]);
	a3textDraw(demoState->text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"SKIP INTERMEDIATE PASSES (toggle 'I') %s", boolText[demoState->skipIntermediatePasses]);
	a3textDraw(demoState->text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"    Forward point light count ('l' decr | incr 'L'): %u / %u", demoState->forwardLightCount, demoStateMaxCount_lightObject);
	a3textDraw(demoState->text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"    Deferred point light count (';' decr | incr ':'): %u / %u", demoState->deferredLightCount, demoStateMaxCount_lightVolume);

	// global controls
	textOffset = -0.8f;
	a3textDraw(demoState->text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"Toggle text display:        't' (toggle) | 'T' (alloc/dealloc) ");
	a3textDraw(demoState->text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"Reload all shader programs: 'P' ****CHECK CONSOLE FOR ERRORS!**** ");

	// input-dependent controls
	textOffset = -0.6f;
	if (a3XboxControlIsConnected(demoState->xcontrol))
	{
		a3textDraw(demoState->text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
			"Xbox controller camera control: ");
		a3textDraw(demoState->text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
			"    Left joystick = rotate | Right joystick, triggers = move");
	}
	else
	{
		a3textDraw(demoState->text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
			"Keyboard/mouse camera control: ");
		a3textDraw(demoState->text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
			"    Left click & drag = rotate | WASDEQ = move | wheel = zoom");
	}
}


// scene data (HUD)
void a3demo_render_data(const a3_DemoState* demoState,
	a3f32 const textAlign, a3f32 const textDepth, a3f32 const textOffsetDelta, a3f32 textOffset)
{
	// text color
	const a3vec4 col = { a3real_half, a3real_zero, a3real_half, a3real_one };

	// display some general data
	a3textDraw(demoState->text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"t_render = %+.4lf ", demoState->renderTimer->totalTime);
	a3textDraw(demoState->text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"dt_render = %.4lf ", demoState->renderTimer->previousTick);
	a3textDraw(demoState->text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"fps_actual = %.4lf ", __a3recipF64(demoState->renderTimer->previousTick));
	a3textDraw(demoState->text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"fps_target = %.4lf ", (a3f64)demoState->renderTimer->ticks / demoState->renderTimer->totalTime);

	// global controls
	textOffset = -0.8f;
	a3textDraw(demoState->text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"Toggle text display:        't' (toggle) | 'T' (alloc/dealloc) ");
	a3textDraw(demoState->text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"Reload all shader programs: 'P' ****CHECK CONSOLE FOR ERRORS!**** ");
}


//-----------------------------------------------------------------------------
// RENDER

void a3shading_render(a3_DemoState const* demoState, a3_Demo_Shading const* demoMode);
void a3pipelines_render(a3_DemoState const* demoState, a3_Demo_Pipelines const* demoMode);
void a3curves_render(a3_DemoState const* demoState, a3_Demo_Curves const* demoMode);

void a3demo_render(a3_DemoState const* demoState)
{
	// display mode for current pipeline
	// ensures we don't go through the whole pipeline if not needed
	a3_DemoState_ModeName const demoMode = demoState->demoMode;


	// amount to offset text as each line is rendered
	a3f32 const textAlign = -0.98f;
	a3f32 const textDepth = -1.00f;
	a3f32 const textOffsetDelta = -0.08f;
	a3f32 textOffset = +1.00f;


	// choose render sub-routine for the current mode
	switch (demoMode)
	{
	case demoState_shading:
		a3shading_render(demoState, demoState->demoMode_shading);
		break;
	case demoState_pipelines:
		a3pipelines_render(demoState, demoState->demoMode_pipelines);
		break;
	case demoState_curves:
		a3curves_render(demoState, demoState->demoMode_curves);
		break;
	}


	// deactivate things
	a3vertexDrawableDeactivate();
	a3shaderProgramDeactivate();
	a3framebufferDeactivateSetViewport(a3fbo_depthDisable, 0, 0, demoState->windowWidth, demoState->windowHeight);
	a3textureDeactivate(a3tex_unit00);


	// text
	if (demoState->textInit)
	{
		// choose text render mode
		switch (demoState->textMode)
		{
			// controls for current mode
		case demoState_textControls:
			a3demo_render_controls(demoState, textAlign, textDepth, textOffsetDelta, textOffset);
			break;

			// controls for general
		case demoState_textControls_gen:
			a3demo_render_controls_gen(demoState, textAlign, textDepth, textOffsetDelta, textOffset);
			break;

			// general data
		case demoState_textData:
			a3demo_render_data(demoState, textAlign, textDepth, textOffsetDelta, textOffset);
			break;
		}
	}
}


//-----------------------------------------------------------------------------
