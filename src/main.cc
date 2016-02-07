#include "Puddi.h"
#include "FpsTracker.h"
#include "LightSource.h"
#include "DrawableObject.h"
#include "Texture.h"
#include "Cube.h"
#include "GlmHeaders.h"
#include "Text.h"
#include "Rectangle.h"
#include "Font.h"
#include "TextString.h"
#include <vector>
#include <iostream>

using namespace puddi;

LightSource *lightSource;
Object *objectContainer;
DrawableObject *cube;
DrawableObject *rect;
TextString *textString;

void init(void)
{
	GLuint texture = Texture::LoadTexture("shrek", "textures/Shrek-and-Yoda.jpg", "bumpmaps/Shrek-and-Yoda_NRM.jpg");
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

	// FONTS
	Font *font = Font::LoadFont("default", "fonts/Bitstream/VeraMono-Bold.ttf");
	font = Font::LoadFont("default", "fonts/SourceCodePro/SourceCodePro-Medium.otf");

	textString = new TextString("bakow", font->monochrome_face);

	// CONTAINERS
	objectContainer = new Object(Puddi::GetRootObject());

	// OBJECTS
	rect = new Rectangle(objectContainer);
	//rect->SetTexture(texture);
	rect->SetTexture(Text::createTextureFromCharacter('1'));
	//rect->SetScaleX(0.5f);
	rect->SetEmissionColor(vec4(0.0f, 0.0f, 0.0f, 0.0f));
	rect->SetEmissive(true);
	//rect->RotateX(M_PI / 2.0f);

	//for (int i = 0; i < 65; ++i)
	//{
		//Rectangle *rect2 = new Rectangle(objectContainer);
		//rect2->SetTexture(Text::createTextureFromCharacter('A' + i));
		//rect2->RotateX(M_PI / 2.0f);
		//rect2->Translate(vec4(i, 0.0f, 0.0f, 0.0f));
	//}

	std::string msg = "int main() { cout << \"hello\" << endl; }";
	for (int i = 0; i < msg.length(); ++i)
	{
		Rectangle *rect2 = new Rectangle(objectContainer);
		rect2->SetTexture(Text::createTextureFromCharacter(msg[i]));
		rect2->RotateX(M_PI / 2.0f);
		rect2->Translate(vec4(i, 0.0f, 0.0f, 0.0f));
	}

	// MIDDLE CUBE
	cube = new Cube(objectContainer);
	//cube->SetPosition(vec4(0.0f, 0.0f, -Puddi::WorldSize * 0.25f, 1.0));
	//cube->SetTexture(texture);
	//cube->SetBumpMapEnabled(false);
	cube->Scale(00000.1f);
	//cube->RotateX(-M_PI / 2.0f);
	//cube->SetEmissionColor(vec4(0.0f, 0.0f, 0.0f, 0.0f));
	//cube->SetEmissive(true);
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
	cube->RotateZ(1.0f / 2000.0f * FpsTracker::GetFrameTimeMs());

	//rect->RotateZ(1.0f / 2000.0f * FpsTracker::GetFrameTimeMs());

	return 0;
}

void draw()
{
    textString->Draw();
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
