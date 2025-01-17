#pragma once
#include "game/deps/core/Bedrock/Bedrock.hpp"
#include "game/minecraft/client/game/ClientGameSetupResult.hpp"
#include "game/minecraft/client/game/ClientPlayMode.hpp"
#include "game/deps/core/Bedrock/IAsyncResult.hpp"
#include "game/minecraft/client/social/MultiplayerServiceIdentifier.hpp"
class AbstractScene;
class Actor;
class ActorBlockRenderer;
class ActorRenderDispatcher;
class ActorResourceDefinitionGroup;
class BlockActorRenderDispatcher;
class BlockCullingGroup;
class BlockSource;
class BlockTessellator;
class BlockTypeRegistry;
class BuildActionIntention;
class CachedScenes;
class CameraRegistry;
class ClientHitDetectCoordinator;
class ClientInputHandler;
class ClientInstanceEventCoordinator;
class ClientMoveInputHandler;
class ClientNetworkEventCoordinator;
class ClientNetworkSystem;
class ClientRequirementVerifier;
class ClientScriptEventCoordinator;
class ClientScriptManager;
class DateManager;
class DeferredLighting;
class DevConsoleLogger;
class DisconnectionRequestHandler;
class EntityContext;
class EntitySystems;
class FileDataRequest;
class FogDefinitionRegistry;
class FogManager;
class FontHandle;
class FrameUpdateContext;
class GameModuleClient;
class GameRenderer;
class GeometryGroup;
class GuiData;
class GuidedFlowManager;
class HitDetectSystem;
class HitResult;
class HolographicPlatform;
class HolosceneRenderer;
class IClientInstances;
class IConnectionEventing;
class IContentKeyProvider;
class IGameConnectionListener;
class IGameModuleApp;
class IMinecraftEventing;
class IMinecraftGame;
class IOptions;
class IResourcePackRepository;
class ISceneStack;
class ITTSEventManager;
class IUIRepository;
class ItemInHandRenderer;
class ItemRegistryRef;
class ItemRenderer;
class KeyboardManager;
class LatencyGraphDisplay;
class Level;
class LevelRenderer;
class LevelRendererCameraProxy;
class LightTexture;
class LocalPlayer;
class MarketplaceServicesManager;
class Minecraft;
class MinecraftGraphics;
class MinecraftInputHandler;
class MobEffectsLayout;
class MultiPlayerLevel;
class Option;
class Options;
class PackManifestFactory;
class PacketSender;
class PersonaRepository;
class PixelCalc;
class Player;
class PlayerReportHandler;
class ResourcePackManager;
class SceneFactory;
class SceneStack;
class ScreenContext;
class ScreenLoadTimeTracker;
class ShaderColor;
class SkinRepository;
class SkinRepositoryClientInterface;
class SoundEngine;
class StoreCatalogItem;
class StoreCatalogRepository;
class TaskGroup;
class TextToSpeechClient;
class Timer;
class ToastManager;
class TrialManager;
class UIEventCoordinator;
class UIProfanityContext;
class UserAuthentication;
class Vec2;
class Vec3;
class VoiceSystem;
class WeakEntityRef;
class WorldTransferAgent;
struct ActorUniqueID;
struct ClientHMDState;
struct DisconnectionScreenParams;
struct ListenerState;
struct LocalPlayerChangedConnector;
struct PacksInfoData;
struct ScreenshotOptions;
struct SplitScreenInfo;
namespace ApplicationSignal {
class ClipboardCopy;
}
namespace ApplicationSignal {
class ClipboardPasteRequest;
}
namespace Automation {
class AutomationClient;
}
namespace Bedrock::Http {
class Status;
}
namespace Core {
class FilePathManager;
}
namespace Core {
class FileStorageArea;
}
namespace Core {
class Path;
}
namespace Editor {
class IEditorManager;
}
namespace Editor {
class IEditorPlayer;
}
namespace OreUI {
class ITelemetry;
}
namespace OreUI {
class Router;
}
namespace OreUI {
class SceneProvider;
}
namespace OreUI {
class UIBlockThumbnailAtlasManager;
}
namespace PlayerCapabilities {
struct IClientController;
}
namespace Realms {
struct World;
}
namespace Scripting {
class ScriptEngine;
}
namespace Social {
class GameConnectionInfo;
}
namespace Social {
class IUserManager;
}
namespace Social {
class MultiplayerServiceManager;
}
namespace Social {
class User;
}
namespace mce {
class Camera;
}
namespace mce {
class Texture;
}
namespace mce {
class TextureGroup;
}
namespace mce {
class TexturePtr;
}
namespace mce {
class UUID;
}
namespace mce {
struct ViewportInfo;
}
namespace persona {
class PersonaPieceCollectionModel;
}
namespace ui {
class ScreenTechStackSelector;
}
class IClientInstance : Bedrock::EnableNonOwnerReferences {
public:
  //NOLINTBEGIN
  // vtable index: 0
  virtual ~IClientInstance() = default;

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
  // vtable index: 61
  virtual bool isInBedScreen() const = 0;

  // vtable index: 62
  virtual bool isInDeathScreen() const = 0;

