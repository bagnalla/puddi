#include <algorithm>
#include <iostream>
#include "DrawableObject.h"
#include "Puddi.h"
#include "RenderGraph.h"
#include "Shader.h"
#include "Shadow.h"
#include "TerrainVertexMesh.h"
#include "Texture.h"

namespace puddi
{
  static std::unordered_map<int, RenderNode*> mesh_id_node_map;

  // RENDERGRAPH PUBLIC

  RenderGraph::RenderGraph()
  {
  }

  RenderNode* RenderGraph::_addVertexMesh(VertexMesh *o)
  {
    auto cubeMap = o->GetCubeMap();
    auto texture = o->GetTexture();
    auto material = o->GetMaterial();
    auto textureBumpmap = Texture::GetBumpMapByTexture(texture);
    if (textureBumpmap == 0)
      textureBumpmap = o->GetBumpMap();
    auto cubeBumpmap = Texture::GetBumpMapByCubeMap(cubeMap);

    // non-emissive
    if (!o->GetEmissive()) {
      MaterialNode *materialNode;

      // cube map
      if (cubeMap != 0) {
	// with bump map
	if (cubeBumpmap != 0 && o->GetBumpMapEnabled()) {
	  if (cubeMapBumpNodeMap.find(cubeMap) == cubeMapBumpNodeMap.end())
	    cubeMapBumpNodeMap.emplace(cubeMap, new CubeMapBumpNode(cubeMap, cubeBumpmap));

	  // without material
	  if (material == Material::None()) {
	    auto *cubeMapBumpNode = cubeMapBumpNodeMap[cubeMap];

	    cubeMapBumpNode->meshes.push_back(o);
	    return cubeMapBumpNode;
	  }
	  // with material
	  else {
	    auto _materialNodeMap = &cubeMapBumpNodeMap[cubeMap]->materialNodeMap;

	    if (_materialNodeMap->find(material) == _materialNodeMap->end())
	      _materialNodeMap->emplace(material, new MaterialNode(material));

	    materialNode = (*_materialNodeMap)[material];

	    materialNode->meshes.push_back(o);
	    return materialNode;
	  }
	}
	// without bump map
	else {
	  if (cubeMapNodeMap.find(cubeMap) == cubeMapNodeMap.end())
	    cubeMapNodeMap.emplace(cubeMap, new CubeMapNode(cubeMap));

	  // without material
	  if (material == Material::None()) {
	    auto *cubeMapNode = cubeMapNodeMap[cubeMap];

	    cubeMapNode->meshes.push_back(o);
	    return cubeMapNode;
	  }
	  // with material
	  else {
	    auto _materialNodeMap = &cubeMapNodeMap[cubeMap]->materialNodeMap;

	    if (_materialNodeMap->find(material) == _materialNodeMap->end())
	      _materialNodeMap->emplace(material, new MaterialNode(material));

	    materialNode = (*_materialNodeMap)[material];

	    materialNode->meshes.push_back(o);
	    return materialNode;
	  }
	}
      }
      // no texture, only material
      else if (texture == 0) {
	// with bump map
	if (textureBumpmap != 0) {
	  std::unordered_map<Material, MaterialNode*> *_materialNodeMap;

	  if (materialBumpNodeMap.find(texture) == materialBumpNodeMap.end())
	    materialBumpNodeMap.emplace(texture, new MaterialBumpNode(textureBumpmap));

	  _materialNodeMap = &materialBumpNodeMap[texture]->materialNodeMap;

	  if (_materialNodeMap->find(material) == _materialNodeMap->end())
	    _materialNodeMap->emplace(material, new MaterialNode(material));

	  materialNode = (*_materialNodeMap)[material];
	}
	// without bump map
	else {
	  if (materialNodeMap.find(material) == materialNodeMap.end())
	    materialNodeMap.emplace(material, new MaterialNode(material));

	  materialNode = materialNodeMap[material];
	}
      }
      // no material, texture only
      else if (material == Material::None()) {
	// with bump map
	if (textureBumpmap != 0 && o->GetBumpMapEnabled()) {
	  if (textureBumpNodeMap.find(texture) == textureBumpNodeMap.end())
	    textureBumpNodeMap.emplace(texture, new TextureBumpNode(texture, textureBumpmap));

	  auto *textureBumpNode = textureBumpNodeMap[texture];

	  textureBumpNode->meshes.push_back(o);
	  return textureBumpNode;
	}
	// without bump map
	else {
	  if (textureNodeMap.find(texture) == textureNodeMap.end())
	    textureNodeMap.emplace(texture, new TextureNode(texture));

	  auto *textureNode = textureNodeMap[texture];

	  textureNode->meshes.push_back(o);
	  return textureNode;
	}
      }
      // texture and material
      else {
	std::unordered_map<Material, MaterialNode*> *_materialNodeMap;

	// with bump map
	if (textureBumpmap != 0 && o->GetBumpMapEnabled()) {
	  if (textureBumpNodeMap.find(texture) == textureBumpNodeMap.end())
	    textureBumpNodeMap.emplace(texture, new TextureBumpNode(texture, textureBumpmap));

	  _materialNodeMap = &textureBumpNodeMap[texture]->materialNodeMap;
	}
	// without bump map
	else {
	  if (textureNodeMap.find(texture) == textureNodeMap.end())
	    textureNodeMap.emplace(texture, new TextureNode(texture));

	  _materialNodeMap = &textureNodeMap[texture]->materialNodeMap;
	}

	if (_materialNodeMap->find(material) == _materialNodeMap->end())
	  _materialNodeMap->emplace(material, new MaterialNode(material));

	materialNode = (*_materialNodeMap)[material];
      }

      materialNode->meshes.push_back(o);
      return materialNode;
    }
    // emissive
    else {
      std::cout << "RenderGraph::_addVertexMesh(): adding emissive mesh"
		<< std::endl;

      std::unordered_map<vec4, EmissiveNode*> *_emissiveNodeMap;
      EmissiveNode *emissiveNode;

      // cubemap
      if (cubeMap != 0) {
	if (cubeMapEmissiveNodeMap.find(cubeMap) == cubeMapEmissiveNodeMap.end())
	  cubeMapEmissiveNodeMap.emplace(cubeMap, new CubeMapEmissiveNode(cubeMap));

	auto cubeMapEmissiveNode = cubeMapEmissiveNodeMap[cubeMap];
	cubeMapEmissiveNode->meshes.push_back(o);
	return cubeMapEmissiveNode;
      }
      // texture
      else if (texture != 0) {
	if (textureEmissiveNodeMap.find(texture) == textureEmissiveNodeMap.end())
	  textureEmissiveNodeMap.emplace(texture, new TextureEmissiveNode(texture));

	auto textureEmissiveNode = textureEmissiveNodeMap[texture];
	textureEmissiveNode->meshes.push_back(o);
	return textureEmissiveNode;
      }
      // no texture
      else {
	_emissiveNodeMap = &emissiveNodeMap;
      }

      if (_emissiveNodeMap->find(o->GetEmissionColor()) ==
	  _emissiveNodeMap->end())
	_emissiveNodeMap->emplace(o->GetEmissionColor(),
				  new EmissiveNode(o->GetEmissionColor()));

      emissiveNode = (*_emissiveNodeMap)[o->GetEmissionColor()];

      emissiveNode->meshes.push_back(o);
      return emissiveNode;
    }
  }

