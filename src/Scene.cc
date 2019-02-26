#include <iostream>
#include "Camera.h"
#include "DrawableObject.h"
#include "RenderGraph.h"
#include "Scene.h"
#include "Shader.h"
#include "VertexMesh.h"

using namespace std;

namespace puddi
{
  static int id_counter;

  Scene::Scene(ProjectionType ptype, bool cam_object)
  {
    this->id = id_counter++;
    this->proj_type = ptype;
    this->depth = 1.0f;
    this->fov = 1.0f;
    if (cam_object) {
      this->camera = new Camera(this);
    }
    this->view = mat4(1.0f);
    this->projection = mat4(1.0f);
    this->render_graph = new RenderGraph();
    this->root = new Object(nullptr);
  }

  Scene::~Scene()
  {
    // for (auto it = this->child_scenes.begin();
    // 	 it != this->child_scenes.end(); ++it) {
    //   delete *it;
    // }
    if (this->camera) {
      delete this->camera;
    }
    delete this->render_graph;
    delete this->root;
  }

  // void Scene::SetProjection(const mat4 &proj)
  // {
  //   this->projection = proj;
  // }

  void Scene::SetView(const mat4 &view)
  {
    this->view = view;
  }

  void Scene::SetCamera(Camera *cam)
  {
    this->camera = cam;
  }

  void Scene::SetDepth(float d)
  {
    this->depth = d;
  }

  void Scene::ForceModelUpdate()
  {
    this->root->GetModelNode()->Update();
  }

  // void Scene::AddChildScene(Scene *child)
  // {
  //   this->child_scenes.push_back(child);
  // }

  // void Scene::RemoveChildScene(int child_id)
  // {
  //   for (size_t i = 0; i < this->child_scenes.size(); ++i) {
  //     if (this->child_scenes[i]->id == child_id) {
  //       this->child_scenes.erase(this->child_scenes.begin() + i);
  //       return;
  //     }
  //   }
  // }

  int Scene::GetId() const
  {
    return this->id;
  }

  mat4 Scene::GetProjection() const
  {
    return this->projection;
  }

  mat4 Scene::GetView() const
  {
    return this->view;
  }

  Camera* Scene::GetCamera() const
  {
    return this->camera;
  }

  RenderGraph* Scene::GetRenderGraph() const
  {
    return this->render_graph;
  }

  Object* Scene::GetRootObject() const
  {
    return root;
  }

  void Scene::ViewportChange(int w, int h)
  {
    std::cout << "Scene::ViewportChange" << std::endl;
    std::cout << "w: " << w << ", h: " << h << ", depth: " <<
      this->depth << std::endl;
    if (this->proj_type == PROJ_PERSPECTIVE) {
      std::cout << "setting perspective projection" << std::endl;
      this->projection = perspective(static_cast<float>(M_PI) / 4.0f,
				     w / static_cast<float>(h), 1.0f,
				     this->depth);
    }
    else {
      std::cout << "setting ortho projection" << std::endl;
      float aspect = w / h;
      this->projection = ortho(aspect, -aspect, -1.0f, 1.0f, -1.0f, 1.0f);
    }
  }

  vec4 Scene::UnprojectPoint(const vec4 &p)
  {
    return this->view * this->projection * p;
  }

  vec2 Scene::UnprojectPointVec2(const vec4 &p)
  {
    vec4 v = this->UnprojectPoint(p);
    return vec2(v.x, v.y);
  }

  bool Scene::IsBehindEye(const vec4 &p)
  {
    // Not sure that this is right.
    return this->UnprojectPoint(p).z < 0;
  }

  // void Scene::AddObject(Object *o)
  // {
  //   this->root->AddChild(o);
  //   // auto drawable = dynamic_cast<DrawableObject*>(o);
  //   // if (drawable) {
  //   //   auto meshes = drawable->GetVertexMeshes();
  //   //   for (auto it = meshes.begin(); it != meshes.end(); ++it) {
  //   // 	this->render_graph->AddVertexMesh(*it);
  //   //   }
  //   // }
  // }

  void Scene::AddVertexMesh(VertexMesh *mesh)
  {
    this->render_graph->AddVertexMesh(mesh);
  }
  
  void Scene::Update()
  {
    // Update camera as a special case.
    this->camera->Update();

    // Update model transforms.
    this->root->GetModelNode()->Update();

    // Run objects' update code.
    this->root->GetUpdateNode()->Update();
  }
  
  void Scene::Draw() const
  {
    // std::cout << "Scene::Draw()" << std::endl;
    Shader::SetProjection(this->projection);
    Shader::SetView(this->view);
    this->render_graph->Render();
  }

  void Scene::VertexMeshChanged(int mesh_id)
  {
    this->render_graph->VertexMeshChanged(mesh_id);
  }

  void Scene::VertexMeshDeleted(int mesh_id)
  {
    this->render_graph->RemoveVertexMesh(mesh_id);
  }

  void Scene::TerrainVertexMeshDeleted(int mesh_id)
  {
    this->render_graph->RemoveTerrainVertexMesh(mesh_id);
  }
}
