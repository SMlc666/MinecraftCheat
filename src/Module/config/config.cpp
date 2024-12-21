#include "config.hpp"
#include "log.hpp"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/error/en.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include <exception>
#include <filesystem>
#include <format>
#include <fstream>
#include <stdexcept>
namespace Config {
rapidjson::Document config;
void saveConfig() {
  std::ofstream configFile(NormalConfigPath, std::ios::out);
  if (!configFile.is_open()) {
    throw std::runtime_error(
        std::format("Failed to open config file for writing. {}", NormalConfigPath));
  }
  rapidjson::StringBuffer buffer;
  rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
  Config::config.Accept(writer);
  configFile.write(buffer.GetString(), buffer.GetSize());
  configFile.close();
}
void loadConfig() {
  std::ifstream configFile(NormalConfigPath, std::ios::in);
  if (!configFile.is_open()) {
    throw std::runtime_error(
        std::format("Failed to open config file for reading. {}", NormalConfigPath));
  }
  rapidjson::IStreamWrapper isw(configFile);
  Config::config.ParseStream(isw);
  if (Config::config.HasParseError()) {
    Config::config = rapidjson::Document();
    configFile.close();
    throw std::runtime_error(
        std::format("Failed to parse config file: {}",
                    rapidjson::GetParseError_En(Config::config.GetParseError())));
  }
  configFile.close();
}
} // namespace Config
void configSetup() {
  std::filesystem::path configPath(NormalConfigPath);
  if (!std::filesystem::exists(configPath)) {
    Config::saveConfig();
  } //初始化
  try {
    Config::loadConfig();
  } catch (std::exception &e) {
    g_log_tool.message(LogLevel::ERROR, "Config", e.what());
  }
}