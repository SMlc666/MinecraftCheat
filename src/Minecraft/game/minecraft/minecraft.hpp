#pragma once
#include "game/common/gamerefs/StackRefResult.hpp"
#include "game/common/gamerefs/IEntityRegistryOwner.hpp"
class Minecraft : public IEntityRegistryOwner {
public:
  std::byte padding[256];

public:
  virtual ~Minecraft();
  virtual StackRefResult<const EntityRegistry> getEntityRegistry() const override;
  virtual StackRefResult<EntityRegistry> getEntityRegistry() override;
  virtual void setSimTimePuase(bool pause);
  virtual void setSimTimeScale(float scale);
  virtual bool getSimPaused() const;
  virtual bool isOnlineClient() const;

public:
  Minecraft *Minecraft_(Minecraft *a1, void *a2, void *a3, void *a4, void *a5, void *a6, void *a7,
                        void *a8, void *a9, void *a10, char a11, void *a12, void *a13, void *a14,
                        void *a15, void *a16, void *a17);
};