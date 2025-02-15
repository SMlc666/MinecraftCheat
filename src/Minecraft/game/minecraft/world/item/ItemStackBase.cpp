#include "ItemStackBase.hpp"
#include "signature.hpp"
bool ItemStackBase::isBlock() const {
  using function = bool (*)(const ItemStackBase *);
  auto func = getSign<function>("ItemStackBase::isBlock");
  return func(this);
}
bool ItemStackBase::isNull() const {
  using function = bool (*)(const ItemStackBase *);
  auto func = getSign<function>("ItemStackBase::isNull");
  return func(this);
}
std::string ItemStackBase::getRawNameId() const {
  using function = std::string (*)(const ItemStackBase *);
  auto func = getSign<function>("ItemStackBase::getRawNameId");
  return func(this);
}