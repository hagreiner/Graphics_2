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
	
	a3_DemoState.h
	Demo state interface and programmer function declarations.

	********************************************
	*** THIS IS YOUR DEMO'S MAIN HEADER FILE ***
	********************************************
*/

#ifndef __ANIMAL3D_DEMOSTATE_H
#define __ANIMAL3D_DEMOSTATE_H


//-----------------------------------------------------------------------------
// animal3D framework includes

#include "animal3D/animal3D.h"
#include "animal3D-A3DG/animal3D-A3DG.h"


//-----------------------------------------------------------------------------
// other demo includes

#include "_utilities/a3_DemoSceneObject.h"
#include "_utilities/a3_DemoShaderProgram.h"


//-----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#else	// !__cplusplus
	typedef struct a3_DemoState					a3_DemoState;
#endif	// __cplusplus


//-----------------------------------------------------------------------------

	// object maximum counts for easy array storage
	// good idea to make these numbers greater than what you actually need 
	//	and if you end up needing more just increase the count... there's 
	//	more than enough memory to hold extra objects
	enum a3_DemoStateObjectMaxCounts
	{
		demoStateMaxCount_sceneObject = 8,
		demoStateMaxCount_cameraObject = 1,
		demoStateMaxCount_lightObject = 4,
		demoStateMaxCount_projector = 1,

		demoStateMaxCount_timer = 1,

		demoStateMaxCount_drawDataBuffer = 1,
		demoStateMaxCount_vertexArray = 8,
		demoStateMaxCount_drawable = 16,
		
		demoStateMaxCount_shaderProgram = 32,
		
		demoStateMaxCount_texture = 16,

		demoStateMaxCount_framebuffer = 1,
	};

	// additional counters for demo modes
	enum a3_DemoStateModeCounts
	{
		demoStateMaxModes = 1,
		demoStateMaxSubModes = 2,
		demoStateMaxOutputModes = 9,
	};

	// demo mode names
	enum a3_DemoStateModeNames
	{
		demoStateMode_main,
	};

	// demo sub-mode names
	enum a3_DemoStateSubModeNames_main
	{
		demoStateSubMode_main_shading,
		demoStateSubMode_main_mrt,
	};

	// demo output mode names
	enum a3_DemoStateOutputNames_main_mrt
	{
		demoStateOutput_main_mrt_composite,
		demoStateOutput_main_mrt_position,
		demoStateOutput_main_mrt_normal,
		demoStateOutput_main_mrt_texcoord,
		demoStateOutput_main_mrt_diffuseTex,
		demoStateOutput_main_mrt_specularTex,
		demoStateOutput_main_mrt_diffuseLight,
		demoStateOutput_main_mrt_specularLight,
		demoStateOutput_main_mrt_fragdepth,
	};


//-----------------------------------------------------------------------------

	// pipeline modes
	enum a3_DemoStatePipelineModeNames
	{
		demoStatePipelineMode_forward,
	};

	// forward shading modes
	enum a3_DemoStateForwardShadingModeNames
	{
		demoStateForwardShadingMode_solid,
		demoStateForwardShadingMode_texture,
		demoStateForwardShadingMode_Lambert,
		demoStateForwardShadingMode_Phong,
		demoStateForwardShadingMode_nonphoto,

		demoStateForwardShadingModeMax
	};

	// display modes
	enum a3_DemoStateForwardDisplayModeNames
	{
		demoStateForwardDisplayMode_texture,
		demoStateForwardDisplayMode_texture_colorManip,
		demoStateForwardDisplayMode_texture_texcoordManip,

		demoStateForwardDisplayModeMax
	};

	
