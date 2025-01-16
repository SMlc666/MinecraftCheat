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
class LocalPlayer;
namespace Editor {
class IEditorPlayer;
}
namespace Editor {
class IEditorManager;
}
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
  virtual ::BlockSource *getRegion() = 0;

  // vtable index: 30
  virtual ::LocalPlayer *getLocalPlayer() const = 0;

  // vtable index: 31
  virtual void setupPrimaryClientEditorManager() = 0;

  // vtable index: 32
  virtual ::Bedrock::NonOwnerPointer<Editor::IEditorPlayer> getLocalEditorPlayer() const = 0;

  // vtable index: 33
  virtual ::Bedrock::NonOwnerPointer<Editor::IEditorManager> getEditorManager() const = 0;

  // vtable index: 34
  virtual bool isPlayerInEditor() const = 0;
  //NOLINTEND
};