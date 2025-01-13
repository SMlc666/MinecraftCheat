#pragma once
#include "tag.hpp"
#include <cstdint>

class Int64Tag : public Tag {
public:
  int64_t data;
};