//-----------------------------------------------------------------------------

	// persistent demo state data structure
	struct a3_DemoState
	{
		//---------------------------------------------------------------------
		// general variables pertinent to the state

		// terminate key pressed
		a3i32 exitFlag;

		// global vertical axis: Z = 0, Y = 1
		a3i32 verticalAxis;

		// asset streaming between loads enabled (careful!)
		a3i32 streaming;

		// window and full-frame dimensions
		a3ui32 windowWidth, windowHeight;
		a3real windowWidthInv, windowHeightInv, windowAspect;
		a3ui32 frameWidth, frameHeight;
		a3real frameWidthInv, frameHeightInv, frameAspect;
		a3i32 frameBorder;


		//---------------------------------------------------------------------
		// objects that have known or fixed instance count in the whole demo

		// text renderer
		a3i32 textInit, textMode, textModeCount;
		a3_TextRenderer text[1];

		// input
		a3_MouseInput mouse[1];
		a3_KeyboardInput keyboard[1];
		a3_XboxControllerInput xcontrol[4];

		// pointer to fast trig table
		a3f32 trigTable[4096 * 4];


		//---------------------------------------------------------------------
		// scene variables and objects

		// demo mode array: 
		//	- mode: which mode/pipeline is being viewed
		//	- sub-mode: which sub-mode/pass in the pipeline is being viewed
		//	- output: which output from the sub-mode/pass is being viewed
		a3ui32 demoMode, demoSubMode[demoStateMaxModes], demoOutputMode[demoStateMaxModes][demoStateMaxSubModes];
		a3ui32 demoModeCount, demoSubModeCount[demoStateMaxModes], demoOutputCount[demoStateMaxModes][demoStateMaxSubModes];

		// toggle grid in scene and axes superimposed, as well as other mods
		a3boolean displayGrid, displayWorldAxes, displayObjectAxes, displayTangentBases;
		a3boolean displaySkybox, displayHiddenVolumes, displayPipeline;
		a3boolean updateAnimation;
		a3boolean stencilTest;

		// grid properties
		a3mat4 gridTransform;
		a3vec4 gridColor;

		// cameras
		a3ui32 activeCamera;

		// lights
		a3ui32 lightingPipelineMode;
		a3ui32 forwardShadingMode, forwardShadingModeCount;
		a3ui32 forwardLightCount;
		a3_DemoPointLight forwardPointLight[demoStateMaxCount_lightObject];

		// display modes
		a3ui32 forwardDisplayMode, forwardDisplayModeCount;


		//---------------------------------------------------------------------
		// object arrays: organized as anonymous unions for two reasons: 
		//	1. easy to manage entire sets of the same type of object using the 
		//		array component
		//	2. at the same time, variables are named pointers

		// scene objects
		union {
			a3_DemoSceneObject sceneObject[demoStateMaxCount_sceneObject];
			struct {
				// main scene objects
				a3_DemoSceneObject
					skyboxObject[1],
					planeObject[1],
					sphereObject[1],
					cylinderObject[1],
					torusObject[1],
					teapotObject[1];
			};
		};
		union {
			a3_DemoSceneObject cameraObject[demoStateMaxCount_cameraObject];
			struct {
				a3_DemoSceneObject
					mainCameraObject[1];
			};
		};
		union {
			a3_DemoSceneObject lighObject[demoStateMaxCount_lightObject];
			struct {
				a3_DemoSceneObject
					mainLightObject[1];
			};
		};

		// projectors/cameras
		//	- any object can have a projector "component" to make it a viewer
		union {
			a3_DemoProjector projector[demoStateMaxCount_projector];
			struct {
				a3_DemoProjector
					sceneCamera[1];						// scene viewing cameras
			};
		};


		// timers
		union {
			a3_Timer timer[demoStateMaxCount_timer];
			struct {
				a3_Timer
					renderTimer[1];						// render FPS timer
			};
		};


		// draw data buffers
		union {
			a3_VertexBuffer drawDataBuffer[demoStateMaxCount_drawDataBuffer];
			struct {
				a3_VertexBuffer
					vbo_staticSceneObjectDrawBuffer[1];			// buffer to hold all data for static scene objects (e.g. grid)
			};
		};

		// vertex array objects
		union {
			a3_VertexArrayDescriptor vertexArray[demoStateMaxCount_vertexArray];
			struct {
				a3_VertexArrayDescriptor
					vao_position_texcoord_normal[1],			// VAO for vertex format with position, texture coordinates and normal
					vao_position_texcoord[1],					// VAO for vertex format with position and texture coordinates
					vao_position_color[1],						// VAO for vertex format with position and color
					vao_position[1];							// VAO for vertex format with only position
			};
		};

		// drawables
		union {
			a3_VertexDrawable drawable[demoStateMaxCount_drawable];
			struct {
				a3_VertexDrawable
					draw_axes[1];								// coordinate axes at the center of the world
				a3_VertexDrawable
					draw_grid[1],								// wireframe ground plane to emphasize scaling
					draw_skybox[1],								// skybox cube mesh
					draw_unitquad[1];							// unit quad (used for fsq)
				a3_VertexDrawable
					draw_pointlight[1];							// volume for point light (low-res sphere)
				a3_VertexDrawable
					draw_plane[1],								// procedural plane
					draw_sphere[1],								// procedural sphere
					draw_cylinder[1],							// procedural cylinder
					draw_torus[1],								// procedural torus
					draw_teapot[1];								// can't not have a Utah teapot
			};
		};


		// shader programs and uniforms
		union {
			a3_DemoStateShaderProgram shaderProgram[demoStateMaxCount_shaderProgram];
			struct {
				a3_DemoStateShaderProgram
					prog_transform_instanced[1],				// transform vertex only with instancing; no fragment shader
					prog_transform[1];							// transform vertex only; no fragment shader
				a3_DemoStateShaderProgram
					prog_drawColorAttrib_instanced[1],			// draw color attribute with instancing
					prog_drawColorUnif_instanced[1],			// draw uniform color with instancing
					prog_drawColorAttrib[1],					// draw color attribute
					prog_drawColorUnif[1];						// draw uniform color
				a3_DemoStateShaderProgram
					prog_drawNonphoto_multi[1],					// draw non-photorealistic shading model, multiple lights
					prog_drawPhong_multi[1],					// draw Phong shading model, multiple lights
					prog_drawLambert_multi[1],					// draw Lambert shading model, multiple lights
					prog_drawTexture[1];						// draw texture
				a3_DemoStateShaderProgram
					prog_drawTexture_coordManip[1],				// draw texture with manipulated texture coordinates
					prog_drawTexture_colorManip[1],				// draw texture with manipulated output color
					prog_drawNonphoto_multi_mrt[1],				// draw non-photorealistic shading model, multiple lights, MRT
					prog_drawPhong_multi_mrt[1],				// draw Phong shading model, multiple lights, MRT
					prog_drawLambert_multi_mrt[1],				// draw Lambert shading model, multiple lights, MRT
					prog_drawTexture_mrt[1];					// draw texture, MRT
			};
		};


		// textures
		union {
			a3_Texture texture[demoStateMaxCount_texture];
			struct {
				a3_Texture
					tex_skybox_clouds[1],
					tex_skybox_water[1],
					tex_earth_dm[1],
					tex_earth_sm[1],
					tex_mars_dm[1],
					tex_mars_sm[1],
					tex_stone_dm[1],
					tex_ramp_dm[1],
					tex_ramp_sm[1],
					tex_checker[1];
			};
		};


		// ****TO-DO: 
		//	-> 2.1a: framebuffer object union
		
		// framebuffers
		union {
			a3_Framebuffer framebuffer[demoStateMaxCount_framebuffer];
			struct {
				a3_Framebuffer
					fbo_scene[1];								// framebuffer for capturing scene
			};
		};
		


		// managed objects, no touchie
		a3_VertexDrawable dummyDrawable[1];


		//---------------------------------------------------------------------
	};

	