  // vtable index: 63
  virtual bool isKeyboardEnabled() const = 0;

  // vtable index: 64
  virtual bool isVRTransitioning() const = 0;

  // vtable index: 65
  virtual bool hasCommands() const = 0;

  // vtable index: 66
  virtual float getHoloviewerScale() const = 0;

  // vtable index: 67
  virtual int getSplitScreenCount() const = 0;

  // vtable index: 68
  virtual bool isStereoRendering() const = 0;

  // vtable index: 69
  virtual bool isShowingLoadingScreen() const = 0;

  // vtable index: 70
  virtual bool shouldDisconnectOnAppSuspended() const = 0;

  // vtable index: 71
  virtual bool isGamePlayTipsEnabled() const = 0;

  // vtable index: 72
  virtual bool isPlatformNX() const = 0;

  // vtable index: 73
  virtual bool isLocalSplitscreenWith(::ActorUniqueID const &) const = 0;

  // vtable index: 74
  virtual bool isValidCrossPlatformSkin() const = 0;

  // vtable index: 75
  virtual bool isCurrentSkinPlatformLocked() const = 0;

  // vtable index: 76
  virtual bool isSelectedSkinInitialized() const = 0;

  // vtable index: 77
  virtual ::SplitScreenInfo getSplitScreenInfo() const = 0;

  // vtable index: 78
  virtual int getCurrentMaxGUIScaleIndex() const = 0;

  // vtable index: 79
  virtual float const &getRealityModeFrameFactor() const = 0;

  // vtable index: 80
  virtual void setRealityModeFrameFactor(float const &) = 0;

  // vtable index: 81
  virtual bool getRealityModeToggleTriggered() const = 0;

  // vtable index: 82
  virtual void setRealityModeToggleTriggered(bool) = 0;

  // vtable index: 83
  virtual bool getHandlingControllerDisconnect() = 0;

  // vtable index: 84
  virtual void setOpenControllerDisconnectScreen(bool) = 0;

  // vtable index: 85
  virtual ClientPlayMode getClientPlayMode() const = 0;

  // vtable index: 86
  virtual void setClientPlayMode(ClientPlayMode const &) = 0;

  // vtable index: 87
  virtual bool getLivingRoomForCredits() const = 0;

  // vtable index: 88
  virtual void setLivingRoomForCredits(bool) = 0;

  // vtable index: 89
  virtual std::function<void()> getCreditsCallback() = 0;

  // vtable index: 90
  virtual void setCreditsCallback(std::function<void()>) = 0;

  // vtable index: 91
  virtual void setupTransitionForCredits(std::function<void()>) = 0;

  // vtable index: 92
  virtual void refreshScreenSizeData() = 0;

  // vtable index: 93
  virtual void onScreenSizeChanged(int, int, float) = 0;

  // vtable index: 94
  virtual void onGuiScaleOffsetChanged() = 0;

  // vtable index: 95
  virtual void onSafeZoneChanged() = 0;

  // vtable index: 96
  virtual bool hasDismissedNewPlayerFlow() const = 0;

  // vtable index: 97
  virtual void quit(std::string const &, ::std::string const &) = 0;

  // vtable index: 98
  virtual IMinecraftGame &getMinecraftGame_DEPRECATED() const = 0;

  // vtable index: 99
  virtual IClientInstances &getClientInstances() const = 0;

  // vtable index: 100
  virtual ::Bedrock::NotNullNonOwnerPtr<Automation::AutomationClient>
  getAutomationClient() const = 0;

  // vtable index: 101
  virtual IMinecraftEventing &getEventing() const = 0;

  // vtable index: 102
  virtual IConnectionEventing &getConnectionEventing() const = 0;

  // vtable index: 103
  virtual FontHandle getFontHandle() const = 0;

  // vtable index: 104
  virtual FontHandle getRuneFontHandle() const = 0;

  // vtable index: 105
  virtual FontHandle getUnicodeFontHandle() const = 0;

  // vtable index: 106
  virtual Bedrock::NotNullNonOwnerPtr<GeometryGroup> getGeometryGroup() const = 0;

  // vtable index: 107
  virtual Bedrock::NotNullNonOwnerPtr<BlockCullingGroup> getBlockCullingGroup() const = 0;

  // vtable index: 108
  virtual Bedrock::NotNullNonOwnerPtr<Social::MultiplayerServiceManager>
  getMultiplayerServiceManager() const = 0;

  // vtable index: 109
  virtual IResourcePackRepository &getResourcePackRepository() const = 0;

  // vtable index: 110
  virtual ResourcePackManager &getResourcePackManager() const = 0;

  // vtable index: 111
  virtual PackManifestFactory &getPackManifestFactory() = 0;

  // vtable index: 112
  virtual Bedrock::NotNullNonOwnerPtr<IContentKeyProvider const> getKeyProvider() const = 0;

  // vtable index: 113
  virtual PacksInfoData const &getHostSpecifiedPacks() const = 0;

  // vtable index: 114
  virtual std::shared_ptr<SkinRepository> getSkinRepository() const = 0;
  //NOLINTEND
};