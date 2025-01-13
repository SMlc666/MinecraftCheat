#pragma once
#include "tag.hpp"

class ByteTag : public Tag {
public:
  unsigned char data;

  ByteTag(unsigned char data) {
    this->data = data;
  }
};