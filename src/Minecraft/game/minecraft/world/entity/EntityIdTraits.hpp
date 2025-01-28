#pragma once
#include "base/mcint.hpp"
#include "entt/entt.hpp"
class EntityId;

struct EntityIdTraits {
  using value_type = EntityId;

  using entity_type = uint32;
  using version_type = uint16;

  static constexpr entity_type entity_mask = 0x3FFFF;
  static constexpr version_type version_mask = 0x3FFF;
};

// Specialize entt::storage_type by removing sigh_mixin
template <typename Type> struct entt::storage_type<Type, EntityId> {
  using type = basic_storage<Type, EntityId>;
};