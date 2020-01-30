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


// OpenGL
#ifdef _WIN32
#include <gl/glew.h>
#include <Windows.h>
#include <GL/GL.h>
#else	// !_WIN32
#include <OpenGL/gl3.h>
#endif	// _WIN32


//-----------------------------------------------------------------------------
// SETUP UTILITIES

// blending state for composition
inline void a3demo_enableCompositeBlending()
{
	// result = ( new*[new alpha] ) + ( old*[1 - new alpha] )
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

// default stencil state for writing
inline void a3demo_enableStencilWrite()
{
//	glStencilFunc(...);			// any stencil value will be set to 1
//	glStencilOp(...);			// replace stencil value if S&D tests pass
//	glStencilMask(...);			// write to stencil buffer
}

// default stencil state for comparing
inline void a3demo_enableStencilCompare()
{
//	glStencilFunc(...);			// stencil test passes if equal to 1
//	glStencilOp(...);			// keep stencil value if S&D tests pass
//	glStencilMask(...);			// don't write to stencil buffer
}


// set default GL state
void a3demo_setDefaultGraphicsState()
{
	const a3f32 lineWidth = 2.0f;
	const a3f32 pointSize = 4.0f;

	// lines and points
	glLineWidth(lineWidth);
	glPointSize(pointSize);

	// backface culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// textures
	glEnable(GL_TEXTURE_2D);

	// background
	glClearColor(0.0f, 0.0f, 0.0, 0.0f);

	// alpha blending
	a3demo_enableCompositeBlending();
}


//-----------------------------------------------------------------------------
// GENERAL UTILITIES

inline a3real4x4r a3demo_quickInverseTranspose_internal(a3real4x4p m_out, const a3real4x4p m_in)
{
	// the basis for this calculation is "inverse transpose" which 
	//	will result in the scale component inverting while rotation 
	//	stays the same
	// expanding the formula using fundamendal matrix identities 
	//	yields this optimized version
	// translation column is untouched
	a3real4ProductS(m_out[0], m_in[0], a3real3LengthSquaredInverse(m_in[0]));
	a3real4ProductS(m_out[1], m_in[1], a3real3LengthSquaredInverse(m_in[1]));
	a3real4ProductS(m_out[2], m_in[2], a3real3LengthSquaredInverse(m_in[2]));
	a3real4SetReal4(m_out[3], m_in[3]);
	return m_out;
}

inline a3real4x4r a3demo_quickInvertTranspose_internal(a3real4x4p m_inout)
{
	a3real4MulS(m_inout[0], a3real3LengthSquaredInverse(m_inout[0]));
	a3real4MulS(m_inout[1], a3real3LengthSquaredInverse(m_inout[1]));
	a3real4MulS(m_inout[2], a3real3LengthSquaredInverse(m_inout[2]));
	return m_inout;
}

inline a3real4x4r a3demo_quickTransposedZeroBottomRow(a3real4x4p m_out, const a3real4x4p m_in)
{
	a3real4x4GetTransposed(m_out, m_in);
	m_out[0][3] = m_out[1][3] = m_out[2][3] = a3real_zero;
	return m_out;
}


//-----------------------------------------------------------------------------
// RENDER TEXT

// controls for main render pipeline
void a3demo_render_main_controls(const a3_DemoState *demoState,
	const a3ui32 demoSubMode, const a3ui32 demoOutput,
	const a3ui32 demoSubModeCount, const a3ui32 demoOutputCount)
{
	// display mode info
	const a3byte *modeText = "Texturing and Shading";
	const a3byte *subModeText[demoStateMaxSubModes] = {
		"Shading",
		"Shading with MRT",
	};
	const a3byte *outputText[demoStateMaxSubModes][demoStateMaxOutputModes] = {
		{
			"Color buffer",
		},
		{
			"Color target 0: FINAL SCENE COLOR",
			"Color target 1: Attrib data: view position",
			"Color target 2: Attrib data: view normal",
			"Color target 3: Attrib data: atlas texcoord",
			"Color target 4: Texture: diffuse map",
			"Color target 5: Texture: specular map",
			"Color target 6: Lighting: diffuse total",
			"Color target 7: Lighting: specular total",
			"Depth buffer",
		},
	};

	// forward pipeline names
	const a3byte* forwardShadingName[] = {
		"Solid color",
		"Texturing",
		"Lambert shading",
		"Phong shading",
		"Nonphotorealistic shading",
	};

	// forward display names
	const a3byte* forwardDisplayName[] = {
		"Texture",
		"Color manipulation",
		"Texcoord manipulation",
	};

	// text color
	const a3vec4 col = { a3real_one, a3real_zero, a3real_one, a3real_one };

	// amount to offset text as each line is rendered
	a3f32 textAlign = -0.98f;
	a3f32 textOffset = 1.00f;
	a3f32 textDepth = -1.00f;
	const a3f32 textOffsetDelta = -0.08f;

	// demo modes
	a3textDraw(demoState->text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"Demo mode (%u / %u) (',' prev | next '.'): %s", demoState->demoMode + 1, demoState->demoModeCount, modeText);
	a3textDraw(demoState->text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"    Sub-mode (%u / %u) ('<' | '>'): %s", demoSubMode + 1, demoSubModeCount, subModeText[demoSubMode]);
	a3textDraw(demoState->text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"        Output (%u / %u) ('{' | '}'): %s", demoOutput + 1, demoOutputCount, outputText[demoSubMode][demoOutput]);

	// lighting modes
	switch (demoState->lightingPipelineMode)
	{
	case demoStatePipelineMode_forward:
		a3textDraw(demoState->text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
			"    Light count (+/- 'L'/'l'): %u / %u", demoState->forwardLightCount, demoStateMaxCount_lightObject);
		a3textDraw(demoState->text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
			"    Forward shading mode (%u / %u) ('j' | 'k'): %s",
			demoState->forwardShadingMode + 1, demoState->forwardShadingModeCount, forwardShadingName[demoState->forwardShadingMode]);
		if (demoSubMode == demoStateSubMode_main_mrt)
			a3textDraw(demoState->text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
				"    Forward display mode (%u / %u) ('J' | 'K'): %s",
				demoState->forwardDisplayMode + 1, demoState->forwardDisplayModeCount, forwardDisplayName[demoState->forwardDisplayMode]);
		else
			textOffset += textOffsetDelta;
		break;
	default:
		textOffset += textOffsetDelta;
		break;
	}

	// toggles
	a3textDraw(demoState->text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"ACTIVE CAMERA ('c' | 'v'): %d / %d", demoState->activeCamera + 1, demoStateMaxCount_cameraObject);
	a3textDraw(demoState->text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"GRID in scene (toggle 'g') %d | SKYBOX backdrop ('b') %d", demoState->displayGrid, demoState->displaySkybox);
	a3textDraw(demoState->text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"WORLD AXES (toggle 'x') %d | OBJECT AXES ('z') %d | TANGENT BASES ('B') %d", demoState->displayWorldAxes, demoState->displayObjectAxes, demoState->displayTangentBases);
	a3textDraw(demoState->text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"HIDDEN VOLUMES (toggle 'h') %d ", demoState->displayHiddenVolumes);
	a3textDraw(demoState->text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"ANIMATION updates (toggle 'm') %d", demoState->updateAnimation);
	a3textDraw(demoState->text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"STENCIL TEST (toggle 'i') %d", demoState->stencilTest);


	//  move down
	textOffset = -0.5f;

	// display controls
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

	// global controls
	a3textDraw(demoState->text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"Toggle text display:        't' (toggle) | 'T' (alloc/dealloc) ");
	a3textDraw(demoState->text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"Reload all shader programs: 'P' ****CHECK CONSOLE FOR ERRORS!**** ");
}


// scene data (HUD)
void a3demo_render_data(const a3_DemoState* demoState)
{
	// text color
	const a3vec4 col = { a3real_one, a3real_zero, a3real_one, a3real_one };

	// amount to offset text as each line is rendered
	a3f32 textAlign = -0.98f;
	a3f32 textOffset = 1.00f;
	a3f32 textDepth = -1.00f;
	const a3f32 textOffsetDelta = -0.08f;

	// move down
	textOffset = +0.9f;

	// display some data
	a3textDraw(demoState->text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"t_render = %+.4lf ", demoState->renderTimer->totalTime);
	a3textDraw(demoState->text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"dt_render = %.4lf ", demoState->renderTimer->previousTick);
	a3textDraw(demoState->text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"fps_actual = %.4lf ", __a3recipF64(demoState->renderTimer->previousTick));
	a3textDraw(demoState->text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"fps_target = %.4lf ", (a3f64)demoState->renderTimer->ticks / demoState->renderTimer->totalTime);
}


//-----------------------------------------------------------------------------
// RENDER SUB-ROUTINES

void a3demo_drawSkybox(const a3_DemoState* demoState)
{
	const a3_DemoStateShaderProgram* currentDemoProgram = demoState->prog_drawTexture;
	const a3_DemoProjector* activeCamera = demoState->projector + demoState->activeCamera;
	a3mat4 modelViewProjectionMat;

	// set up render
	a3shaderProgramActivate(currentDemoProgram->program);
	a3real4x4Product(modelViewProjectionMat.m, activeCamera->viewProjectionMat.m, demoState->skyboxObject->modelMat.m);
	a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uMVP, 1, modelViewProjectionMat.mm);
	a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uAtlas, 1, a3mat4_identity.mm);
	a3textureActivate(demoState->tex_skybox_clouds, a3tex_unit00);

	// draw inverted box
	glCullFace(GL_FRONT);
	a3vertexDrawableActivateAndRender(demoState->draw_skybox);
	glCullFace(GL_BACK);
}

