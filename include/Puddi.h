#ifndef PUDDI_H
#define PUDDI_H

#define _CRT_SECURE_NO_DEPRECATE
#define GLUT_DISABLE_ATEXIT_HACK
#define GLM_FORCE_RADIANS

#include "GlmHeaders.h"
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <string>

#include "Camera.h"
#include "Object.h"

namespace puddi
{
	class Object;
	class RenderGraph;
	class Camera;
	struct UpdateNode;
	struct ModelNode;

	typedef int (*update_function)(void);
	typedef void (*draw_function)(void);

	class Puddi
	{
	public:
		static float WorldSize;
		static float ViewDistance;
		static float FOV;

		static mat4 ProjectionMatrix;
		static mat4 CameraMatrix;
		static vec4 CameraPosition;

		static Camera *MainCamera;

		static RenderGraph *MainRenderGraph;

		// secondary is mainly for objects with transparency
		// to be rendered after everything else
		static RenderGraph *SecondaryRenderGraph;

		static std::string WindowTitleMessage;

		static int Init(float worldSize);

		static int Run();

		static void UpdateProjectionMatrixAndViewport();

		static const Object* GetRootObject();

		// only enables full screen atm
		static void ToggleFullScreen();

		static void RegisterUpdateFunction(update_function f);
		static void RegisterPreDrawFunction(draw_function f);
		static void RegisterDrawFunction(draw_function f);

	private:
		static SDL_Window *window;
		static SDL_GLContext glcontext;

		static Object *rootObject;
		static UpdateNode *rootUpdateNode;
		static ModelNode *rootModelNode;

		static std::vector<update_function> updateFunctions;
		static std::vector<draw_function> preDrawFunctions;
		static std::vector<draw_function> drawFunctions;

		static int update();

		static void draw();

		// do this after all vertex data is loaded
		static void sendVertexDataToGPU();

		static int cleanup(int status_code);
	};
}

#endif
