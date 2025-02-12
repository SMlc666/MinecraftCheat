#pragma once
#include "glm/common.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/ext/vector_float2.hpp"
class AABB {
public:
  glm::vec3 min, max;
  glm::vec2 size; //width and height
public:
  // Constructor to initialize the AABB with min and max points
  AABB(const glm::vec3 &min, const glm::vec3 &max) : min(min), max(max) {
    size = glm::vec2(max.x - min.x, max.y - min.y);
  }

  // Default constructor
  AABB() : min(glm::vec3(0)), max(glm::vec3(0)), size(glm::vec2(0)) {
  }

  // Function to check if a point is inside the AABB
  bool Contains(const glm::vec3 &point) const {
    return point.x >= min.x && point.x <= max.x && point.y >= min.y && point.y <= max.y &&
           point.z >= min.z && point.z <= max.z;
  }

  // Function to check if another AABB intersects with this one
  bool Intersects(const AABB &other) const {
    return (min.x <= other.max.x && max.x >= other.min.x) &&
           (min.y <= other.max.y && max.y >= other.min.y) &&
           (min.z <= other.max.z && max.z >= other.min.z);
  }

  // Function to get the center of the AABB
  glm::vec3 GetCenter() const {
    return (min + max) * 0.5f;
  }

  // Function to get the dimensions (width, height, depth) of the AABB
  glm::vec3 GetDimensions() const {
    return max - min;
  }

  // Function to expand the AABB to include a point
  void ExpandToInclude(const glm::vec3 &point) {
    min = glm::min(min, point);
    max = glm::max(max, point);
    size = glm::vec2(max.x - min.x, max.y - min.y);
  }

  // Function to expand the AABB to include another AABB
  void ExpandToInclude(const AABB &other) {
    min = glm::min(min, other.min);
    max = glm::max(max, other.max);
    size = glm::vec2(max.x - min.x, max.y - min.y);
  }

  // Function to translate the AABB by a vector
  void Translate(const glm::vec3 &translation) {
    min += translation;
    max += translation;
  }

  // Function to scale the AABB uniformly
  void Scale(float scale) {
    glm::vec3 center = GetCenter();
    glm::vec3 dimensions = GetDimensions() * scale;
    min = center - dimensions * 0.5f;
    max = center + dimensions * 0.5f;
    size = glm::vec2(max.x - min.x, max.y - min.y);
  }

  // Function to compute the surface area of the AABB
  float SurfaceArea() const {
    glm::vec3 dimensions = GetDimensions();
    return 2.0f * (dimensions.x * dimensions.y + dimensions.y * dimensions.z +
                   dimensions.z * dimensions.x);
  }

  // Function to compute the volume of the AABB
  float Volume() const {
    glm::vec3 dimensions = GetDimensions();
    return dimensions.x * dimensions.y * dimensions.z;
  }

    // Setter for width (strategy a: keep min unchanged, adjust max)
    void setWidth(float width) {
        max.x = min.x + width;
        size.x = width;
    }

        // Setter for height (strategy a: keep min unchanged, adjust max)
    void setHeight(float height) {
        max.y = min.y + height;
        size.y = height;
    }

    // 策略b: 保持中心点不变
    void setWidthKeepCenter(float width) {
        glm::vec3 center = GetCenter();
        float halfWidth = width * 0.5f;
        min.x = center.x - halfWidth;
        max.x = center.x + halfWidth;
        size.x = width;
    }

    // 策略c: 保持max不变
    void setWidthKeepMax(float width) {
        min.x = max.x - width;
        size.x = width;
    }
};