// sub-routine for rendering the demo state (main render pipeline)
void a3demo_render_main(const a3_DemoState *demoState,
	const a3ui32 demoSubMode, const a3ui32 demoOutput,
	const a3ui32 demoSubModeCount, const a3ui32 demoOutputCount)
{
	// pointers
	const a3_VertexDrawable *currentDrawable;
	const a3_DemoStateShaderProgram *currentDemoProgram;
	const a3_DemoPointLight *pointLight;

	// framebuffers
	const a3_Framebuffer* currentReadFBO, * currentWriteFBO;

	// indices
	a3ui32 i, j, k;

	// RGB
	const a3vec4 rgba4[] = {
		{ 1.0f, 0.0f, 0.0f, 1.0f },	// red
		{ 0.0f, 1.0f, 0.0f, 1.0f },	// green
		{ 0.0f, 0.0f, 1.0f, 1.0f },	// blue
		{ 0.0f, 1.0f, 1.0f, 1.0f },	// cyan
		{ 1.0f, 0.0f, 1.0f, 1.0f },	// magenta
		{ 1.0f, 1.0f, 0.0f, 1.0f },	// yellow
		{ 1.0f, 0.5f, 0.0f, 1.0f },	// orange
		{ 0.0f, 0.5f, 1.0f, 1.0f },	// sky blue
		{ 0.5f, 0.5f, 0.5f, 1.0f },	// solid grey
		{ 0.5f, 0.5f, 0.5f, 0.5f },	// translucent grey
	};
	const a3real
		*const red = rgba4[0].v, *const green = rgba4[1].v, *const blue = rgba4[2].v,
		*const cyan = rgba4[3].v, *const magenta = rgba4[4].v, *const yellow = rgba4[5].v,
		*const orange = rgba4[6].v, *const skyblue = rgba4[7].v,
		*const grey = rgba4[8].v, *const grey_t = rgba4[9].v;


	// bias matrix
	const a3mat4 bias = {
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f,
	};

	// final model matrix and full matrix stack
	a3mat4 modelViewProjectionMat = a3mat4_identity;
	a3mat4 modelViewMat = a3mat4_identity, modelMat = a3mat4_identity;

	// camera used for drawing
	const a3_DemoProjector *activeCamera = demoState->projector + demoState->activeCamera;
	const a3_DemoSceneObject *activeCameraObject = activeCamera->sceneObject;

	// current scene object being rendered, for convenience
	const a3_DemoSceneObject *currentSceneObject, *endSceneObject;

	// temp drawable pointers
	const a3_VertexDrawable* drawable[] = {
		demoState->draw_plane,
		demoState->draw_sphere,
		demoState->draw_cylinder,
		demoState->draw_torus,
		demoState->draw_teapot,
	};

	// temp texture pointers
	const a3_Texture* texture_dm[] = {
		demoState->tex_stone_dm,
		demoState->tex_earth_dm,
		demoState->tex_stone_dm,
		demoState->tex_mars_dm,
		demoState->tex_checker,
	};
	const a3_Texture* texture_sm[] = {
		demoState->tex_stone_dm,
		demoState->tex_earth_sm,
		demoState->tex_stone_dm,
		demoState->tex_mars_sm,
		demoState->tex_checker,
	};

	// forward pipeline shader programs
	const a3_DemoStateShaderProgram* forwardProgram[][demoStateForwardShadingModeMax] = {
		{
			demoState->prog_drawColorUnif,
			demoState->prog_drawTexture,
			demoState->prog_drawLambert_multi,
			demoState->prog_drawPhong_multi,
			demoState->prog_drawNonphoto_multi,
		}, {
			demoState->prog_drawColorUnif,
			demoState->prog_drawTexture_mrt,
			demoState->prog_drawLambert_multi_mrt,
			demoState->prog_drawPhong_multi_mrt,
			demoState->prog_drawNonphoto_multi_mrt,
		},
	};

	// display shader programs
	const a3_DemoStateShaderProgram* displayProgram[][demoStateForwardDisplayModeMax] = {
		{
			0,
		}, {
			demoState->prog_drawTexture,
			demoState->prog_drawTexture_colorManip,
			demoState->prog_drawTexture_coordManip,
		},
	};

	// ****TO-DO: 
	//	-> 2.1g: framebuffer for display
	
	// framebuffers from which to read based on pipeline mode
	const a3_Framebuffer* readFBO[] = {
		0,
		demoState->fbo_scene,
	};
	

	// tmp lighting data
	a3f32 lightSz[demoStateMaxCount_lightObject];
	a3f32 lightSzInvSq[demoStateMaxCount_lightObject];
	a3vec4 lightPos[demoStateMaxCount_lightObject];
	a3vec4 lightCol[demoStateMaxCount_lightObject];


	//-------------------------------------------------------------------------
	// 1) SCENE PASS: render scene with desired shader
	//	- activate scene framebuffer
	//	- draw scene
	//		- clear buffers
	//		- render shapes using appropriate shaders
	//		- capture color and depth

	// select target framebuffer
	switch (demoSubMode)
	{
		// shading
	case demoStateSubMode_main_shading:
		// target back buffer (default)
		a3framebufferDeactivateSetViewport(a3fbo_depth24_stencil8,
			-demoState->frameBorder, -demoState->frameBorder, demoState->frameWidth, demoState->frameHeight);

		// skybox or regular clear
		glDisable(GL_STENCIL_TEST);
		glDisable(GL_BLEND);
		if (demoState->displaySkybox)
		{
			// change depth mode to 'always' to ensure box gets drawn and resets depth
			glDepthFunc(GL_ALWAYS);
			a3demo_drawSkybox(demoState);
			glDepthFunc(GL_LEQUAL);
		}
		else
		{
			// clearing is expensive!
			// only call clear if skybox is not used; 
			//	skybox will draw over everything otherwise
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
		break;

		// shading with MRT
	case demoStateSubMode_main_mrt:
		// ****TO-DO: 
		//	-> 2.1h: activate framebuffer
		
		// target scene framebuffer
		currentWriteFBO = demoState->fbo_scene;
		a3framebufferActivate(currentWriteFBO);
		

		// clear now, handle skybox later
		glDisable(GL_STENCIL_TEST);
		glDisable(GL_BLEND);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		break;
	}


	// optional stencil test before drawing objects
	if (demoState->stencilTest)
	{
		// draw to stencil buffer: 
		//	- render first sphere to the stencil buffer to set drawable area
		//		- don't want values for the shape to actually be drawn to 
		//			color or depth buffers, so apply a MASK for this object
		//	- enable stencil test for everything else

		// drawing "lens" object using simple program
		currentDemoProgram = demoState->prog_transform;
		a3shaderProgramActivate(currentDemoProgram->program);
		a3real4x4SetScale(modelMat.m, a3real_four);
		a3real4x4Product(modelViewProjectionMat.m, activeCamera->viewProjectionMat.m, modelMat.m);
		a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uMVP, 1, modelViewProjectionMat.mm);

		// default stencil write settings
		a3demo_enableStencilWrite();

		// enable test and clear buffer (do this after mask is set)
		glEnable(GL_STENCIL_TEST);
		glClear(GL_STENCIL_BUFFER_BIT);

		// disable drawing this object to color or depth
	//	glColorMask(...);
	//	glDepthMask(...);

		// inverted small sphere in solid transparent color
		// used as our "lens" for the depth and stencil tests
		glCullFace(GL_FRONT);
		a3vertexDrawableActivateAndRender(demoState->draw_sphere);
		glCullFace(GL_BACK);

		// enable drawing following objects to color and depth
	//	glColorMask(...);
	//	glDepthMask(...);

		// default stencil compare settings
		a3demo_enableStencilCompare();
	}


	// copy temp light data
	for (k = 0, pointLight = demoState->forwardPointLight;
		k < demoState->forwardLightCount;
		++k, ++pointLight)
	{
		lightSz[k] = pointLight->radius;
		lightSzInvSq[k] = pointLight->radiusInvSq;
		lightPos[k] = pointLight->viewPos;
		lightCol[k] = pointLight->color;
	}


	// support multiple geometry passes
	for (i = 0, j = 1; i < j; ++i)
	{
		// select forward algorithm
		switch (i)
		{
			// forward pass
		case 0: {
			// select program based on settings
			currentDemoProgram = forwardProgram[demoSubMode][demoState->forwardShadingMode];
			a3shaderProgramActivate(currentDemoProgram->program);

			// send shared data: 
			//	- projection matrix
			//	- light data
			//	- activate shared textures including atlases if using
			//	- shared animation data
			a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uP, 1, activeCamera->projectionMat.mm);
			a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uP_inv, 1, activeCamera->projectionMatInv.mm);
			a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uAtlas, 1, a3mat4_identity.mm);
			a3shaderUniformSendDouble(a3unif_single, currentDemoProgram->uTime, 1, &demoState->renderTimer->totalTime);
			a3shaderUniformSendFloat(a3unif_vec4, currentDemoProgram->uColor, 1, skyblue);
			a3shaderUniformSendInt(a3unif_single, currentDemoProgram->uLightCt, 1, &demoState->forwardLightCount);
			a3shaderUniformSendFloat(a3unif_single, currentDemoProgram->uLightSz, demoState->forwardLightCount, lightSz);
			a3shaderUniformSendFloat(a3unif_single, currentDemoProgram->uLightSzInvSq, demoState->forwardLightCount, lightSzInvSq);
			a3shaderUniformSendFloat(a3unif_vec4, currentDemoProgram->uLightPos, demoState->forwardLightCount, lightPos->v);
			a3shaderUniformSendFloat(a3unif_vec4, currentDemoProgram->uLightCol, demoState->forwardLightCount, lightCol->v);
			a3textureActivate(demoState->tex_ramp_dm, a3tex_unit04);
			a3textureActivate(demoState->tex_ramp_sm, a3tex_unit05);

			// individual object requirements: 
			//	- modelviewprojection
			//	- modelview
			//	- modelview for normals
			//	- per-object animation data
			for (k = 0,
				currentSceneObject = demoState->planeObject, endSceneObject = demoState->teapotObject;
				currentSceneObject <= endSceneObject;
				++k, ++currentSceneObject)
			{
				// update and send data
				modelMat = currentSceneObject->modelMat;
				a3real4x4Product(modelViewMat.m, activeCameraObject->modelMatInv.m, modelMat.m);
				a3real4x4Product(modelViewProjectionMat.m, activeCamera->viewProjectionMat.m, modelMat.m);

				a3shaderUniformSendFloat(a3unif_vec4, currentDemoProgram->uColor, 1, rgba4[k + 3].v);
				a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uMVP, 1, modelViewProjectionMat.mm);
				a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uMV, 1, modelViewMat.mm);
				a3demo_quickInvertTranspose_internal(modelViewMat.m);
				modelViewMat.v3 = a3vec4_zero;
				a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uMV_nrm, 1, modelViewMat.mm);
				a3textureActivate(texture_dm[k], a3tex_unit00);
				a3textureActivate(texture_sm[k], a3tex_unit01);

				// draw
				currentDrawable = drawable[k];
				a3vertexDrawableActivateAndRender(currentDrawable);
			}
		}	break;
			// end geometry pass
		}
	}


	// stop using stencil
	if (demoState->stencilTest)
		glDisable(GL_STENCIL_TEST);


	// draw grid aligned to world
	if (demoState->displayGrid)
	{
		currentDemoProgram = demoState->prog_drawColorUnif;
		a3shaderProgramActivate(currentDemoProgram->program);
		currentDrawable = demoState->draw_grid;
		modelViewProjectionMat = activeCamera->viewProjectionMat;
		a3real4x4ConcatL(modelViewProjectionMat.m, demoState->gridTransform.m);
		a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uMVP, 1, modelViewProjectionMat.mm);
		a3shaderUniformSendFloat(a3unif_vec4, currentDemoProgram->uColor, 1, demoState->gridColor.v);
		a3vertexDrawableActivateAndRender(currentDrawable);
	}


	//-------------------------------------------------------------------------
	// PREPARE FOR POST-PROCESSING
	//	- double buffer swap (if applicable)
	//	- ensure blending is disabled
	//	- re-activate FSQ drawable IF NEEDED (i.e. changed in previous step)
	glDisable(GL_BLEND);
	currentDrawable = demoState->draw_unitquad;
	a3vertexDrawableActivate(currentDrawable);


	//-------------------------------------------------------------------------
	// DISPLAY: final pass, perform and present final composite
	//	- finally draw to back buffer
	//	- select display texture(s)
	//	- activate final pass program
	//	- draw final FSQ

	// revert to back buffer and disable depth testing
	a3framebufferDeactivateSetViewport(a3fbo_depthDisable,
		-demoState->frameBorder, -demoState->frameBorder, demoState->frameWidth, demoState->frameHeight);

	// ****TO-DO: 
	//	-> 2.3a: select display framebuffer
	
	// select framebuffer to display based on mode
	currentReadFBO = readFBO[demoSubMode];

	// select output to display
	switch (demoSubMode)
	{
		// no framebuffer active for scene render
	case demoStateSubMode_main_shading:
		// do nothing
		break;

		// scene was rendered to framebuffer
	case demoStateSubMode_main_mrt:
		// composite skybox
		if (demoState->displaySkybox)
		{
			a3demo_drawSkybox(demoState);
			a3demo_enableCompositeBlending();
		}

		// select output to display
		if (currentReadFBO->color && (!currentReadFBO->depthStencil || demoOutput < demoOutputCount - 1))
			a3framebufferBindColorTexture(currentReadFBO, a3tex_unit00, demoOutput);
		else
			a3framebufferBindDepthTexture(currentReadFBO, a3tex_unit00);
		break;
	}
	


	// ****TO-DO: 
	//	-> 2.3b: display scene rendered off-screen
	
	// final display: activate desired final program and draw FSQ
	if (currentReadFBO)
	{
		// prepare for final draw
		currentDrawable = demoState->draw_unitquad;
		a3vertexDrawableActivate(currentDrawable);

		// determine if additional passes are required
		{
			// most basic option: simply display texture
			currentDemoProgram = displayProgram[demoSubMode][demoState->forwardDisplayMode];
			a3shaderProgramActivate(currentDemoProgram->program);
		}

		// done
		a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uMVP, 1, a3mat4_identity.mm);
		a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uAtlas, 1, a3mat4_identity.mm);
		a3shaderUniformSendDouble(a3unif_single, currentDemoProgram->uTime, 1, &demoState->renderTimer->totalTime);
		a3vertexDrawableRenderActive();
	}
	


	//-------------------------------------------------------------------------
	// OVERLAYS: done after FSQ so they appear over everything else
	//	- disable depth testing
	//	- draw overlays appropriately

	// hidden volumes
	if (demoState->displayHiddenVolumes)
	{
		// draw light volumes
		glCullFace(GL_FRONT);
		currentDemoProgram = demoState->prog_drawColorUnif;
		currentDrawable = demoState->draw_pointlight;
		a3shaderProgramActivate(currentDemoProgram->program);
		a3vertexDrawableActivate(currentDrawable);
		for (k = 0; k < demoState->forwardLightCount; ++k)
		{
			a3real4x4SetScale(modelMat.m, (a3real)0.005 * demoState->forwardPointLight[k].radius);
			modelMat.v3 = demoState->forwardPointLight[k].worldPos;
			a3real4x4Product(modelViewProjectionMat.m, activeCamera->viewProjectionMat.m, modelMat.m);
			a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uMVP, 1, modelViewProjectionMat.mm);
			a3shaderUniformSendFloat(a3unif_vec4, currentDemoProgram->uColor, 1, demoState->forwardPointLight[k].color.v);
			a3vertexDrawableRenderActive();
		}
		glCullFace(GL_BACK);
	}


	// superimpose axes
	// draw coordinate axes in front of everything
	currentDemoProgram = demoState->prog_drawColorAttrib;
	a3shaderProgramActivate(currentDemoProgram->program);
	currentDrawable = demoState->draw_axes;
	a3vertexDrawableActivate(currentDrawable);

	// center of world from current viewer
	// also draw other viewer/viewer-like object in scene
	if (demoState->displayWorldAxes)
	{
		modelViewProjectionMat = activeCamera->viewProjectionMat;
		a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uMVP, 1, modelViewProjectionMat.mm);
		a3vertexDrawableRenderActive();
	}

	// individual objects
	if (demoState->displayObjectAxes)
	{
		// scene objects
		for (k = 0,
			currentSceneObject = demoState->planeObject, endSceneObject = demoState->teapotObject;
			currentSceneObject <= endSceneObject;
			++k, ++currentSceneObject)
		{
			a3real4x4Product(modelViewProjectionMat.m, activeCamera->viewProjectionMat.m, currentSceneObject->modelMat.m);
			a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uMVP, 1, modelViewProjectionMat.mm);
			a3vertexDrawableRenderActive();
		}

		// other objects
		if (demoState->displayHiddenVolumes)
		{
			for (k = 0,
				currentSceneObject = demoState->mainLightObject, endSceneObject = demoState->mainLightObject;
				currentSceneObject <= endSceneObject;
				++k, ++currentSceneObject)
			{
				a3real4x4Product(modelViewProjectionMat.m, activeCamera->viewProjectionMat.m, currentSceneObject->modelMat.m);
				a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uMVP, 1, modelViewProjectionMat.mm);
				a3vertexDrawableRenderActive();
			}
		}
	}


	// pipeline
	if (demoState->displayPipeline)
	{

	}

	// ****TO-DO: optionally comment out
	currentReadFBO = currentWriteFBO = 0;
}


