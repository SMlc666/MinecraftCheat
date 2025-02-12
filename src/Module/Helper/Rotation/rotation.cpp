#include "rotation.hpp"
#include <algorithm>
#include <cmath>

Helper::Rotation::Rotation Helper::Rotation::toRotation(const glm::vec3 &from,
                                                        const glm::vec3 &to) {
  float diffX = to.x - from.x;
  float diffY = to.y - from.y;
  float diffZ = to.z - from.z;
  Helper::Rotation::Rotation rotation;
  rotation.yaw = static_cast<float>(std::atan2(diffZ, diffX) * (180.0f / glm::pi<float>()) - 90.0f);
  rotation.pitch = static_cast<float>(-std::atan2(diffY, std::sqrt(diffX * diffX + diffZ * diffZ)) *
                                      (180.0f / glm::pi<float>()));
  return rotation;
}
float Helper::Rotation::getAngleDifference(float a, float b) {
  return std::fmod(std::fmod(a - b, 360.0f) + 540.0f, 360.0f) - 180.0f;
}
float Helper::Rotation::getRotationDifference(const Rotation &a, const Rotation &b) {
  return std::hypot(getAngleDifference(a.yaw, b.yaw), a.pitch - b.pitch);
}

Helper::Rotation::Rotation Helper::Rotation::interpolateRotation(
    const Rotation& current,
    const Rotation& target,
    float deltaTime,
    float minStep,
    float maxStep,
    float stepFactor) {
  if (current.pitch == target.pitch && current.yaw == target.yaw) {
    return target;
  }

  float rotationDiff = getRotationDifference(current, target);

  // 如果角度差小于等于0,直接返回目标角度
  if (rotationDiff <= 0.0f) {
    return target;
  }

  // 修改步长计算公式
  float step = stepFactor * deltaTime * (rotationDiff + 1.0f);
  step = std::clamp(step, minStep, maxStep);

  // 使用加权平均计算插值后的角度
  float weight = step / rotationDiff;
    float yawDiff = getAngleDifference(target.yaw, current.yaw);
  float newPitch = current.pitch + (target.pitch - current.pitch) * weight;
  float newYaw = current.yaw + yawDiff * weight;

  // 规范化pitch和yaw的值
  newPitch = std::clamp(newPitch, -90.0f, 90.0f);
  newYaw = std::fmod(newYaw + 180.0f, 360.0f) - 180.0f;

  return {newPitch, newYaw};
}
