#pragma once
#include <string>

static const std::string NormalConfigPath = "/sdcard/MinecraftCheat/config.json";

extern void configSetup();
namespace Config {
std::string getString(bool is_pretty = false);
void saveConfig();
void initConfig();
void loadConfig();
} // namespace Config
