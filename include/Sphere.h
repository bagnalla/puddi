#ifndef SPHERE_H
#define SPHERE_H

#include "DrawableObject.h"

namespace puddi
{
  class Sphere : public DrawableObject
  {
  public:
    static void Init();
    Sphere(Object *par);

    static std::vector<glm::vec4> GetSphereVertices();
    static std::vector<glm::vec4> GetSphereNormals();

  private:
    static std::vector<glm::vec4> sphere_vertices;
    static std::vector<glm::vec4> sphere_normals;
    static void createPrototypeVertices();
  };
}

#endif
