#pragma once
#include <string>

static const std::string NormalConfigPath = "/sdcard/MinecraftCheat/config.json";

extern void configSetup();
namespace Config {
void saveConfig();
void initConfig();
void loadConfig();
} // namespace Config
