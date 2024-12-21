#include "config.hpp"
#include "log.hpp"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include <fstream>
namespace Config {
rapidjson::Document config;
void saveConfig() {
}
} // namespace Config
void configSetup() {
  std::ifstream configFile(NormalConfigPath, std::ios::out | std::ios::in | std::ios::app);
  if (!configFile.is_open()) {
    g_log_tool.message(LogLevel::ERROR, "Config", "Failed to open config file for writing");
    return;
  }
  rapidjson::IStreamWrapper isw(configFile);
  Config::config.ParseStream(isw);
  if (Config::config.HasParseError()) {
    g_log_tool.message(LogLevel::ERROR, "Config", "Failed to parse config file");
    return;
  }
}