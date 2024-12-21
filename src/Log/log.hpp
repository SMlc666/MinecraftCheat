#pragma once
#include <vector>
#include <string>
#include <chrono>
#include <unordered_map>
enum LogLevel { DEBUG, INFO, WARN, ERROR, FATAL };
const std::unordered_map<LogLevel, std::string> LogLevelNames = {
    {DEBUG, "DEBUG"}, {INFO, "INFO"}, {WARN, "WARN"}, {ERROR, "ERROR"}, {FATAL, "FATAL"}};
static const std::string NormalLogFile = "/sdcard/MinecraftCheat/normal.log";
struct LogEntry {
  LogLevel level;
  std::string tag;
  std::string message;
  std::chrono::system_clock::time_point time;
  LogEntry(LogLevel lvl, const std::string &t, const std::string &msg)
      : level(lvl), tag(t), message(msg) {
    time = std::chrono::system_clock::now();
  }
  std::string getFormattedTime() const {
    std::time_t now_time_t = std::chrono::system_clock::to_time_t(time);
    std::tm now_tm = *std::localtime(&now_time_t);
    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &now_tm);
    return std::string(buffer);
  }
};

class Log {
private:
  std::vector<LogEntry> logs;
  std::unordered_map<std::string, bool> tag_map;

public:
  void message(LogLevel level, const std::string &tag, const std::string &msg);
  const std::vector<LogEntry> getLogs() const;
  const std::vector<LogEntry> getLogs(LogLevel Level) const;
  const std::vector<LogEntry> getLogs(std::string tag) const;
  const std::unordered_map<std::string, bool> getTagMap() const;
  void cleanLogs();
  void cleanLogs(LogLevel Level);
  void cleanLogs(std::string tag);
  void SaveToFile(const std::string &filename = NormalLogFile);
};
extern Log g_log_tool;