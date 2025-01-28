#pragma once
#include "glm/ext/vector_float3.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"
namespace Helper::Rotation {
struct Rotation {
  float pitch;
  float yaw;
};
Rotation toRotation(const glm::vec3 &from, const glm::vec3 &to);
float getAngleDifference(float a, float b);
float getRotationDifference(const Rotation &a, const Rotation &b);
} // namespace Helper::Rotation
