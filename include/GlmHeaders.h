/**
 * @file GlmHeaders.h
 * @author Alexander Bagnall
 * @brief Convenience header for including commonly used GLM headers.
 */

#ifndef GLM_HEADERS_H
#define GLM_HEADERS_H
#define GLM_FORCE_RADIANS

#ifdef _WIN32
    #define M_PI    3.14159265358979323846264338327950288
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/matrix.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
//#include <glm/gtx/fast_trigonometry.hpp> // for wrapAngle, not there on linux

using namespace glm;

#endif // GLM_HEADERS_H