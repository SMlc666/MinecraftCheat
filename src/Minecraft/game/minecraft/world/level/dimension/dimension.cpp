#include "dimension.hpp"

#include <utility>
#include "signature.hpp"
void Dimension::forEachPlayer(std::function<bool(Player &)> callback) {
  using function = void (*)(Dimension *, std::function<bool(Player &)>);
  auto func = getSign<function>("Dimension::forEachPlayer");
  func(this, std::move(callback));
}