#include "rotation.hpp"
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