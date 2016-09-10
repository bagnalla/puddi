/**
 * @file Puddi.h
 * @author Alexander Bagnall
 * @brief
 *
 */

#ifndef PUDDI_H
#define PUDDI_H

#define _CRT_SECURE_NO_DEPRECATE
#define GLUT_DISABLE_ATEXIT_HACK
#define GLM_FORCE_RADIANS

#include <string>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include "GlmHeaders.h"

namespace puddi
{
    class RenderGraph;
    class Camera;
    class Object;
    class DrawableObject;
    enum ShadowMode : int;
    enum ShadowResolution : int;

    namespace engine
    {
        typedef int (*update_function)(void);
        typedef void (*draw_function)(void);
        typedef void (*init_function)(void);

        extern float WorldSize;
        extern float ViewDistance;
        extern float FOV;

        extern mat4 ProjectionMatrix;
        extern mat4 CameraMatrix;
        extern vec4 CameraPosition;

        extern Camera *MainCamera;

        extern std::string WindowTitleMessage;

        int Init(float worldSize);

        int Run();

        void UpdateProjectionMatrixAndViewport();

        Object* GetRootObject();

        RenderGraph* GetDefaultRenderGraph();
        RenderGraph* GetRenderGraph(size_t index);
        size_t AddRenderGraph();

        void RenderAll();

        void ForceModelUpdate();

        // only enables full screen atm
        void ToggleFullScreen();

        void RegisterPostInitFunction(init_function f);
        void RegisterUpdateFunction(update_function f);
        void RegisterPreDrawFunction(draw_function f);
        void RegisterDrawFunction(draw_function f);

        // must be called after Shader::Init (call from post_init function)
        void EnableShadows(ShadowMode mode, ShadowResolution resolution);

        void SetShadowLightPosition(const glm::vec3 &pos);
        void SetShadowIgnoreObject(DrawableObject *o);

        void SetRelativeMouseMode(int m);

        void SetClearColor(const vec4 &col);
    }
}

#endif
