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

#include "_a3_demo_utilities/a3_DemoSceneObject.h"
#include "_a3_demo_utilities/a3_DemoShaderProgram.h"

#include "a3_Demo_Shading.h"
#include "a3_Demo_Pipelines.h"


//-----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#else	// !__cplusplus
	typedef struct a3_DemoState					a3_DemoState;
	typedef enum a3_DemoState_ModeName			a3_DemoState_ModeName;
	typedef enum a3_DemoState_TextDisplayName	a3_DemoState_TextDisplayName;
#endif	// __cplusplus


//-----------------------------------------------------------------------------

	// demo mode names
	enum a3_DemoState_ModeName
	{
		demoState_shading,				// basic shading mode
		demoState_pipelines,			// different pipelines for exploration

		demoState_mode_max
	};


	// text mode names
	enum a3_DemoState_TextDisplayName
	{
		demoState_textDisabled,			// no text overlay
		demoState_textControls,			// display controls
		demoState_textControls_gen,		// display general controls
		demoState_textData,				// display data

		demoState_text_max
	};


	// object maximum counts for easy array storage
	// good idea to make these numbers greater than what you actually need 
	//	and if you end up needing more just increase the count... there's 
	//	more than enough memory to hold extra objects
	enum a3_DemoState_ObjectMaxCount
	{
		demoStateMaxCount_sceneObject = 8,
		demoStateMaxCount_cameraObject = 1,
		demoStateMaxCount_lightObject = 4,
		demoStateMaxCount_projector = 2,

		demoStateMaxCount_timer = 1,

		demoStateMaxCount_drawDataBuffer = 1,
		demoStateMaxCount_vertexArray = 8,
		demoStateMaxCount_drawable = 16,
		
		demoStateMaxCount_shaderProgram = 32,
		
		demoStateMaxCount_texture = 16,

		demoStateMaxCount_framebuffer = 16,
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
		a3boolean streaming;

		// window and full-frame dimensions
		a3ui32 windowWidth, windowHeight;
		a3real windowWidthInv, windowHeightInv, windowAspect;
		a3ui32 frameWidth, frameHeight;
		a3real frameWidthInv, frameHeightInv, frameAspect;
		a3i32 frameBorder;


		//---------------------------------------------------------------------
		// objects that have known or fixed instance count in the whole demo

		// text renderer
		a3boolean textInit;
		a3_DemoState_TextDisplayName textMode;
		a3_TextRenderer text[1];

		// input
		a3_MouseInput mouse[1];
		a3_KeyboardInput keyboard[1];
		a3_XboxControllerInput xcontrol[4];

		// pointer to fast trig table
		a3f32 trigTable[4096 * 4];


		//---------------------------------------------------------------------
		// scene variables and objects

		// demo modes
		a3_Demo_Shading demoMode_shading[1];
		a3_Demo_Pipelines demoMode_pipelines[1];
		a3_DemoState_ModeName demoMode;

		// cameras
		a3ui32 activeCamera;

		// toggle grid in scene and axes superimposed, as well as other mods
		a3boolean displayGrid, displaySkybox, displayHiddenVolumes, displayPipeline;
		a3boolean displayWorldAxes, displayObjectAxes, displayTangentBases;
		a3boolean updateAnimation;
		a3boolean stencilTest;
		a3boolean skipIntermediatePasses;

		// grid properties
		a3mat4 gridTransform;
		a3vec4 gridColor;

		// lights
		a3ui32 forwardLightCount;
		a3_DemoPointLight forwardPointLight[demoStateMaxCount_lightObject];


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
				a3_DemoProjector
					shadowLight[1];						// light for capturing shadow map
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
				a3_DemoStateShaderProgram
					prog_drawTexture_outline[1],				// draw texture with outlines from prior pass
					prog_drawPhong_multi_shadow_mrt[1];			// draw Phong shading with shadow mapping
				a3_DemoStateShaderProgram
					prog_drawTexture_brightPass[1],				// draw texture with bright-pass or tone-mapping
					prog_drawTexture_blurGaussian[1],			// draw texture with Gaussian blurring
					prog_drawTexture_blendScreen4[1];			// draw texture with 4-layer screen blend
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


		// framebuffers
		union {
			a3_Framebuffer framebuffer[demoStateMaxCount_framebuffer];
			struct {
				a3_Framebuffer
					fbo_scene_c16d24s8_mrt[1];					// framebuffer for capturing scene
				a3_Framebuffer
					fbo_shadow_d32[1];							// framebuffer for capturing shadow map
				a3_Framebuffer
					// ****TO-DO: 
					//	-> 2.1a: uncomment post-processing framebuffers
					
					fbo_post_c16_2fr[3],						// framebuffers for post-processing, half frame size
					fbo_post_c16_4fr[3],						// framebuffers for post-processing, quarter frame size
					fbo_post_c16_8fr[3],						// framebuffers for post-processing, eighth frame size
					
					fbo_composite_c16[3];						// framebuffers for composition
			};
		};


		// managed objects, no touchie
		a3_VertexDrawable dummyDrawable[1];


		//---------------------------------------------------------------------
	};


//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif	// __cplusplus


#endif	// !__ANIMAL3D_DEMOSTATE_H