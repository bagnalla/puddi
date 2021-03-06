#ifndef RENDERGRAPH_H
#define RENDERGRAPH_H

#include <GL/glew.h>
#include <unordered_map>
#include <vector>
#include "Material.h"
#include "RenderNode.h"

namespace puddi
{
  class TerrainVertexMesh;

  class RenderGraph
  {
  public:
    // constructor
    RenderGraph();

    // root node for objects with cube map without bump maps and normal lighting
    std::unordered_map<GLuint, CubeMapNode*> cubeMapNodeMap;

    // root node for objects with cube map with bump maps and normal lighting
    std::unordered_map<GLuint, CubeMapBumpNode*> cubeMapBumpNodeMap;

    // root node for emissive objects with cube map
    std::unordered_map<GLuint, CubeMapEmissiveNode*> cubeMapEmissiveNodeMap;

    // root node for textured objects with bump maps and normal lighting
    std::unordered_map<GLuint, TextureBumpNode*> textureBumpNodeMap;

    // root node for textured objects without bump maps and normal lighting
    std::unordered_map<GLuint, TextureNode*> textureNodeMap;

    // root node for textured emissive objects
    std::unordered_map<GLuint, TextureEmissiveNode*> textureEmissiveNodeMap;

    // root node for non-textured objects with normal lighting
    std::unordered_map<Material, MaterialNode*> materialNodeMap;

    // root node for non-textured objects with normal lighting and bumpmaps
    std::unordered_map<GLuint, MaterialBumpNode*> materialBumpNodeMap;

    // root node for emissive objects
    std::unordered_map<vec4, EmissiveNode*> emissiveNodeMap;

    // list of TERRAIN textured objects without bump maps and normal lighting
    std::vector<TerrainVertexMesh*> terrainVertexMeshes;

    // list of TERRAIN textured objects with bump maps and normal lighting
    std::vector<TerrainVertexMesh*> terrainVertexMeshesWithBump;

    // add an object to the graph
    RenderNode* AddVertexMesh(VertexMesh *o);
    void RemoveVertexMesh(int mesh_id);

    void VertexMeshChanged(int mesh_id);

    void AddTerrainVertexMesh(TerrainVertexMesh *o);
    void RemoveTerrainVertexMesh(int mesh_id);

    // render everything
    void Render() const;

    int Size() const;

  private:
    RenderNode* _addVertexMesh(VertexMesh *o);
  };
}

#endif