  RenderNode* RenderGraph::AddVertexMesh(VertexMesh *o)
  {
    auto node = this->_addVertexMesh(o);
    mesh_id_node_map.emplace(o->GetId(), node);
    return node;
  }

  void RenderGraph::RemoveVertexMesh(int mesh_id)
  {
    auto it = mesh_id_node_map.find(mesh_id);
    if (it != mesh_id_node_map.end()) {
      auto node = it->second;
      mesh_id_node_map.erase(it);
      for (size_t i = 0; i < node->meshes.size(); ++i) {
	if (node->meshes[i]->GetId() == mesh_id) {
	  node->meshes.erase(node->meshes.begin() + i);
	  return;
	}
      }
    }
  }

  void RenderGraph::VertexMeshChanged(int mesh_id)
  {
    std::cout << "RenderGraph::VertexMeshChanged" << std::endl;
    std::cout << "mesh_id: " << mesh_id << std::endl;

    auto it = mesh_id_node_map.find(mesh_id);
    if (it != mesh_id_node_map.end()) {
      auto node = it->second;
      // Find the mesh object.
      VertexMesh *mesh = nullptr;
      for (size_t i = 0; i < node->meshes.size(); ++i) {
	if (node->meshes[i]->GetId() == mesh_id) {
	  mesh = node->meshes[i];
	  break;
	}
      }
      if (mesh) {
	// Remove and re-add the mesh.
	this->RemoveVertexMesh(mesh_id);
	this->AddVertexMesh(mesh);
      }
      else {
	std::cerr <<
	  "RenderGraph::VertexMeshChanged: mesh not found in render node.\n";
      }
    }
  }

