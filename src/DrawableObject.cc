#include "DrawableObject.h"
#include "Puddi.h"
#include "RenderGraph.h"
#include "Schematic.h"
#include "Shader.h"
#include "Shadow.h"
#include "TerrainVertexMesh.h"

namespace puddi
{
  // PUBLIC

  DrawableObject::DrawableObject(Object *par) : Object(par)
  {
    init();
  }

  DrawableObject::DrawableObject(Object *par, const VertexMesh &mesh) : Object(par)
  {
    init();
    VertexMesh *vertexMesh = new VertexMesh(mesh);
    this->AddVertexMesh(vertexMesh);
    updateRenderNodes();
  }

  DrawableObject::DrawableObject(Object *par, TerrainVertexMesh *mesh) : Object(par)
  {
    init();
    this->AddVertexMesh(mesh);
    updateRenderNodes();
  }

  DrawableObject::DrawableObject(Object *par, SchematicNode *schematic) : DrawableObject(par)
  {
    BuildFromSchematic(schematic);
  }

  DrawableObject::~DrawableObject()
  {
    for (auto it = vertexMeshes.begin(); it != vertexMeshes.end(); ++it)
      delete *it;
  }

  //  void DrawableObject::Draw() const
  //  {
  //      Shader::SetModel(finalModel);
  //      /*if (cubeMap != 0)
  //          Shader::SetReflectiveCubeMap(reflectiveCubeMap);*/
  //      for (auto it = vertexMeshes.begin(); it != vertexMeshes.end(); ++it)
  //          (*it)->Draw();
  //  };

  void DrawableObject::SendTransformToGPU()
  {
    Shader::SetModel(finalModel);
  }

  // void DrawableObject::AddVertexMesh(VertexMesh *vertexMesh)
  // {
  //   vertexMeshes.push_back(vertexMesh);
  //   vertexMeshes[vertexMeshes.size() - 1]->SetOwner(this);
  //   updateRenderNodes();
  // }

  std::vector<VertexMesh*> DrawableObject::GetVertexMeshes() const
  {
    return this->vertexMeshes;
  }

  void DrawableObject::BuildFromSchematic(SchematicNode *schematic)
  {
    baseTransform = schematic->transform;

    for (auto it = schematic->vertexMeshes.begin(); it != schematic->vertexMeshes.end(); ++it)
      {
	vertexMeshes.push_back(new VertexMesh(*it));
	vertexMeshes[vertexMeshes.size() - 1]->SetOwner(this);
      }

    for (auto it = schematic->children.begin(); it != schematic->children.end(); ++it)
      new DrawableObject(this, *it);

    updateRenderNodes();
  }

  void DrawableObject::SetEmissive(bool e)
  {
    for (auto it = vertexMeshes.begin(); it != vertexMeshes.end(); ++it)
      (*it)->SetEmissive(e);
    updateRenderNodes();
  }

  //void DrawableObject::SetBlend(bool b)
  //{
  //  blend = b;
  //}

  void DrawableObject::SetEmissionColor(const vec4& c)
  {
    for (auto it = vertexMeshes.begin(); it != vertexMeshes.end(); ++it)
      (*it)->SetEmissionColor(c);
    updateRenderNodes();
    for (auto it = children.begin(); it != children.end(); ++it)
      static_cast<DrawableObject*>(*it)->SetEmissionColor(c);
  }

  void DrawableObject::SetMaterial(const Material& m)
  {
    for (auto it = vertexMeshes.begin(); it != vertexMeshes.end(); ++it)
      (*it)->SetMaterial(m);
    updateRenderNodes();
    for (auto it = children.begin(); it != children.end(); ++it)
      static_cast<DrawableObject*>(*it)->SetMaterial(m);
  }

  void DrawableObject::SetTexture(GLuint tex)
  {
    for (auto it = vertexMeshes.begin(); it != vertexMeshes.end(); ++it)
      (*it)->SetTexture(tex);
    updateRenderNodes();
    for (auto it = children.begin(); it != children.end(); ++it)
      static_cast<DrawableObject*>(*it)->SetTexture(tex);
  }

