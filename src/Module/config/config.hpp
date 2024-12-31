#pragma once
#include "rapidjson/document.h"
#include <string>

static const std::string NormalConfigPath = "/sdcard/MinecraftCheat/config.json";

extern void configSetup();
namespace Config {
std::string getString(bool is_pretty = false);
rapidjson::Document &getDocument();
void saveConfig();
void initConfig();
void loadConfig();
} // namespace Config