  void RenderGraph::AddTerrainVertexMesh(TerrainVertexMesh *o)
  {
    if (!o->GetBumpMapEnabled())
      {
	auto it = std::find(terrainVertexMeshes.begin(),
			    terrainVertexMeshes.end(), o);
	if (it == terrainVertexMeshes.end())
	  terrainVertexMeshes.push_back(o);
      }
    else
      {
	auto it = std::find(terrainVertexMeshesWithBump.begin(),
			    terrainVertexMeshesWithBump.end(), o);
	if (it == terrainVertexMeshesWithBump.end())
	  terrainVertexMeshesWithBump.push_back(o);
      }
  }

  void RenderGraph::RemoveTerrainVertexMesh(int mesh_id)
  {
    // auto it = std::find(terrainVertexMeshes.begin(),
    // 			terrainVertexMeshes.end(), o);
    // if (it != terrainVertexMeshes.end())
    //   terrainVertexMeshes.erase(it);

    // it = std::find(terrainVertexMeshesWithBump.begin(),
    // 		   terrainVertexMeshesWithBump.end(), o);
    // if (it != terrainVertexMeshesWithBump.end())
    //   terrainVertexMeshesWithBump.erase(it);

    for (size_t i = 0; i < terrainVertexMeshes.size(); ++i) {
      if (terrainVertexMeshes[i]->GetId() == mesh_id) {
	terrainVertexMeshes.erase(terrainVertexMeshes.begin() + i);
	break;
      }
    }

    for (size_t i = 0; i < terrainVertexMeshesWithBump.size(); ++i) {
      if (terrainVertexMeshesWithBump[i]->GetId() == mesh_id) {
        terrainVertexMeshesWithBump.erase(terrainVertexMeshesWithBump.begin()
					  + i);
	break;
      }
    }
  }

  void RenderGraph::Render() const
  {
    // std::cout << "RenderGraph::Render()" << std::endl;
    
    // render emissive objects
    Shader::SetProgram("emissive_color");
    for (auto it = emissiveNodeMap.begin();
	 it != emissiveNodeMap.end(); ++it)
      (*it).second->Render();

    // render emissive objects with cubemap
    Shader::SetProgram("emissive_cubemap");
    for (auto it = cubeMapEmissiveNodeMap.begin();
	 it != cubeMapEmissiveNodeMap.end(); ++it)
      (*it).second->Render();

    // render emissive objects with texture
    Shader::SetProgram("emissive_texture");
    for (auto it = textureEmissiveNodeMap.begin();
	 it != textureEmissiveNodeMap.end(); ++it)
      (*it).second->Render();

    // render objects with texture, bump map, material
    Shader::SetProgram("texture_bump");
    for (auto it = textureBumpNodeMap.begin();
	 it != textureBumpNodeMap.end(); ++it)
      (*it).second->Render();

    // render objects with texture, material
    Shader::SetProgram("texture");
    for (auto it = textureNodeMap.begin();
	 it != textureNodeMap.end(); ++it)
      (*it).second->Render();

    // render objects with only material
    Shader::SetProgram("material");
    for (auto it = materialNodeMap.begin();
	 it != materialNodeMap.end(); ++it)
      (*it).second->Render();

    // render objects with material and bumpmap
    Shader::SetProgram("material_bump");
    for (auto it = materialBumpNodeMap.begin();
	 it != materialBumpNodeMap.end(); ++it)
      (*it).second->Render();

    // render objects with cubemap
    Shader::SetProgram("cubemap");
    for (auto it = cubeMapNodeMap.begin();
	 it != cubeMapNodeMap.end(); ++it)
      (*it).second->Render();

    // render objects with cubemap and bumpmap
    Shader::SetProgram("cubemap_bump");
    for (auto it = cubeMapBumpNodeMap.begin();
	 it != cubeMapBumpNodeMap.end(); ++it)
      (*it).second->Render();

    // render terrain without bump maps
    /*Shader::SetProgram("terrain_texture");
      for (auto it = terrainVertexMeshes.begin();
      it != terrainVertexMeshes.end(); ++it)
      (*it)->DrawWithoutBumpMap();*/

    // render terrain with bump maps
    Shader::SetProgram("terrain_texture_bump");
    for (auto it = terrainVertexMeshesWithBump.begin();
	 it != terrainVertexMeshesWithBump.end(); ++it)
      (*it)->DrawWithBumpMap();
  }

  int RenderGraph::Size() const
  {
    return cubeMapNodeMap.size() + cubeMapBumpNodeMap.size() +
      cubeMapEmissiveNodeMap.size() + textureBumpNodeMap.size() +
      textureNodeMap.size() + textureEmissiveNodeMap.size() +
      materialNodeMap.size() + materialBumpNodeMap.size() +
      emissiveNodeMap.size() + terrainVertexMeshes.size() +
      terrainVertexMeshesWithBump.size();

  }
}