  void DrawableObject::SetBumpMap(GLuint bump)
  {
    for (auto it = vertexMeshes.begin(); it != vertexMeshes.end(); ++it)
      (*it)->SetBumpMap(bump);
    updateRenderNodes();
    for (auto it = children.begin(); it != children.end(); ++it)
      static_cast<DrawableObject*>(*it)->SetBumpMap(bump);
  }

  void DrawableObject::SetCubeMap(GLuint cm)
  {
    for (auto it = vertexMeshes.begin(); it != vertexMeshes.end(); ++it)
      (*it)->SetCubeMap(cm);
    updateRenderNodes();
  }

  void DrawableObject::EnableRender(bool propagate)
  {
    renderEnabled = true;

    for (auto it = vertexMeshes.begin(); it != vertexMeshes.end(); ++it)
      (*it)->EnableRender();

    if (propagate)
      {
	for (auto it = children.begin(); it != children.end(); ++it)
	  static_cast<DrawableObject*>(*it)->EnableRender();
      }

    updateRenderNodes();
  }

  void DrawableObject::DisableRender(bool propagate)
  {
    renderEnabled = false;

    for (auto it = vertexMeshes.begin(); it != vertexMeshes.end(); ++it)
      (*it)->DisableRender();

    if (propagate)
      {
	for (auto it = children.begin(); it != children.end(); ++it)
	  static_cast<DrawableObject*>(*it)->DisableRender();
      }
  }

  void DrawableObject::Cull()
  {
    for (auto it = vertexMeshes.begin(); it != vertexMeshes.end(); ++it)
      (*it)->Cull();

    Object::Cull();
  }

  void DrawableObject::UnCull()
  {
    for (auto it = vertexMeshes.begin(); it != vertexMeshes.end(); ++it)
      (*it)->UnCull();

    Object::UnCull();

    updateRenderNodes();
  }

  bool DrawableObject::GetRenderEnabled() const
  {
    return renderEnabled;
  }

  void DrawableObject::SetBumpMapEnabled(bool b)
  {
    for (auto it = vertexMeshes.begin(); it != vertexMeshes.end(); ++it)
      (*it)->SetBumpMapEnabled(b);
    for (auto it = children.begin(); it != children.end(); ++it)
      static_cast<DrawableObject*>(*it)->SetBumpMapEnabled(b);
  }

  void DrawableObject::SetReflectiveCubeMap(bool b)
  {
    for (auto it = vertexMeshes.begin(); it != vertexMeshes.end(); ++it)
      (*it)->SetReflectiveCubeMap(b);
    for (auto it = children.begin(); it != children.end(); ++it)
      static_cast<DrawableObject*>(*it)->SetReflectiveCubeMap(b);
  }

  void DrawableObject::DisableShadowCasting()
  {
    for (auto it = vertexMeshes.begin(); it != vertexMeshes.end(); ++it)
      Shadow::RemoveFromDepthRenderList((*it)->GetId());
    for (auto it = children.begin(); it != children.end(); ++it)
      static_cast<DrawableObject*>(*it)->DisableShadowCasting();
  };

  // void DrawableObject::SetScene(Scene *scene)
  // {
  //     for (auto it = vertexMeshes.begin(); it != vertexMeshes.end(); ++it)
  //     {
  //         auto vMesh = *it;
  //         // vMesh->SetScene(scene);
  //         vMesh->UpdateRenderNode();
  //     }
  //     for (auto it = children.begin(); it != children.end(); ++it)
  //         static_cast<DrawableObject*>(*it)->SetScene(scene);
  // }

  void DrawableObject::AddVertexMesh(VertexMesh *mesh)
  {
    mesh->SetOwner(this);
    vertexMeshes.push_back(mesh);
    updateRenderNodes();
  }

  void DrawableObject::AddMeshesToScene(Scene *scene) const
  {
    for (auto it = this->vertexMeshes.begin();
	 it != this->vertexMeshes.end(); ++it) {
      scene->AddVertexMesh(*it);
    }
  }

  // PRIVATE

  void DrawableObject::init()
  {
    renderEnabled = true;
    updateRenderNodes();
  }

  void DrawableObject::updateRenderNodes()
  {
    for (auto it = vertexMeshes.begin(); it != vertexMeshes.end(); ++it) {
      // (*it)->UpdateRenderNode();
      engine::VertexMeshChanged((*it)->GetId());
    }
  }
}