//-----------------------------------------------------------------------------

	// demo-related functions

	// idle loop
	void a3demo_input(a3_DemoState* demoState, a3f64 dt);
	void a3demo_update(a3_DemoState* demoState, a3f64 dt);
	void a3demo_render(a3_DemoState const* demoState);

	// loading
	void a3demo_loadGeometry(a3_DemoState* demoState);
	void a3demo_loadShaders(a3_DemoState* demoState);
	void a3demo_loadTextures(a3_DemoState* demoState);
	void a3demo_loadFramebuffers(a3_DemoState* demoState);
	void a3demo_refresh(a3_DemoState* demoState);

	// unloading
	void a3demo_unloadGeometry(a3_DemoState* demoState);
	void a3demo_unloadShaders(a3_DemoState* demoState);
	void a3demo_unloadTextures(a3_DemoState* demoState);
	void a3demo_unloadFramebuffers(a3_DemoState* demoState);
	void a3demo_validateUnload(a3_DemoState const* demoState);

	// other utils & setup
	void a3demo_setDefaultGraphicsState();
	void a3demo_initScene(a3_DemoState* demoState);
	void a3demo_initSceneRefresh(a3_DemoState* demoState);


//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif	// __cplusplus


#endif	// !__ANIMAL3D_DEMOSTATE_H