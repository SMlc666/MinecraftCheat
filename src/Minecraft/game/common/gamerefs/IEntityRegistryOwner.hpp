#pragma once
#include "game/common/gamerefs/StackRefResult.hpp"
#include "game/deps/core/NonOwnerPointer.hpp"
class EntityRegistry;

class IEntityRegistryOwner : public Bedrock::EnableNonOwnerReferences {
protected:
  virtual ~IEntityRegistryOwner();

public:
  virtual StackRefResult<const EntityRegistry> getEntityRegistry() const;
  virtual StackRefResult<EntityRegistry> getEntityRegistry();
};