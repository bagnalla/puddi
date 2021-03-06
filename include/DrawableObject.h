/**
 * @file DrawableObject.h
 * @author Alexander Bagnall
 * @brief Base class for objects that are composed of drawable meshes.
 *
 * Inherits from Object. Some properties of meshes are settable via
 * an object of this type and they are applied to all meshes under
 * the domain of the object (either directly belonging to the object
 * or to a child under the object)
 */

#ifndef DRAWABLEOBJECT_H
#define DRAWABLEOBJECT_H

#include "Object.h"
#include "VertexMesh.h"

namespace puddi
{
  class TerrainVertexMesh;
  class VertexMesh;
  struct SchematicNode;

  class DrawableObject : public Object
  {
  public:
    DrawableObject(Object *par);

    DrawableObject(Object *par, const VertexMesh &mesh);

    DrawableObject(Object *par, TerrainVertexMesh *mesh);

    DrawableObject(Object *par, SchematicNode *schematic);

    ~DrawableObject();

    //virtual void Draw() const;

    virtual void SendTransformToGPU();

    void BuildFromSchematic(SchematicNode *schematic);

    void SetEmissive(bool e);

    virtual void SetEmissionColor(const vec4& color);

    //void SetBlend(bool b);

    virtual void SetMaterial(const Material& m);

    void SetTexture(GLuint tex);

    void SetBumpMap(GLuint bump);

    void SetCubeMap (GLuint cm);

    void EnableRender(bool propagate = true);

    void DisableRender(bool propagate = true);

    void Cull();

    void UnCull();

    bool GetRenderEnabled() const;

    void SetBumpMapEnabled(bool b);

    void SetReflectiveCubeMap(bool b);

    void DisableShadowCasting();

    /* void SetScene(Scene *scene); */

    void AddVertexMesh(VertexMesh *mesh);
    std::vector<VertexMesh*> GetVertexMeshes() const;
    void AddMeshesToScene(Scene *scene) const;

  private:
    bool renderEnabled;

  protected:
    std::vector<VertexMesh*> vertexMeshes;

    void init();

    void updateRenderNodes();
  };
}

#endif