//-----------------------------------------------------------------------------
// RENDER

void a3demo_render(const a3_DemoState *demoState)
{
	// display mode for current pipeline
	// ensures we don't go through the whole pipeline if not needed
	const a3ui32 demoMode = demoState->demoMode, demoModeCount = demoState->demoModeCount;
	const a3ui32 demoSubMode = demoState->demoSubMode[demoMode], demoSubModeCount = demoState->demoSubModeCount[demoMode];
	const a3ui32 demoOutput = demoState->demoOutputMode[demoMode][demoSubMode], demoOutputCount = demoState->demoOutputCount[demoMode][demoSubMode];


	// choose render sub-routine for the current mode
	switch (demoMode)
	{
		// main render pipeline
	case demoStateMode_main:
		a3demo_render_main(demoState, demoSubMode, demoOutput, demoSubModeCount, demoOutputCount);
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
		case 1:
			// choose text sub-routine based on pipeline
			switch (demoMode)
			{
				// main render pipeline
			case demoStateMode_main:
				a3demo_render_main_controls(demoState, 
					demoSubMode, demoOutput, demoSubModeCount, demoOutputCount);
				break;
			}
			break;

			// general data
		case 2: 
			a3demo_render_data(demoState);
			break;
		}
	}
}


//-----------------------------------------------------------------------------
