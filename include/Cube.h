/**
 * @file Cube.h
 * @author Alexander Bagnall
 * @brief Cube object.
 *
 * Convenience class for objects with a cube mesh.
 */

#ifndef CUBE_H
#define CUBE_H

#include "DrawableObject.h"

namespace puddi
{
  class Cube : public DrawableObject
  {
  public:
    static void Init();
    Cube(Object *par);

    bool ContainsPoint(const vec4& p) const;
    static std::vector<glm::vec4> GetCubeVertices();

  private:
    static std::vector<glm::vec4> cube_vertices;
    static void createPrototypeVertices();
  };
}

#endif
