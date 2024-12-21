#include "config.hpp"
#include "log.hpp"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/error/en.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include <format>
#include <fstream>
namespace Config {
rapidjson::Document config;
void saveConfig() {
  std::ofstream configFile(NormalConfigPath, std::ios::out);
  if (!configFile.is_open()) {
    g_log_tool.message(LogLevel::ERROR, "Config", "Failed to open config file for writing");
    return;
  }
  rapidjson::StringBuffer buffer;
  rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
  Config::config.Accept(writer);
  configFile.write(buffer.GetString(), buffer.GetSize());
  configFile.close();
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
    configFile.close();
    g_log_tool.message(LogLevel::ERROR, "Config",
                       std::format("Failed to parse config file: {}",
                                   rapidjson::GetParseError_En(Config::config.GetParseError())));
    Config::config = rapidjson::Document();
    return;
  }
  configFile.close();
}