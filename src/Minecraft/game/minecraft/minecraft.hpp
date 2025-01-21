#pragma once
#include "game/common/SubClientId.hpp"
#include "game/common/gamerefs/StackRefResult.hpp"
#include "game/common/gamerefs/IEntityRegistryOwner.hpp"
#include "gsl/gsl"
#include <cstddef>
class GameCallbacks;
class IMinecraftEventing;
class AllowList;
class ServerMetrics;
class PermissionsFile;
class GameSession;
class MinecraftCommands;
class Timer;
class PacketSender;
class IMinecraftApp;
namespace Core {
class FilePathManager;
}
class Minecraft : public IEntityRegistryOwner {
public:
  //NOLINTBEGIN
  GameCallbacks &mGameCallbacks;
  IMinecraftEventing &mEventing;
  std::byte padding28[8];
  std::byte padding30[8];
  std::byte padding38[8];
  std::byte padding40[8];
  AllowList &mAllowList;
  PermissionsFile *mPermissionsFile;
  std::byte padding58[8];
  std::byte padding60[2];
  std::byte padding62[22];
  std::byte padding78[2];
  std::byte padding80[8];
  ServerMetrics *mServerMetrics;
  std::byte padding90[2];
  std::byte padding92[6];
  std::unique_ptr<MinecraftCommands> mCommands;
  std::unique_ptr<GameSession> mGameSession;
  std::chrono::seconds mMaxPlayerIdleTime;
  std::byte paddingB0[16];
  std::byte paddingC0[16];
  Timer &mSimTimer;
  Timer &mRealTimer;
  std::byte paddingA8[8];
  PacketSender &mPacketSender;
  std::byte paddingE8[8];
  IMinecraftApp &mApp;
  SubClientId mClientSubId;
  //NOLINTEND
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

public:
  Timer &getRealTimer();
};
static_assert(offsetof(Minecraft, mGameCallbacks) == 0x18, "Minecraft has wrong layout");
static_assert(offsetof(Minecraft, mEventing) == 0x20, "Minecraft has wrong layout");
static_assert(offsetof(Minecraft, mAllowList) == 0x48, "Minecraft has wrong layout");
static_assert(offsetof(Minecraft, mPermissionsFile) == 0x50, "Minecraft has wrong layout");
static_assert(offsetof(Minecraft, mServerMetrics) == 0x88, "Minecraft has wrong layout");
static_assert(offsetof(Minecraft, mCommands) == 0x98, "Minecraft has wrong layout");
static_assert(offsetof(Minecraft, mGameSession) == 0xA0, "Minecraft has wrong layout");
static_assert(offsetof(Minecraft, mMaxPlayerIdleTime) == 0xA8, "Minecraft has wrong layout");
static_assert(offsetof(Minecraft, mSimTimer) == 0xD0, "Minecraft has wrong layout");
static_assert(offsetof(Minecraft, mRealTimer) == 0xD8, "Minecraft has wrong layout");
static_assert(offsetof(Minecraft, mPacketSender) == 0xE8, "Minecraft has wrong layout");
static_assert(offsetof(Minecraft, mApp) == 0xF8, "Minecraft has wrong layout");
static_assert(offsetof(Minecraft, mClientSubId) == 0x100, "Minecraft has wrong layout");