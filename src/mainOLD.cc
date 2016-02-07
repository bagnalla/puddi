#include "Puddi.h"
#include "Shader.h"
#include "InitShader.h"
#include "FpsTracker.h"
#include "Rectangle.h"
#include "Cube.h"
#include "Sphere.h"
#include "Skybox.h"
#include "LightSource.h"
#include "Util.h"
#include "UpdateGraph.h"
#include "ModelGraph.h"
#include "RenderGraph.h"
#include "Texture.h"
#include "Shadow.h"
#include "EnvironmentMap.h"
#include "Schematic.h"
#include "VertexMesh.h"
#include "TerrainVertexMesh.h"
#include "HeightMapTerrain.h"
#include <vector>
#include <iostream>

//#include <thread>
//#include <ppl.h>

using namespace puddi;

Sphere *sun;
DrawableObject *player;
LightSource *lightSource;
Object *objectContainer;
std::vector<DrawableObject*> objects;
DrawableObject *middleCube;

Object *groundContainer;

DrawableObject *terrain;
TerrainVertexMesh *terrainMesh;

GLuint colorCubemap;

GLuint tempTexture;

void init(void)
{
	Texture::LoadTexture("shrek", "textures/Shrek-and-Yoda.jpg", "bumpmaps/Shrek-and-Yoda_NRM.jpg");
	Texture::LoadTexture("ben", "textures/ben.jpg", "bumpmaps/ben_NRM.jpg");
	Texture::LoadTexture("stone_wall", "textures/stone_wall.jpg", "bumpmaps/stone_wall_NRM.jpg");
	Texture::LoadTexture("costanza", "textures/costanza.jpg", "bumpmaps/costanza_NRM.jpg");
	Texture::LoadTexture("dowm_furry", "textures/dowm furry.jpg", "bumpmaps/dowm furry_NRM.jpg");
	Texture::LoadTexture("isaac_final_form", "textures/finalformsquare.jpg", "bumpmaps/finalformsquare_NRM.jpg");
	Texture::LoadTexture("bill", "textures/bill.png", "bumpmaps/bill_NRM.jpg");
	Texture::LoadCubeMap("cubemap_test_1", "textures/cubemap_test_1.jpg", "bumpmaps/cubemap_test_1_NRM.jpg");
	Texture::LoadTexture("skybox_2", "textures/skybox2.jpg");
	Texture::LoadCubeMap("skybox_2", "textures/skybox2.jpg");
	//Texture::LoadCubeMapMirrored("skybox_2", "textures/skybox2.jpg");
	Texture::LoadTexture("sand1", "textures/sand1.jpg", "bumpmaps/sand1_NRM.jpg");
	Texture::LoadTexture("grass1", "textures/grass1.jpg", "bumpmaps/grass1_NRM.jpg");
	Texture::LoadTexture("stone1", "textures/stone1.jpg", "bumpmaps/stone1_NRM.jpg");
	Texture::LoadTexture("snow1", "textures/snow1.jpg", "bumpmaps/snow1_NRM.jpg");

	if (Schematic::InitSchematic("models/Millennium_Falcon.obj", "millennium_falcon") < 0)
		std::cerr << "error loading Millennium_Falcon model\n";
	if (Schematic::InitSchematic("models/R2-D2.obj", "r2d2") < 0)
		std::cerr << "error loading R2-D2. model\n";
	if (Schematic::InitSchematic("models/C-3PO_v2.obj", "c3po") < 0)
		std::cerr << "error loading C-3PO_v2 model\n";
	if (Schematic::InitSchematic("models/Stormtrooper.obj", "stormtrooper") < 0)
		std::cerr << "error loading Stormtrooper model\n";
	if (Schematic::InitSchematic("models/Darth_Sidious.obj", "darth_sidious") < 0)
		std::cerr << "error loading Darth_Sidious model\n";
	if (Schematic::InitSchematic("models/tie-intercept.obj", "tie_fighter") < 0)
		std::cerr << "error loading tie-intercept model\n";

	terrainMesh = new TerrainVertexMesh(HeightMapTerrain::CreateTerrainMeshFromFile("heightmaps/terrain1_height.jpg", 25.0f, 25.0f, 0.25f));
	//terrainMesh = new TerrainVertexMesh(HeightMapTerrain::CreateTerrainMeshFromFile("heightmaps/west_norway.png", 50.0f, 50.0f, 0.25f));
	//terrainMesh = new TerrainVertexMesh(HeightMapTerrain::CreateTerrainMeshFromFile("textures/bill.png", 25.0f, 25.0f, 0.1f));
	terrainMesh->SetTexture1(Texture::GetTextureByName("sand1"));
	terrainMesh->SetTexture2(Texture::GetTextureByName("grass1"));
	terrainMesh->SetTexture3(Texture::GetTextureByName("stone1"));
	terrainMesh->SetTexture4(Texture::GetTextureByName("snow1"));
	terrainMesh->SetTextureHeights(vec4(0.0f, 0.3f, 0.5f, 0.9f));

	//Puddi::SendVertexDataToGPU();

	//auto v = Shader::Vertices;
	//auto n = Shader::Normals;
	//auto s = Shader::Tangents;
	//auto t = Shader::Binormals;
	//auto tx = Shader::TextureCoordinates;
	//auto vi = Shader::VertexIndices;

	glClearColor(0.5, 0.5, 1.0, 1.0);

	Puddi::MainCamera->SetPosition(vec4(0.0f, -25.0f, 0.0f, 1.0f));
	Puddi::MainCamera->LookAt(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	Puddi::MainCamera->SetZoomOut(10.0f);

	lightSource = LightSource::ObtainLightSource();
	lightSource->ambient = vec4(1.0, 1.0, 1.0, 1.0);
	lightSource->diffuse = vec4(1.0, 1.0, 1.0, 1.0);
	lightSource->specular = vec4(1.0, 1.0, 1.0, 1.0);
	//lightSource->position = vec4(-Puddi::WorldSize / 2.0f, -Puddi::WorldSize / 2.0f, Puddi::WorldSize / 2.0f, 0.0f);
	//lightSource->position = vec4(0.0f, 0.0f, 10.0f, 1.0f);
	lightSource->position = vec4(-1.0f, -1.0f, 1.0f, 0.0f);
	lightSource->UpdateMatrix();
	LightSource::UpdateLightSourceMatricesInShaders();

	// CONTAINERS
	objectContainer = new Object(Puddi::GetRootObject(), false, true);
	groundContainer = new Object(Puddi::GetRootObject());
	groundContainer->Translate(vec4(0.0f, 0.0f, -Puddi::WorldSize / 2.0f, 0.0f));

	char *textureNames[] = { "shrek", "dowm_furry", "isaac_final_form", "costanza", "bill" };

	// MANY CUBES
	int range = Puddi::WorldSize * 0.75f;
	for (int i = 0; i < 500; ++i)
	{
		Cube *cube = new Cube(objectContainer);
		cube->SetPosition(vec4(rand() % range - range / 2.0f, rand() % range - range / 2.0f, rand() % range - range / 2.0f, 1.0f));
		//cube->SetTexture(Texture::GetTextureByName("isaac_final_form"));
		cube->SetTexture(Texture::GetTextureByName(textureNames[rand() % 5]));
		cube->SetMaterial(Material::Plastic(vec4(0.3f, 0.3f, 0.3f, 1.0f)));
		//cube->SetMaterial(Material::Plastic(vec4(1.0f, 0.0f, 0.0f, 1.0f)));
		cube->Scale(5.0f);
		objects.push_back(cube);
		//cube->Cull();
	}

	// R2D2S LOL
	for (int i = 0; i < 100; ++i)
	{
		DrawableObject *o = new DrawableObject(objectContainer, Schematic::GetSchematicByName("r2d2"));
		o->SetPosition(vec4(rand() % range - range / 2.0f, rand() % range - range / 2.0f, rand() % range - range / 2.0f, 1.0f));
		o->RotateX(-M_PI / 2.0f);
		//cube->SetTexture(Texture::GetTextureByName("shrek"));
		//cube->SetTexture(Texture::GetTextureByName(textureNames[rand() % 4]));
		//cube->SetMaterial(Material::Plastic(vec4(0.3f, 0.3f, 0.3f, 1.0f)));
		//cube->SetMaterial(Material::Plastic(vec4(1.0f, 0.0f, 0.0f, 1.0f)));
		o->Scale(5.0f);
		objects.push_back(o);
		//cube->Cull();
	}

	// C3POS LMAO
	for (int i = 0; i < 100; ++i)
	{
		DrawableObject *o = new DrawableObject(objectContainer, Schematic::GetSchematicByName("c3po"));
		o->SetPosition(vec4(rand() % range - range / 2.0f, rand() % range - range / 2.0f, rand() % range - range / 2.0f, 1.0f));
		o->RotateX(-M_PI / 2.0f);
		//cube->SetTexture(Texture::GetTextureByName("shrek"));
		//cube->SetTexture(Texture::GetTextureByName(textureNames[rand() % 4]));
		//cube->SetMaterial(Material::Plastic(vec4(0.3f, 0.3f, 0.3f, 1.0f)));
		//cube->SetMaterial(Material::Plastic(vec4(1.0f, 0.0f, 0.0f, 1.0f)));
		o->Scale(5.0f);
		objects.push_back(o);
		//cube->Cull();
	}

	// STORMTROOPERS
	for (int i = 0; i < 100; ++i)
	{
		DrawableObject *o = new DrawableObject(objectContainer, Schematic::GetSchematicByName("stormtrooper"));
		o->SetPosition(vec4(rand() % range - range / 2.0f, rand() % range - range / 2.0f, rand() % range - range / 2.0f, 1.0f));
		o->RotateX(-M_PI / 2.0f);
		o->Scale(5.0f);
		objects.push_back(o);
	}

	// DARTH INSNIDIOUS
	for (int i = 0; i < 100; ++i)
	{
		DrawableObject *o = new DrawableObject(objectContainer, Schematic::GetSchematicByName("darth_sidious"));
		o->SetPosition(vec4(rand() % range - range / 2.0f, rand() % range - range / 2.0f, rand() % range - range / 2.0f, 1.0f));
		o->RotateX(-M_PI / 2.0f);
		o->Scale(5.0f);
		objects.push_back(o);
	}

	// MIDDLE CUBE
	//middleCube = new Cube(objectContainer);
	//middleCube = new DrawableObject(objectContainer, Schematic::GetSchematicByName("tie_fighter"));
	middleCube = new DrawableObject(objectContainer, Schematic::GetSchematicByName("millennium_falcon"));
	//middleCube = new DrawableObject(cubeContainer, Schematic::GetSchematicByName("r2d2"));
	//middleCube = new DrawableObject(cubeContainer, Schematic::GetSchematicByName("torchic"));
	//Sphere *cube = new Sphere(cubeContainer);
	middleCube->SetPosition(vec4(0.0f, 0.0f, -Puddi::WorldSize * 0.25f, 1.0));
	//middleCube->SetTexture(Shadow::depthBuffer);
	//middleCube->SetTexture(Texture::GetTextureByName("bill"));
	//cube->SetCubeMap(Texture::GetCubeMapByName("cubemap_test_1"));
	//middleCube->SetEmissive(true);
	//middleCube->SetMaterial(Material::Plastic(vec4(0.3f, 0.3f, 0.3f, 1.0f)));
	//cube->SetMaterial(Material::Plastic(vec4(1.0f, 1.0f, 1.0f, 0.0f)));
	//middleCube->Scale(10.0f);
	middleCube->Scale(0.1f);
	//middleCube->Scale(0.01f);
	middleCube->RotateX(-M_PI / 2.0f);
	//middleCube->SetBumpMapEnabled(false);
	//cubes.push_back(cube);

	// SUN
	//sun = new Cube(rootObject);
	sun = new Sphere(Puddi::GetRootObject());
	sun->SetPosition(lightSource->position);
	sun->SetEmissionColor(vec4(1.0f, 1.0f, 0.0f, 1.0f));
	sun->SetEmissive(true);
	sun->Scale(0.5f);
	//cubes.push_back(cube);

	//// CAMERA CUBE
	//cameraCube = new Cube(camera);
	//cameraCube->SetMaterial(Material::Chrome());
	//cameraCube->SetPosition(vec4(0.0, 5.0f, 0.0, 1.0));
	////cameraCube->SetEmissionColor(vec4(1.0f, 1.0f, 0.0f, 1.0f));
	////cameraCube->SetEmissive(true);
	//cameraCube->Scale(1.0f);

	// PLAYER
	player = new Sphere(Puddi::MainCamera);
	//player = new DrawableObject(camera, Schematic::GetSchematicByName("torchic"));
	player->SetMaterial(Material::Chrome());
	//player->SetTexture(Texture::GetTextureByName("shrek"));
	//player->SetMaterial(Material::Plastic(vec4(0.25f, 0.25f, 0.25f, 1.0f)));
	//player->SetMaterial(Material::Plastic(vec4(0.0f, 0.0f, 0.0f, 1.0f)));
	colorCubemap = EnvironmentMap::GenerateCubeMap();
	player->SetCubeMap(colorCubemap);
	player->SetReflectiveCubeMap(true);
	player->Scale(1.0f);

	// GIANT SKY CUBE
	Skybox *skybox = new Skybox(Puddi::MainCamera);
	//skybox->SetTexture(Texture::GetTextureByName("shrek"));
	skybox->SetCubeMap(Texture::GetCubeMapByName("skybox_2"));
	//skybox->SetTexture(Texture::GetTextureByName("skybox_2"));
	//skybox->SetCubeMap(Texture::GetCubeMapByName("cubemap_test_1"));
	skybox->SetEmissive(true);
	//skybox->SetMaterial(Material::WhiteRubber());
	skybox->SetScale(Puddi::ViewDistance);
	//skybox->RotateX(-M_PI / 2.0f);
	//skybox->RotateZ(M_PI);
	//skybox->DisableShadowCasting();
	//skybox->AddIgnoreParentModelFlag(IGNORE_PARENT_ROTATIONX | IGNORE_PARENT_ROTATIONY | IGNORE_PARENT_ROTATIONZ);

	//// FLOOR
	//float floorScale = Puddi::WorldSize / 10.0f;
	//for (int i = 0; i < Puddi::WorldSize / floorScale; ++i)
	//{
	//	for (int j = 0; j < Puddi::WorldSize / floorScale; ++j)
	//	{
	//		Rectangle *rect = new Rectangle(groundContainer);
	//		rect->SetPosition(vec4(-Puddi::WorldSize / 2.0f + floorScale / 2.0f + i * floorScale, -Puddi::WorldSize / 2.0f + floorScale / 2.0f + j * floorScale, 0.0f, 1.0));
	//		rect->SetTexture(Texture::GetTextureByName("stone_wall"));
	//		rect->SetMaterial(Material::Plastic(vec4(0.0f, 0.0f, 0.0f, 1.0f)));
	//		rect->Scale(floorScale);
	//	}
	//}

	////// CEILING
	////for (int i = 0; i < Puddi::WorldSize / floorScale; ++i)
	////{
	////	for (int j = 0; j < Puddi::WorldSize / floorScale; ++j)
	////	{
	////		Rectangle *rect = new Rectangle(groundContainer);
	////		rect->SetPosition(vec4(-Puddi::WorldSize / 2.0f + floorScale / 2.0f + i * floorScale, -Puddi::WorldSize / 2.0f + floorScale / 2.0f + j * floorScale, Puddi::WorldSize, 1.0));
	////		rect->SetTexture(Texture::GetTextureByName("stone_wall"));
	////		rect->SetMaterial(Material::Plastic(vec4(0.0f, 0.0f, 0.0f, 1.0f)));
	////		rect->Scale(floorScale);
	////		rect->SetRotationX(M_PI);
	////	}
	////}

	////// WALLS
	////// SOUTH
	////Object *southWall = new Object(rootObject);
	////for (int i = 0; i < Puddi::WorldSize / floorScale; ++i)
	////{
	////	for (int j = 0; j < Puddi::WorldSize / floorScale; ++j)
	////	{
	////		Rectangle *rect = new Rectangle(southWall);
	////		rect->SetPosition(vec4(-Puddi::WorldSize / 2.0f + floorScale / 2.0f + i * floorScale, -Puddi::WorldSize / 2.0f + floorScale / 2.0f + j * floorScale, 0.0f, 1.0));
	////		rect->SetTexture(Texture::GetTextureByName("stone_wall"));
	////		rect->SetMaterial(Material::Plastic(vec4(0.0f, 0.0f, 0.0f, 1.0f)));
	////		rect->Scale(floorScale);
	////	}
	////}
	////southWall->SetRotationX(M_PI / 2.0);
	////southWall->Translate(vec4(0.0, -Puddi::WorldSize / 2.0f, 0.0, 0.0));

	//// NORTH
	//Object *northWall = new Object(rootObject);
	//for (int i = 0; i < Puddi::WorldSize / floorScale; ++i)
	//{
	//	for (int j = 0; j < Puddi::WorldSize / floorScale; ++j)
	//	{
	//		Rectangle *rect = new Rectangle(northWall);
	//		rect->SetPosition(vec4(-Puddi::WorldSize / 2.0f + floorScale / 2.0f + i * floorScale, -Puddi::WorldSize / 2.0f + floorScale / 2.0f + j * floorScale, 0.0f, 1.0));
	//		rect->SetTexture(Texture::GetTextureByName("stone_wall"));
	//		rect->SetMaterial(Material::Plastic(vec4(0.0f, 0.0f, 0.0f, 1.0f)));
	//		rect->Scale(floorScale);
	//	}
	//}
	//northWall->SetRotationX(-M_PI / 2.0);
	//northWall->Translate(vec4(0.0, Puddi::WorldSize / 2.0f, 0.0, 0.0));

	////// WEST
	////Object *westWall = new Object(rootObject);
	////for (int i = 0; i < Puddi::WorldSize / floorScale; ++i)
	////{
	////	for (int j = 0; j < Puddi::WorldSize / floorScale; ++j)
	////	{
	////		Rectangle *rect = new Rectangle(westWall);
	////		rect->SetPosition(vec4(-Puddi::WorldSize / 2.0f + floorScale / 2.0f + i * floorScale, -Puddi::WorldSize / 2.0f + floorScale / 2.0f + j * floorScale, 0.0f, 1.0));
	////		rect->SetTexture(Texture::GetTextureByName("stone_wall"));
	////		rect->SetMaterial(Material::Plastic(vec4(0.0f, 0.0f, 0.0f, 1.0f)));
	////		rect->Scale(floorScale);
	////	}
	////}
	////westWall->SetRotationX(-M_PI / 2.0);
	////westWall->SetRotationZ(-M_PI / 2.0);
	////westWall->Translate(vec4(-Puddi::WorldSize / 2.0f, 0.0, 0.0, 0.0));

	//// EAST
	//Object *eastWall = new Object(rootObject);
	//for (int i = 0; i < Puddi::WorldSize / floorScale; ++i)
	//{
	//	for (int j = 0; j < Puddi::WorldSize / floorScale; ++j)
	//	{
	//		Rectangle *rect = new Rectangle(eastWall);
	//		rect->SetPosition(vec4(-Puddi::WorldSize / 2.0f + floorScale / 2.0f + i * floorScale, -Puddi::WorldSize / 2.0f + floorScale / 2.0f + j * floorScale, 0.0f, 1.0));
	//		rect->SetTexture(Texture::GetTextureByName("stone_wall"));
	//		rect->SetMaterial(Material::Plastic(vec4(0.0f, 0.0f, 0.0f, 1.0f)));
	//		rect->Scale(floorScale);
	//	}
	//}
	//eastWall->SetRotationX(-M_PI / 2.0);
	//eastWall->SetRotationZ(M_PI / 2.0);
	//eastWall->Translate(vec4(Puddi::WorldSize / 2.0f, 0.0, 0.0, 0.0));

	terrain = new DrawableObject(Puddi::GetRootObject());
	terrain->AddVertexMesh(terrainMesh);
	terrain->SetScale(10.0f);
	terrain->Translate(vec4(-2000.0f, -2000.0f, -2000.0f, 0.0f));

	// overwrite z range to give better shadow precision
	Shadow::SetZRange(100.0f, Puddi::ViewDistance);

	// ENABLE SHADOWS
	//Shadow::RenderShadowCubeMap(vec3(lightSource->position), sun);
	//Puddi::MainRenderGraph->SetShadowMode(SHADOW_MODE_OMNI);
	
	//Shadow::RenderShadowOrthoMap(vec3(lightSource->position));
	//Shadow::SetMode(SHADOW_MODE_UNI);
	//Shadow::SetResolution(SHADOW_RESOLUTION_EXTRA_HIGH);
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
				middleCube->RotateX(-0.005f * FpsTracker::GetFrameTimeMs());
				break;
			case SDLK_2:
				middleCube->RotateX(0.005f * FpsTracker::GetFrameTimeMs());
				break;
			case SDLK_3:
				middleCube->RotateY(-0.005f * FpsTracker::GetFrameTimeMs());
				break;
			case SDLK_4:
				middleCube->RotateY(0.005f * FpsTracker::GetFrameTimeMs());
				break;
			case SDLK_5:
				middleCube->RotateZ(-0.005f * FpsTracker::GetFrameTimeMs());
				break;
			case SDLK_6:
				middleCube->RotateZ(0.005f * FpsTracker::GetFrameTimeMs());
				break;
			// MOVE LIGHT SOURCE
			case SDLK_UP:
				sun->Translate(vec4(0.0f, 0.05f, 0.0f, 0.0f) * static_cast<float>(FpsTracker::GetFrameTimeMs()));
				lightSource->position = sun->GetPosition();
				lightSource->UpdateMatrix();
				LightSource::UpdateLightSourceMatricesInShaders();
				break;
			case SDLK_DOWN:
				sun->Translate(vec4(0.0f, -0.05f, 0.0f, 0.0f) * static_cast<float>(FpsTracker::GetFrameTimeMs()));
				lightSource->position = sun->GetPosition();
				lightSource->UpdateMatrix();
				LightSource::UpdateLightSourceMatricesInShaders();
				break;
			case SDLK_LEFT:
				sun->Translate(vec4(-0.05f, 0.0f, 0.0f, 0.0f) * static_cast<float>(FpsTracker::GetFrameTimeMs()));
				lightSource->position = sun->GetPosition();
				lightSource->UpdateMatrix();
				LightSource::UpdateLightSourceMatricesInShaders();
				break;
			case SDLK_RIGHT:
				sun->Translate(vec4(0.05f, 0.0f, 0.0f, 0.0f) * static_cast<float>(FpsTracker::GetFrameTimeMs()));
				lightSource->position = sun->GetPosition();
				lightSource->UpdateMatrix();
				LightSource::UpdateLightSourceMatricesInShaders();
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

	if (holdingMouseClick)
	{
		//
	}

	// update stuff

	/*cubeContainer->RotateX(static_cast<float>(rand()) / RAND_MAX / 10000.0f * FpsTracker::GetFrameTimeMs());
	cubeContainer->RotateY(static_cast<float>(rand()) / RAND_MAX / 10000.0f * FpsTracker::GetFrameTimeMs());
	cubeContainer->RotateZ(static_cast<float>(rand()) / RAND_MAX / 10000.0f * FpsTracker::GetFrameTimeMs());*/
	//cubeContainer->RotateX(1.0f / 10000.0f * FpsTracker::GetFrameTimeMs());
	//cubeContainer->RotateY(1.0f / 10000.0f * FpsTracker::GetFrameTimeMs());
	//objectContainer->RotateZ(1.0f / 10000.0f * FpsTracker::GetFrameTimeMs());

	//groundContainer->RotateX(1.0f / 10000.0f * FpsTracker::GetFrameTimeMs());

	//for (auto it = cubes.begin(); it != cubes.end(); ++it)
	/*concurrency::parallel_for_each(begin(objects), end(objects), [&](Object *o)
	{
		o->RotateX(static_cast<float>(rand()) / RAND_MAX / 1000.0f * FpsTracker::GetFrameTimeMs());
		o->RotateY(static_cast<float>(rand()) / RAND_MAX / 1000.0f * FpsTracker::GetFrameTimeMs());
		o->RotateZ(static_cast<float>(rand()) / RAND_MAX / 1000.0f * FpsTracker::GetFrameTimeMs());
	});*/
	//concurrency::parallel_for_each(begin(objects), end(objects), [&](Object *o)
	//{
		////o->RotateX(1.0f / 2000.0f * FpsTracker::GetFrameTimeMs());
		////o->RotateY(1.0f / 2000.0f * FpsTracker::GetFrameTimeMs());
		//o->RotateZ(1.0f / 2000.0f * FpsTracker::GetFrameTimeMs());
	//}, concurrency::static_partitioner());
	
	for (auto it = objects.begin(); it != objects.end(); ++it)
	{
		//o->RotateX(1.0f / 2000.0f * FpsTracker::GetFrameTimeMs());
		//o->RotateY(1.0f / 2000.0f * FpsTracker::GetFrameTimeMs());
		(*it)->RotateZ(1.0f / 2000.0f * FpsTracker::GetFrameTimeMs());
	}

	//sun->SetPosition(camera->GetPosition() - camera->GetLookDirection() * 10.0f);
	//cameraCube->SetPosition(camera->GetPosition() - camera->GetLookDirection() * 10.0f);
	//camera->Update();

	return 0;
}

void draw()
{
	EnvironmentMap::Render(colorCubemap, vec3(Puddi::MainCamera->GetPosition()), player, 1.0f, Puddi::ViewDistance);
	//Shadow::RenderShadowCubeMap(vec3(lightSource->position), sun);
	//Shadow::RenderShadowOrthoMap(vec3(lightSource->position));

	//Puddi::UpdateProjectionMatrixAndViewport();

	// draw stuff
	//Puddi::Draw();
}

int main(int argc, char **argv)
{
	if (int initStatus = Puddi::Init(3000.0f) != 0)
		return initStatus;

	init();
	
	Puddi::RegisterUpdateFunction(update);
	
	Puddi::RegisterPreDrawFunction(draw);
	
	return Puddi::Run();
}
