#pragma once
#include "tag.hpp"

class StringTag : public Tag {
public:
  std::string data;

  StringTag(std::string data) {
    this->data = data;
  }
};