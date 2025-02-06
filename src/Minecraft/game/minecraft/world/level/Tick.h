#pragma once
#include "base/mcint.hpp"
struct Tick {
public:
  using Type = uint64;

  Type t;

  [[nodiscard]] constexpr operator Type() const {
    return t;
  } // NOLINT

  [[nodiscard]] constexpr Tick(Type v) : t(v){}; // NOLINT

  [[nodiscard]] constexpr Tick(Tick const &v) = default;

  [[nodiscard]] constexpr Tick() : t(0){};
};
