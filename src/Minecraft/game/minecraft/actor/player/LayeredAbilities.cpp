#include "LayeredAbilities.hpp"
#include "signature.hpp"

void LayeredAbilities::setAbility(AbilitiesIndex index, bool value) {
  using function = void (*)(LayeredAbilities *, AbilitiesIndex, bool);
  auto func = getSign<function>("LayeredAbilities::setAbility");
  return func(this, index, value);
}