#include <iostream>
#include "Camera.h"
#include "Cube.h"
#include "EnvironmentMap.h"
#include "Font.h"
#include "FpsTracker.h"
#include "LightSource.h"
#include "Line.h"
#include "Puddi.h"
#include "RenderGraph.h"
#include "Rectangle.h"
#include "Scene.h"
#include "Schematic.h"
#include "Shader.h"
#include "Shadow.h"
#include "Skeleton.h"
#include "Skybox.h"
#include "Sphere.h"
#include "Texture.h"
#include "Util.h"

using namespace std;

namespace puddi
{
  namespace engine
  {
    // PRIVATE
    namespace
    {
      SDL_Window* window;
      SDL_GLContext glcontext;

      Object* rootObject = new Object(nullptr);
      // UpdateNode* rootUpdateNode;
      // ModelNode* rootModelNode;

      Scene *world_scene;
      Camera *world_camera;

      vector<Scene*> scenes;

      std::vector<init_function> postInitFunctions;
      std::vector<update_function> updateFunctions;
      std::vector<draw_function> preDrawFunctions;
      std::vector<draw_function> drawFunctions;

      glm::vec3 shadowLightPosition;
      DrawableObject* shadowIgnoreObject = nullptr;

      int update()
      {
	FpsTracker::Update();

	SDL_SetWindowTitle(window, Util::ToString(FpsTracker::GetFps()).c_str());

	// rootModelNode->Update();

	// rootUpdateNode->Update();
	//std::cout << rootModelNode->children.size() << std::endl;

	UpdateAll();

	return 0;
      }

      void preDraw()
      {
	if (Shadow::GetMode() == SHADOW_MODE_UNI)
	  Shadow::RenderShadowOrthoMap(world_scene, shadowLightPosition);
	else if (Shadow::GetMode() == SHADOW_MODE_OMNI)
	  Shadow::RenderShadowCubeMap(world_scene, shadowLightPosition,
				      shadowIgnoreObject);
      }

      void draw()
      {
	// UpdateProjectionMatricesAndViewport();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render all scenes
	RenderAll();

	// call additional draw functions
	for (auto it = drawFunctions.begin(); it != drawFunctions.end(); ++it)
	  (*it)();

	SDL_GL_SwapWindow(window);
	glFlush();
      }

      int cleanup(int status_code)
      {
	Schematic::Cleanup();
	Skeleton::Cleanup();

	SDL_GL_DeleteContext(glcontext);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return status_code;
      }

      void sendVertexDataToGPU()
      {
	Shader::Init();

	// clear all vertex data on cpu side
	Shader::Vertices.clear();
	Shader::Normals.clear();
	Shader::Tangents.clear();
	Shader::Binormals.clear();
	Shader::TextureCoordinates.clear();
	Shader::VertexIndices.clear();
	Shader::Vertices.resize(0);
	Shader::Normals.resize(0);
	Shader::Tangents.resize(0);
	Shader::Binormals.resize(0);
	Shader::TextureCoordinates.resize(0);
	Shader::VertexIndices.resize(0);
      }
    }

    // PUBLIC

    float WorldSize;

    std::string WindowTitleMessage = "";

    int Init(float worldSize)
    {
      srand(static_cast<unsigned int>(time(NULL)));

      WorldSize = worldSize;

      if (int status = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO) != 0)
      {
	std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
	SDL_Quit();
	return status;
      }

      //if (TTF_Init())
      //{
      //std::cerr << "Failed to initialize TTF. Error: " << TTF_GetError() << std::endl;
      //}

      // 4x AA
      SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
      SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

      window =
	SDL_CreateWindow("Puddi", SDL_WINDOWPOS_CENTERED,
			 SDL_WINDOWPOS_CENTERED, 640, 480,
			 SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

      // Create an OpenGL context associated with the window.
      glcontext = SDL_GL_CreateContext(window);

      // enable AA
      glEnable(GL_MULTISAMPLE);

      //SDL_GL_MakeCurrent(window, glcontext);
      // Needed for glTexBuffer on OpenGL 3.0 (for buffer texture)
      glewExperimental = GL_TRUE;
      if (glewInit() != GLEW_OK)
      {
	std::cerr << "GLEW failed to load.\n";
	exit(1);
      }

      if (SDL_SetRelativeMouseMode(SDL_TRUE) == -1)
	std::cerr << "unable to set relative mouse mode.\n";

      // Main world scene
      world_scene = new Scene(PROJ_PERSPECTIVE, true);
      world_scene->SetDepth(worldSize * 1.6f);
      scenes.push_back(world_scene);

      Line::Init();
      Rectangle::Init();
      Cube::Init();
      Sphere::Init();
      Skybox::Init();
      LightSource::Init();
      Shadow::Init();
      EnvironmentMap::Init();
      Schematic::Init();
      Skeleton::Init();
      Font::Init();

      glEnable(GL_DEPTH_TEST);

      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      //glBlendFunc(GL_ZERO, GL_SRC_COLOR);
      //glBlendFunc(GL_ONE, GL_ONE);

      if (GLEW_VERSION_3_1)
      {
	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(UINT_MAX);
      }

      // ViewDistance = WorldSize * 1.6f;

      // set default clear color
      glClearColor(0.5, 0.5, 1.0, 1.0);

      // initialize projection matrix
      UpdateProjectionMatricesAndViewport();

      // SET SHADOW NEAR AND FAR PLANES
      // Shadow::SetZRange(2.0f, ViewDistance);
      Shadow::SetZRange(2.0f, WorldSize * 1.6f);

      // enable seamless cube maps
      glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

      // start texture loading
      Texture::InitTextureLoading();

      std::cerr << "Puddi: initialization complete. Waiting for call to Run() ...\n";

      return 0;
    }

