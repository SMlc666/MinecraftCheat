#include "runtimes.hpp"
static Minecraft *g_mc = nullptr;
Minecraft *runtimes::getMinecraftInstance() {
  return g_mc;
}
void runtimes::setMinecraftInstance(Minecraft *mc_) {
  g_mc = mc_;
}
