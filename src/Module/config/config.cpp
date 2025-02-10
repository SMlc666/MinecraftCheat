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
#include <mutex>
#include <stdexcept>
namespace Config {
std::mutex config_mutex;
rapidjson::Document config;
std::string getString(bool is_pretty) {
  std::lock_guard<std::mutex> lock(config_mutex);
  if (!config.IsObject()) {
    return "";
  }
  if (is_pretty) {
    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    config.Accept(writer);
    return buffer.GetString();
  }
  return config.GetString();
}
void initConfig() {
  std::lock_guard<std::mutex> lock(config_mutex);
  config = rapidjson::Document();
  config.SetObject();
}
rapidjson::Document &getDocument() {
  std::lock_guard<std::mutex> lock(config_mutex);
  return config;
}
void saveConfig() {
  std::lock_guard<std::mutex> lock(config_mutex);
  std::ofstream configFile(NormalConfigPath, std::ios::out);
  if (!configFile.is_open()) {
    throw std::runtime_error(
        std::format("Failed to open config file for writing. {}", NormalConfigPath));
  }
  rapidjson::StringBuffer buffer;
  rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
  Config::config.Accept(writer);
  configFile.write(buffer.GetString(), static_cast<std::streamsize>(buffer.GetSize()));
  configFile.close();
}
void loadConfig() {
  std::lock_guard<std::mutex> lock(config_mutex);
  std::ifstream configFile(NormalConfigPath, std::ios::in);
  if (!configFile.is_open()) {
    throw std::runtime_error(
        std::format("Failed to open config file for reading. {}", NormalConfigPath));
  }
  rapidjson::IStreamWrapper isw(configFile);
  Config::config.ParseStream(isw);
  if (Config::config.HasParseError()) {
    initConfig();
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
  Config::initConfig();
  if (!std::filesystem::exists(configPath)) {
    Config::saveConfig();
  } //初始化
  try {
    Config::loadConfig();
  } catch (std::exception &e) {
    g_log_tool.message(LogLevel::ERROR, "Config", e.what());
  }
}