#include "signature.hpp"
#include "MemTool.hpp"
#include "log.hpp"
#include <cstdint>
#include <format>
#include <unordered_map>
static const std::string module = "libminecraftpe.so";
//NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
const std::unordered_map<std::string, uintptr_t> IDApattern = {
    {"Minecraft::Minecraft", 0x7396278},
    {"Minecraft::update", 0x739736C},
    {"ClientInstance::onStartJoinGame", 0x501A590},
    {"Player::getGameMode", 0x6D03208},
    {"Player::getName", 0x6CE97A8},
    {"Actor::getLevel", 0x723AF78},
    {"LoopbackPacketSender::send", 0x62BA068},
    {"GameMode::attack", 0x6B87228},
    {"Dimension::forEachPlayer", 0x89414F8},
    {"Actor::getHealth", 0x724DF58},
    {"Player::getSupplies", 0x6CEABB4},
    {"ItemStackBase::isBlock", 0x858FC20},
    {"ItemStackBase::isNull", 0x858E888},
    {"LevelRenderer::renderLevel", 0x4BA4690},
    {"LocalPlayer::NormalTick", 0x5579B68},
    {"Actor::lerpMotion", 0x7252FA4},
    {"ActorCollision::isOnGround", 0x6CF032C},
    {"AnimatePacket::AnimatePacket", 0x6211FCC},
    {"Actor::setPos", 0x72448A8},
    {"Mob::isSprinting", 0x70FBAC0},
    {"Actor::isJumping", 0x725109C},
    {"Actor::getFallDistance", 0x724D788},
    {"MoveInputHandler::tick", 0x55FBE18},
    {"Player::canOpenContainerScreen", 0x6CF5A3C},
};
//NOLINTEND(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
std::unordered_map<std::string, void *> signatures{};
void signaturesInit() {
  void *base = MemTool::getModuleBase<void *>(module);
  void *end = MemTool::getModuleEnd<void *>(module);
  g_log_tool.message(LogLevel::INFO, "initSignatures",
                     std::format("Module base: {:p}, end: {:p}", base, end));
  for (const auto &IDA : IDApattern) {
    void *addr = reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(base) + IDA.second);
    signatures.insert({IDA.first, addr});
    g_log_tool.message(LogLevel::INFO, "initSignatures",
                       std::format("Found {} at {:p}", IDA.first, addr));
  }
}