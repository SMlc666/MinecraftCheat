#pragma once
#include <mutex>
#include <utility>
#include <vector>
#include <string>
#include <chrono>
#include <unordered_map>
enum LogLevel { DEBUG, INFO, WARN, ERROR, FATAL };
const std::unordered_map<LogLevel, std::string> LogLevelNames = {
    {DEBUG, "DEBUG"}, {INFO, "INFO"}, {WARN, "WARN"}, {ERROR, "ERROR"}, {FATAL, "FATAL"}};
static const std::string NormalLogFile = "/sdcard/MinecraftCheat/normal.log";
static const int MAX_TIME_LENGTH = 80;
struct LogEntry {
  LogLevel level;
  std::string tag;
  std::string message;
  std::chrono::system_clock::time_point time;
  LogEntry(LogLevel lvl, std::string timestamp, std::string msg)
      : level(lvl), tag(std::move(timestamp)), message(std::move(msg)) {
    time = std::chrono::system_clock::now();
  }
  std::string getFormattedTime() const {
    std::time_t now_time_t = std::chrono::system_clock::to_time_t(time);
    std::tm now_tm = *std::localtime(&now_time_t);
    char buffer[MAX_TIME_LENGTH];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &now_tm);
    return std::string(buffer);
  }
};

class Log {
private:
  std::vector<LogEntry> logs;
  std::unordered_map<std::string, bool> tag_map;
  mutable std::mutex mtx;

public:
  void message(LogLevel Level, const std::string &tag, const std::string &message);
  template <typename... Args>
  void message(LogLevel Level, const std::string &tag, const std::string &fmt, Args... args) {
    std::string message = std::format(fmt, std::forward<Args>(args)...);
    this->message(Level, tag, message);
  }
  std::vector<LogEntry> getLogs() const;
  std::vector<LogEntry> getLogs(LogLevel Level) const;
  std::vector<LogEntry> getLogs(const std::string &tag) const;
  std::unordered_map<std::string, bool> getTagMap() const;
  void cleanLogs();
  void cleanLogs(LogLevel Level);
  void cleanLogs(std::string tag);
  void SaveToFile(const std::string &filename = NormalLogFile);
};
extern Log g_log_tool;