#pragma once
class Minecraft;
namespace runtimes {
void setMinecraftInstance(Minecraft *mc_);
Minecraft *getMinecraftInstance();
} // namespace runtimes