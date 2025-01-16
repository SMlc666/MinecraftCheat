#pragma once
#include "game/deps/core/Bedrock/Bedrock.hpp"
#include "game/minecraft/client/game/ClientGameSetupResult.hpp"
#include "game/deps/core/Bedrock/IAsyncResult.hpp"
namespace Core {
class FilePathManager;
}
class Timer;
class ClientNetworkSystem;
class IGameModuleApp;
class SceneFactory;
class WeakEntityRef;
namespace Social {
class IUserManager;
}
namespace OreUI {
class SceneProvider;
}
namespace OreUI {
class Router;
}
namespace Social {
class GameConnectionInfo;
}
namespace Social {
class MultiplayerServiceIdentifier;
}
class FrameUpdateContext;
class GameModuleClient;
class BlockSource;
class Actor;
class LocalPlayer;
namespace Editor {
class IEditorPlayer;
}
namespace Editor {
class IEditorManager;
}
struct ListenerState;
class HitResult;
struct LocalPlayerChangedConnector;
class IClientInstance : Bedrock::EnableNonOwnerReferences {
public:
  //NOLINTBEGIN
  // vtable index: 0
  virtual ~IClientInstance() /*override*/ = default;

  // vtable index: 1
  virtual void onInitMinecraftGame() = 0;

  // vtable index: 2
  virtual void onDestroyMinecraftGame() = 0;

  // vtable index: 3
  virtual void init(Bedrock::NotNullNonOwnerPtr<Core::FilePathManager> const &,
                    ClientNetworkSystem &, Timer &, Timer &,
                    Bedrock::NotNullNonOwnerPtr<Social::IUserManager> const &, int,
                    IGameModuleApp &) = 0;

  // vtable index: 4
  virtual void initSceneFactory(std::unique_ptr<SceneFactory>,
                                std::unique_ptr<OreUI::SceneProvider>) = 0;
  // vtable index: 5
  virtual void setUiRouter(std::unique_ptr<::OreUI::Router>) = 0;

  // vtable index: 6
  virtual void initGraphics() = 0;

  // vtable index: 7
  virtual void stop() = 0;

  // vtable index: 8
  virtual void teardown() = 0;

  // vtable index: 9
  virtual void preShutDown() = 0;
  // vtable index: 10
  virtual void setGameConnectionInfo(Social::GameConnectionInfo const &) = 0;

  // vtable index: 11
  virtual void onStartJoinGame(bool, std::string const &, int,
                               Social::MultiplayerServiceIdentifier) = 0;

  // vtable index: 12
  virtual void onCancelJoinGame() = 0;

  // vtable index: 13
  virtual void requestLeaveGameAsync() = 0;

  // vtable index: 14
  virtual void requestLeaveGame(bool, bool) = 0;

  // vtable index: 15
  virtual void stopPlayScreen() = 0;

  // vtable index: 16
  virtual bool isLeaveGameDone() const = 0;

  // vtable index: 17
  virtual void setupPlayScreenForLeaveGame() = 0;
  // vtable index: 18
  virtual void resetPrimaryClient() = 0;

  // vtable index: 19
  virtual void resetGameSession() = 0;

  // vtable index: 20
  virtual void tick() = 0;

  // vtable index: 21
  virtual void frameUpdate(FrameUpdateContext &) = 0;

  // vtable index: 22
  virtual bool update(bool) = 0;

  // vtable index: 23
  virtual void preFrameTick() = 0;

  // vtable index: 24
  virtual void endFrame() = 0;

  // vtable index: 25
  virtual float getFrameAlpha() = 0;

  // vtable index: 26
  virtual void startSubClientLateJoin(bool, std::unique_ptr<GameModuleClient>) = 0;

  // vtable index: 28
  virtual void setupClientGame(std::function<void(ClientGameSetupResult)> &&, bool,
                               std::unique_ptr<GameModuleClient>) = 0;

  // vtable index: 27
  virtual std::shared_ptr<Bedrock::Threading::IAsyncResult<ClientGameSetupResult>>
  setupClientGame(bool, std::unique_ptr<GameModuleClient>) = 0;

  // vtable index: 29
  virtual BlockSource *getRegion() = 0;

  // vtable index: 30
  virtual LocalPlayer *getLocalPlayer() const = 0;

  // vtable index: 31
  virtual void setupPrimaryClientEditorManager() = 0;

  // vtable index: 32
  virtual Bedrock::NonOwnerPointer<Editor::IEditorPlayer> getLocalEditorPlayer() const = 0;

  // vtable index: 33
  virtual Bedrock::NonOwnerPointer<Editor::IEditorManager> getEditorManager() const = 0;

  // vtable index: 34
  virtual bool isPlayerInEditor() const = 0;
  // vtable index: 35
  virtual LocalPlayerChangedConnector getLocalPlayerChangedConnector() = 0;

  // vtable index: 36
  virtual WeakEntityRef getLocalUser() const = 0;

  // vtable index: 37
  virtual Actor *getCameraActor() const = 0;

  // vtable index: 38
  virtual void setCameraActor(Actor *) = 0;

  // vtable index: 39
  virtual ListenerState const *getListenerState() const = 0;

  // vtable index: 40
  virtual Actor *getCameraTargetActor() const = 0;

  // vtable index: 41
  virtual void setCameraTargetActor(::Actor *) = 0;

  // vtable index: 42
  virtual WeakEntityRef getCameraEntity() const = 0;

  // vtable index: 43
  virtual void getRawCameraEntities(WeakEntityRef &, WeakEntityRef &) const = 0;

  // vtable index: 44
  virtual void setRawCameraEntities(WeakEntityRef, WeakEntityRef) = 0;

  // vtable index: 45
  virtual HitResult const &getLatestHitResult() const = 0;

  // vtable index: 46
  virtual bool isLeavingGame() const = 0;

  // vtable index: 47
  virtual bool isDestroyingGame() const = 0;

  // vtable index: 48
  virtual bool isShuttingDown() const = 0;

  // vtable index: 49
  virtual bool useLowFrequencyUIRender() const = 0;

  // vtable index: 50
  virtual bool isSplitScreenActive() const = 0;

  // vtable index: 51
  virtual bool isVRClient() const = 0;

  // vtable index: 52
  virtual bool isARClient() const = 0;

  // vtable index: 53
  virtual bool isARVRClient() const = 0;

  // vtable index: 54
  virtual bool isHoloviewerMode() const = 0;

  // vtable index: 55
  virtual bool isHoloscreenMode() const = 0;

  // vtable index: 56
  virtual bool isLivingroomMode() const = 0;

  // vtable index: 57
  virtual bool isExitingLevel() const = 0;

  // vtable index: 58
  virtual bool isNotVLRMode() const = 0;

  // vtable index: 59
  virtual bool isHoloRealityMode() const = 0;

  // vtable index: 60
  virtual bool isRealityFullVRMode() const = 0;
  //NOLINTEND
};