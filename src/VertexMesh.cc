#include <algorithm>
#include <iostream>
#include "DrawableObject.h"
#include "Puddi.h"
#include "RenderGraph.h"
#include "Shader.h"
#include "Shadow.h"

namespace puddi
{
  static int id_counter = 0;

  // PUBLIC

  VertexMesh::VertexMesh()
  {
    init();
  }

  VertexMesh::VertexMesh(DrawableObject *o, const Material& mat,
			 int iOffset, int iCount, VertexMode vmode)
  {
    this->id = id_counter++;
    this->init();
    this->owner = o;
    this->material = mat;
    this->indexOffset = iOffset;
    this->indexCount = iCount;
    this->vmode = vmode;
  }

  VertexMesh::~VertexMesh()
  {
    //std::cout << "in VertexMesh destructor\n";
    // if (renderNode != NULL) {
    //   auto it = std::find(renderNode->meshes.begin(),
    // 			  renderNode->meshes.end(), this);
    //   if (it != renderNode->meshes.end())
    // 	renderNode->meshes.erase(std::find(renderNode->meshes.begin(),
    // 					   renderNode->meshes.end(), this));
    //   else
    // 	std::cerr << "in VerteshMesh destructor: attempted to remove self from rendernode that didnt know about me. this should never happen\n";
    // }

    // Shadow::RemoveFromDepthRenderList(this);

    engine::VertexMeshDeleted(this->id);
  }

  void VertexMesh::AddVertexMeshPrototype(const std::string &name,
					  const Material& mat, int iOffset,
					  int iCount, VertexMode vmode)
  {
    auto it = vertexMeshPrototypeMap.find(name);
    if (it == vertexMeshPrototypeMap.end())
      vertexMeshPrototypeMap.emplace(name,
				     VertexMesh(NULL, mat, iOffset,
						iCount, vmode));
    else
      std::cerr << "vertex mesh already exists with name: " <<
	name << std::endl;
  }

  VertexMesh VertexMesh::GetVertexMeshPrototypeByName(const std::string &name)
  {
    auto it = vertexMeshPrototypeMap.find(name);
    if (it != vertexMeshPrototypeMap.end())
      return vertexMeshPrototypeMap[name];
    else
      {
	std::cerr << "vertex mesh doesn't exist with name: " <<
	  name << std::endl;
	return VertexMesh(NULL, Material::None(), 0, 0, V_POINTS);
      }
  }

  void VertexMesh::Draw() const
  {
    // std::cout << "vmesh drawing" << std::endl;
    this->owner->SendTransformToGPU();

    if (this->cubeMap != 0)
      Shader::SetReflectiveCubeMap(this->reflectiveCubeMap);

    glDrawElements(this->vmode, this->indexCount, GL_UNSIGNED_INT,
		   reinterpret_cast<void*>(this->indexOffset * sizeof(uint)));
  }

  // void VertexMesh::UpdateRenderNode()
  // {
  //   if (renderNode != NULL) {
  //     auto it = std::find(renderNode->meshes.begin(),
  // 			  renderNode->meshes.end(), this);
  //     if (it != renderNode->meshes.end())
  // 	renderNode->meshes.erase(std::find(renderNode->meshes.begin(),
  // 					   renderNode->meshes.end(), this));
  //     else
  // 	std::cerr << "in VerteshMesh::UpdateRenderNode(): attempted to remove self from rendernode that didnt know about me. this should never happen\n";
  //   }

  //   // renderNode = engine::GetRenderGraph(renderGraphIndex)->AddVertexMesh(this);
  //   renderNode = this->scene->GetRenderGraph()->AddVertexMesh(this);

  //   Shadow::AddToDepthRenderList(this);
  // }

  void VertexMesh::EnableRender()
  {
    renderEnabled = true;
  }

  void VertexMesh::DisableRender()
  {
    renderEnabled = false;
  }

  void VertexMesh::Cull()
  {
    if (renderNode != NULL)
      renderNode->meshes.erase(std::find(renderNode->meshes.begin(),
					 renderNode->meshes.end(), this));
    renderNode = NULL;

    Shadow::RemoveFromDepthRenderList(this->id);
  }

  void VertexMesh::UnCull()
  {
    Shadow::AddToDepthRenderList(this);
    engine::VertexMeshChanged(this->id);
    // UpdateRenderNode();
  }

  DrawableObject* VertexMesh::GetOwner() const
  {
    return owner;
  }
  void VertexMesh::SetOwner(DrawableObject *o)
  {
    owner = o;
  }

  int VertexMesh::GetId() const
  {
    return this->id;
  }

  Material VertexMesh::GetMaterial() const
  {
    return material;
  }
  void VertexMesh::SetMaterial(const Material& mat)
  {
    material = mat;
  }

  GLuint VertexMesh::GetTexture() const
  {
    return texture;
  }
  void VertexMesh::SetTexture(GLuint tex)
  {
    texture = tex;
  }

  GLuint VertexMesh::GetBumpMap() const
  {
    return bumpmap;
  }
  void VertexMesh::SetBumpMap(GLuint b)
  {
    bumpmap = b;
  }

  GLuint VertexMesh::GetCubeMap() const
  {
    return cubeMap;
  }
  void VertexMesh::SetCubeMap(GLuint cMap)
  {
    cubeMap = cMap;
  }

  bool VertexMesh::GetReflectiveCubeMap() const
  {
    return reflectiveCubeMap;
  }
  void VertexMesh::SetReflectiveCubeMap(bool b)
  {
    reflectiveCubeMap = b;
  }

  bool VertexMesh::GetBumpMapEnabled() const
  {
    return bumpMapEnabled;
  }
  void VertexMesh::SetBumpMapEnabled(bool b)
  {
    bumpMapEnabled = b;
  }

  bool VertexMesh::GetEmissive() const
  {
    return emissive;
  }
  void VertexMesh::SetEmissive(bool b)
  {
    emissive = b;
  }

  vec4 VertexMesh::GetEmissionColor() const
  {
    return emissionColor;
  }
  void VertexMesh::SetEmissionColor(const vec4& eColor)
  {
    emissionColor = eColor;
  }

  bool VertexMesh::GetRenderEnabled() const
  {
    return renderEnabled;
  }
  void VertexMesh::SetRenderEnabled(bool b)
  {
    renderEnabled = b;
  }

  // size_t VertexMesh::GetRenderGraphIndex() const
  // {
  //   return renderGraphIndex;
  // }
  // void VertexMesh::SetRenderGraphIndex(size_t i)
  // {
  //   renderGraphIndex = i;
  // }
  // Scene* VertexMesh::GetScene() const
  // {
  //   return this->scene;
  // }
  // void VertexMesh::SetScene(Scene *scene)
  // {
  //   this->scene = scene;
  // }

  // PRIVATE

  std::unordered_map<std::string, VertexMesh>
  VertexMesh::vertexMeshPrototypeMap;

  void VertexMesh::init()
  {
    owner = nullptr;
    material = Material::None();
    texture = 0;
    bumpmap = 0;
    cubeMap = 0;
    reflectiveCubeMap = false;
    bumpMapEnabled = true;
    emissive = false;
    emissionColor = vec4();
    renderNode = nullptr;
    renderEnabled = true;
    indexOffset = 0;
    indexCount = 0;
    this->vmode = V_POINTS;
  }
}
