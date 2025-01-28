#include "BlockLegacy.hpp"
#include <string>
std::string BlockLegacy::getName() const {
  const std::string a = *name;
  return a;
}