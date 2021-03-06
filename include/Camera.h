#ifndef CAMERA_H
#define CAMERA_H

#include <SDL2/SDL.h>
#include "Object.h"

namespace puddi
{
  class Player;
  class Scene;

  class Camera : public Object
  {
  public:
    // Camera(Object *par);
    Camera(Scene *scene);

    void Update();

    float GetZoomOut() const;
    void SetZoomOut(float z);
    vec4 GetLookDirection() const;
    void SetLookDirection(const vec3& lookDir);
    vec4 GetEyePosition() const;
    void LookAt(const vec4& pos);
    void RotateX(float x);
    void InputEvent(SDL_Event ev);

  private:
    bool holdingW, holdingA, holdingS, holdingD, holdingZ, holdingSpace;
    vec3 lookTheta; // rotation values of the camera
    vec4 lookDirection; // look direction vector of the camera
    mat4 lookRotation;
    float zoomOut; // zoom out value
    Scene *scene;
    vec4 eye_position;

    void move();

    void updateView();
  };
}

#endif
