#pragma once
#include "glm/ext/vector_float3.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"
namespace Helper::Rotation {
struct Rotation {
  float pitch;
  float yaw;
};

// 新增插值函数声明
Rotation interpolateRotation(
    const Rotation& current,
    const Rotation& target,
    float deltaTime = 0.016f,
    float minStep = 0.01f,
    float maxStep = 1.0f,
    float stepFactor = 0.1f
);

Rotation toRotation(const glm::vec3 &from, const glm::vec3 &to);
float getAngleDifference(float a, float b);
float getRotationDifference(const Rotation &a, const Rotation &b);
} // namespace Helper::Rotation
