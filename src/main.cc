#include "Puddi.h"
#include "FpsTracker.h"
#include "LightSource.h"
#include "DrawableObject.h"
#include "Texture.h"
#include "Cube.h"
#include "GlmHeaders.h"
#include "Rectangle.h"
#include "Skybox.h"
#include "Font.h"
#include <vector>
#include <iostream>
#include <string>

using namespace puddi;
using namespace std;

LightSource *lightSource;
Object *objectContainer;
DrawableObject *cube;
DrawableObject *rect;

void init(void)
{
	GLuint texture = Texture::LoadTexture("shrek", "textures/Shrek-and-Yoda.jpg");
	Texture::LoadCubeMap("skybox_2", "textures/skybox2.jpg");
	//GLuint texture = Texture::LoadTexture("shrek", "textures/Shrek-and-Yoda (copy).jpg");
	//GLuint texture = Texture::CreateTextureFromString("bakow", "bakow_string", "fonts/Bitstream/VeraMono-Bold.ttf", 24, vec3(1.0f, 0.0f, 0.0f));

	Puddi::MainCamera->SetPosition(vec4(0.0f, -5.0f, 0.0f, 1.0f));
	Puddi::MainCamera->LookAt(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	//Puddi::MainCamera->SetZoomOut(10.0f);

	lightSource = LightSource::ObtainLightSource();
	lightSource->ambient = vec4(1.0, 1.0, 1.0, 1.0);
	lightSource->diffuse = vec4(1.0, 1.0, 1.0, 1.0);
	lightSource->specular = vec4(1.0, 1.0, 1.0, 1.0);
	lightSource->position = vec4(-1.0f, -1.0f, 1.0f, 0.0f);
	lightSource->UpdateMatrix();
	LightSource::UpdateLightSourceMatricesInShaders();

	// CONTAINERS
	objectContainer = new Object(Puddi::GetRootObject());

	Skybox *skybox = new Skybox(Puddi::MainCamera);
	skybox->SetCubeMap(Texture::GetCubeMapByName("skybox_2"));
	skybox->SetEmissive(true);
	skybox->SetScale(Puddi::ViewDistance);

	// OBJECTS
    //rect = new Rectangle(objectContainer);
    //rect->SetTexture(texture);
    //rect->RotateX(M_PI / 2.0f);

	// MIDDLE CUBE
	//cube = new Cube(objectContainer);
	//cube->SetTexture(texture);
	//cube->Scale(00000.1f);
	
	//DrawableObject *object = new DrawableObject(objectContainer, VertexMesh::GetVertexMeshPrototypeByName("a"));
	////DrawableObject *object = new Rectangle(objectContainer);
	//object->SetTexture(Texture::GetTextureByName("myfont"));
	//object->SetBumpMapEnabled(false);
	//object->RotateX(M_PI / 2.0f);
	
	//int rows = 10;
	//int cols = 10;
	//for (int i = 0; i < rows; ++i)
	//{
		//for (int j = 0; j < cols; ++j)
		//{
			//DrawableObject *object = new DrawableObject(objectContainer, VertexMesh::GetVertexMeshPrototypeByName(std::string(1, i * cols + j + 32)));
			//object->SetTexture(Texture::GetTextureByName("myfont"));
			//object->RotateX(M_PI / 2.0f);
			//object->Translate(vec4(j, 0.0f, -i, 0.0f));
		//}
	//}
	
	//string msg = "int main() { cout << \"hello\" << endl; }";
	vector<string> lines;
	lines.push_back("int main()");
	lines.push_back("{");
	lines.push_back("  cout << \"hello\" << endl;");
	lines.push_back("}");
	for (int j = 0; j < lines.size(); j++)
	{
		string msg = lines[j];
		for (int i = 0; i < msg.length(); ++i)
		{
			DrawableObject *object = new DrawableObject(objectContainer, VertexMesh::GetVertexMeshPrototypeByName(std::string(1, msg[i])));
			object->SetTexture(Texture::GetTextureByName("myfont"));
			object->RotateX(M_PI / 2.0f);
			object->Translate(vec4(i, 0.0f, -j, 0.0f));
			object->SetEmissive(true);
		}
	}
}

//----------------------------------------------------------------------------

bool holdingMouseClick = false;
int update()
{
	SDL_Event ev;
	while (SDL_PollEvent(&ev))
	{
		// send event to camera
		Puddi::MainCamera->InputEvent(ev);

		// quit
		if (ev.type == SDL_QUIT)
		{
			return 1;
		}
		// key press
		else if (ev.type == SDL_KEYDOWN)
		{
			switch (ev.key.keysym.sym)
			{
			// EXIT PROGRAM
			case SDLK_ESCAPE:
				return 1;
				break;
			// ENABLE FULLSCREEN
			case SDLK_f:
				Puddi::ToggleFullScreen();
				break;
			// ROTATE MIDDLE OBJECT
			case SDLK_1:
				cube->RotateX(-0.005f * FpsTracker::GetFrameTimeMs());
				break;
			case SDLK_2:
				cube->RotateX(0.005f * FpsTracker::GetFrameTimeMs());
				break;
			case SDLK_3:
				cube->RotateY(-0.005f * FpsTracker::GetFrameTimeMs());
				break;
			case SDLK_4:
				cube->RotateY(0.005f * FpsTracker::GetFrameTimeMs());
				break;
			case SDLK_5:
				cube->RotateZ(-0.005f * FpsTracker::GetFrameTimeMs());
				break;
			case SDLK_6:
				cube->RotateZ(0.005f * FpsTracker::GetFrameTimeMs());
				break;
			}
		}
		// mouse click
		else if (ev.type == SDL_MOUSEBUTTONDOWN)
		{
			holdingMouseClick = true;
		}
		// mouse release
		else if (ev.type == SDL_MOUSEBUTTONUP)
		{
			holdingMouseClick = false;
		}
		// mouse motion
		else if (ev.type == SDL_MOUSEMOTION)
		{
			//
		}
		// window event
		else if (ev.type == SDL_WINDOWEVENT)
		{
			Puddi::UpdateProjectionMatrixAndViewport();
		}
	}
	//o->RotateX(1.0f / 2000.0f * FpsTracker::GetFrameTimeMs());
	//o->RotateY(1.0f / 2000.0f * FpsTracker::GetFrameTimeMs());
	//cube->RotateZ(1.0f / 2000.0f * FpsTracker::GetFrameTimeMs());

	//rect->RotateZ(1.0f / 2000.0f * FpsTracker::GetFrameTimeMs());

	return 0;
}

void draw()
{
}

int main(int argc, char **argv)
{
	if (int initStatus = Puddi::Init(100.0f) != 0)
		return initStatus;

	init();

	Puddi::RegisterUpdateFunction(update);

	Puddi::RegisterDrawFunction(draw);

	return Puddi::Run();
}