    int Run()
    {
      // end texture loading
      Texture::EndTextureLoading();

      sendVertexDataToGPU();

      // call post-init functions
      for (auto it = postInitFunctions.begin(); it != postInitFunctions.end(); ++it)
	(*it)();

      int status_code;

      std::cerr << "Puddi: entering main loop\n";

      while (true)
      {
	if ((status_code = update()))
	  return cleanup(status_code);

	// call additional update functions
	for (auto it = updateFunctions.begin(); it != updateFunctions.end(); ++it)
	{
	  if ((status_code = (*it)()))
	    return cleanup(status_code);
	}

	// call pre-draw functions
	for (auto it = preDrawFunctions.begin(); it != preDrawFunctions.end(); ++it)
	  (*it)();

	preDraw();

	draw();
      }
    }

    void UpdateProjectionMatricesAndViewport()
    {
      int w, h;
      SDL_GetWindowSize(window, &w, &h);
      // ProjectionMatrix = perspective(static_cast<float>(M_PI) / 4.0f,
      // 				     w / static_cast<float>(h), 1.0f,
      // 				     ViewDistance);
      // Shader::SetProjection(ProjectionMatrix);
      for (auto it = scenes.begin(); it != scenes.end(); ++it) {
	(*it)->ViewportChange(w, h);
      }

      glViewport(0, 0, w, h);
    }

    Scene* GetScene(size_t index)
    {
      if (index >= scenes.size())
	return nullptr;
      return scenes[index];
    }

    size_t AddScene(ProjectionType pType, bool cam_object)
    {
      scenes.push_back(new Scene(pType, cam_object));
      return scenes.size() - 1;
    }

    void UpdateAll()
    {
      for (auto it = scenes.begin(); it != scenes.end(); ++it)
    	(*it)->Update();
    }

    void RenderAll()
    {
      for (auto it = scenes.begin(); it != scenes.end(); ++it)
    	(*it)->Draw();
    }

    void ForceModelUpdates()
    {
      for (auto it = scenes.begin(); it != scenes.end(); ++it) {
	(*it)->ForceModelUpdate();
      }
    }

    void ToggleFullScreen()
    {
      SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    }

    void RegisterPostInitFunction(init_function f)
    {
      postInitFunctions.push_back(f);
    }

    void RegisterUpdateFunction(update_function f)
    {
      updateFunctions.push_back(f);
    }

    void RegisterPreDrawFunction(draw_function f)
    {
      preDrawFunctions.push_back(f);
    }

    void RegisterDrawFunction(draw_function f)
    {
      drawFunctions.push_back(f);
    }

    void EnableShadows(ShadowMode mode, ShadowResolution resolution)
    {
      Shadow::SetResolution(resolution);

      // initial call to these so that the shadow map binding will
      // work when you enable shadows
      if (mode == SHADOW_MODE_UNI)
	Shadow::RenderShadowOrthoMap(world_scene, vec3(1.0f, 0.0f, 0.0f));
      else
	Shadow::RenderShadowCubeMap(world_scene, vec3(1.0f, 0.0f, 0.0f),
				    shadowIgnoreObject);

      Shadow::SetMode(mode);
    }

    void SetShadowLightPosition(const glm::vec3 &pos)
    {
      shadowLightPosition = pos;
    }

    void SetShadowIgnoreObject(DrawableObject *o)
    {
      shadowIgnoreObject = o;
    }

    void SetLineWidth(float w)
    {
      glLineWidth(w);
    }

    Scene* GetWorldScene()
    {
      return world_scene;
    }

    Camera* GetWorldCamera()
    {
      return world_camera;
    }
  }
}
