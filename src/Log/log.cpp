#include "log.hpp"
#include <mutex>
#include <unordered_map>
#include <format>
#include <fstream>
void Log::message(LogLevel Level, const std::string &tag, const std::string &message) {
  std::lock_guard<std::mutex> lock(mtx);
#ifndef _DEBUG_
  if (Level == LogLevel::DEBUG)
    return;
#endif
  tag_map.insert({tag, true});
  logs.push_back(LogEntry(Level, tag, message));
}

std::vector<LogEntry> Log::getLogs() const {
  std::lock_guard<std::mutex> lock(mtx);
  return logs;
}
std::vector<LogEntry> Log::getLogs(LogLevel Level) const {
  std::lock_guard<std::mutex> lock(mtx);
  std::vector<LogEntry> result;
  for (const auto &log : logs) { // 使用范围 for 循环
    if (log.level == Level) {
      result.push_back(log);
    }
  }
  return result;
}
std::vector<LogEntry> Log::getLogs(const std::string &tag) const {
  std::lock_guard<std::mutex> lock(mtx);
  std::vector<LogEntry> result;
  for (const auto &log : logs) { // 使用范围 for 循环
    if (log.tag == tag) {
      result.push_back(log);
    }
  }
  return result;
}
std::unordered_map<std::string, bool> Log::getTagMap() const {
  std::lock_guard<std::mutex> lock(mtx);
  return tag_map;
}
void Log::cleanLogs() {
  std::lock_guard<std::mutex> lock(mtx);
  logs.clear();
}
void Log::cleanLogs(LogLevel Level) {
  std::lock_guard<std::mutex> lock(mtx);
  logs.erase(std::remove_if(logs.begin(), logs.end(),
                            [&](const LogEntry &log) { return log.level == Level; }),
             logs.end());
}
void Log::cleanLogs(std::string tag) {
  std::lock_guard<std::mutex> lock(mtx);
  logs.erase(
      std::remove_if(logs.begin(), logs.end(), [&](const LogEntry &log) { return log.tag == tag; }),
      logs.end());
}
void Log::SaveToFile(const std::string &filename) {
  std::lock_guard<std::mutex> lock(mtx);
  std::ofstream ofs(filename, std::ios::out);
  if (!ofs.is_open()) {
    throw std::runtime_error("Failed to open file: " + filename);
    return;
  }
  for (const auto &log : logs) {
    ofs << std::format("[{}] [{}] [{}] {}", log.getFormattedTime(), LogLevelNames.at(log.level),
                       log.tag, log.message)
        << std::endl;
  }
  ofs.close();
}
Log g_log_tool;