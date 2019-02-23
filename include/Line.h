#ifndef LINE_H
#define LINE_H

#include "DrawableObject.h"

namespace puddi
{
  class Line : public DrawableObject
  {
  public:
    static void Init();
    Line(Object *par);

    // Give endpoints in world space and the line will compute all the
    // transforms.
    void SetWorldPoints(const glm::vec4 &p1, const glm::vec4 &p2);

  private:
    static void createPrototypeVertices();
  };
}

#endif
