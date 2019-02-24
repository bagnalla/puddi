#include "Camera.h"
#include "FpsTracker.h"
#include "Puddi.h"
#include "Scene.h"
#include "Shader.h"
#include "Util.h"

namespace puddi
{
  // PUBLIC

  // Camera::Camera(Object *par) : Object(par)
  Camera::Camera(Scene *scene) : Object(nullptr)
  {
    this->scene = scene;
    this->lookTheta = vec3();
    this->lookDirection = vec4(0.0f, 1.0f, 0.0f, 0.0f);
    this->lookRotation = mat4(1.0f);
    this->zoomOut = 0.0;
    holdingW = holdingA = holdingS = holdingD = holdingZ = holdingSpace = false;
  }

  void Camera::Update()
  {
    lookRotation = Util::RotateZ(theta.z) * Util::RotateX(theta.x);
    lookDirection = lookRotation * vec4(0.0f, 1.0f, 0.0f, 0.0f);

    move();

    updateView();
  }

  float Camera::GetZoomOut() const
  {
    return zoomOut;
  }

  void Camera::SetZoomOut(float z)
  {
    zoomOut = z;
  }

  vec4 Camera::GetLookDirection() const
  {
    return lookDirection;
  }

  void Camera::SetLookDirection(const vec3& lookDir)
  {
    lookDirection = normalize(vec4(lookDir, 0.0f));
  }

  void Camera::LookAt(const vec4& pos)
  {
    lookDirection = normalize(pos - position);
  }

  void Camera::RotateX(float x)
  {
    SetRotationX(static_cast<float>(clamp(theta.x + static_cast<double>(x),
					  -M_PI / 2.0 + 0.1,
					  M_PI / 2.0 - 0.1)));
  }

  void Camera::InputEvent(SDL_Event ev)
  {
    if (ev.type == SDL_KEYDOWN)
    {
      switch (ev.key.keysym.sym)
      {
      case SDLK_w:
	holdingW = true;
	break;
      case SDLK_a:
	holdingA = true;
	break;
      case SDLK_s:
	holdingS = true;
	break;
      case SDLK_d:
	holdingD = true;
	break;
      case SDLK_z:
	//Translate(vec4(0.0f, 0.0f, -engine::WorldSize / 100.0f, 0.0f));
	holdingZ = true;
	break;
      case SDLK_SPACE:
	//Translate(vec4(0.0f, 0.0f, engine::WorldSize / 100.0f, 0.0f));
	holdingSpace = true;
	break;
      default:
	break;
      }
    }
    else if (ev.type == SDL_KEYUP)
    {
      switch (ev.key.keysym.sym)
      {
      case SDLK_w:
	holdingW = false;
	break;
      case SDLK_a:
	holdingA = false;
	break;
      case SDLK_s:
	holdingS = false;
	break;
      case SDLK_d:
	holdingD = false;
	break;
      case SDLK_z:
	holdingZ = false;
	break;
      case SDLK_SPACE:
	holdingSpace = false;
	break;
      default:
	break;
      }
    }
    else if (ev.type == SDL_MOUSEMOTION)
    {
      RotateZ(ev.motion.xrel / 1000.0f);
      RotateX(ev.motion.yrel / 1000.0f);
    }
  }

  // PRIVATE

  void Camera::move()
  {
    vec4 moveDirection = vec4(0.0, 0.0, 0.0, 0.0);

    if (holdingA)
      moveDirection -= Util::RotateZ(static_cast<float>(M_PI / 2.0)) *
	lookDirection;
    if (holdingD)
      moveDirection += Util::RotateZ(static_cast<float>(M_PI / 2.0)) *
	lookDirection;
    if (holdingW)
      moveDirection += lookDirection;
    if (holdingS)
      moveDirection -= lookDirection;

    if (length(moveDirection) != 0.0)
      moveDirection = normalize(vec4(moveDirection.x, moveDirection.y,
				     0.0, 0.0));

    if (holdingZ)
      moveDirection -= vec4(0.0f, 0.0f, 1.0f, 0.0f);
    if (holdingSpace)
      moveDirection += vec4(0.0f, 0.0f, 1.0f, 0.0f);

    vec4 finalMove = moveDirection * engine::WorldSize / 10000.0f *
      static_cast<float>(FpsTracker::GetFrameTimeMs());

    Translate(finalMove);
  }

  void Camera::updateView()
  {
    vec4 lookPos = position + lookDirection;

    vec4 zoomOffset = -(normalize(lookPos - position)) * zoomOut;

    // engine::CameraMatrix = lookAt(vec3(position + zoomOffset),
    // 				  vec3(lookPos), vec3(0.0f, 0.0f, 1.0f));
    // Shader::SetCamera(engine::CameraMatrix);
    // engine::CameraPosition = position + zoomOffset;
    // Shader::SetCameraPosition(engine::CameraPosition);

    this->scene->SetView(lookAt(vec3(position + zoomOffset),
				vec3(lookPos), vec3(0.0f, 0.0f, 1.0f)));
    this->eye_position = position + zoomOffset;
    // Shader::SetCameraPosition(engine::CameraPosition);
  }
}
