#ifndef VERTEXMESH_H
#define VERTEXMESH_H

#include <string>
#include <unordered_map>
#include <GL/glew.h>
#include "Material.h"

namespace puddi
{
  struct RenderNode;
  class DrawableObject;
  class Scene;

  enum VertexMode { V_POINTS, V_LINES, V_LINE_STRIP, V_LINE_LOOP,
		    V_TRIANGLES, V_TRIANGLE_STRIP, V_TRIANGLE_FAN };

  class VertexMesh
  {
  public:
    // need public default constructor to use it as non-dynamic memory struct
    VertexMesh();

    VertexMesh(DrawableObject *o, const Material& mat,
	       int iOffset, int iCount, VertexMode vmode);

    virtual ~VertexMesh();

    static void AddVertexMeshPrototype(const std::string &name,
				       const Material& mat, int iOffset,
				       int iCount, VertexMode vmode);

    static VertexMesh GetVertexMeshPrototypeByName(const std::string &name);

    virtual void Draw() const;

    virtual void UpdateRenderNode();

    void EnableRender();

    void DisableRender();

    void Cull();

    void UnCull();

    DrawableObject* GetOwner() const;
    void SetOwner(DrawableObject *o);

    Material GetMaterial() const;
    void SetMaterial(const Material& mat);

    GLuint GetTexture() const;
    void SetTexture(GLuint tex);

    GLuint GetBumpMap() const;
    void SetBumpMap(GLuint b);

    GLuint GetCubeMap() const;
    void SetCubeMap(GLuint cMap);

    bool GetReflectiveCubeMap() const;
    void SetReflectiveCubeMap(bool b);

    bool GetBumpMapEnabled() const;
    void SetBumpMapEnabled(bool b);

    bool GetEmissive() const;
    void SetEmissive(bool b);

    vec4 GetEmissionColor() const;
    void SetEmissionColor(const vec4& eColor);

    bool GetRenderEnabled() const;
    void SetRenderEnabled(bool b);

    /* size_t GetRenderGraphIndex() const; */
    /* void SetRenderGraphIndex(size_t i); */
    Scene* GetScene() const;
    void SetScene(Scene *scene);

  private:
    static std::unordered_map<std::string, VertexMesh> vertexMeshPrototypeMap;

    DrawableObject *owner;
    Material material;
    GLuint texture;
    GLuint cubeMap;
    bool reflectiveCubeMap;
    bool bumpMapEnabled;
    GLuint bumpmap;
    bool emissive;
    vec4 emissionColor;
    RenderNode *renderNode;
    bool renderEnabled;
    VertexMode vmode;

    void init();

  protected:
    uint indexOffset;
    uint indexCount;
    /* size_t renderGraphIndex; */
    Scene *scene;
  };
}

#endif
