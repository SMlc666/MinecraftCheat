#pragma once
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <exception>
enum LogLevel { DEBUG, INFO, WARN, ERROR, FATAL };
struct LogEntry {
  LogLevel level;
  std::string tag;
  std::string message;
  LogEntry(LogLevel lvl, const std::string &t, const std::string &msg)
      : level(lvl), tag(t), message(msg) {
  }
};

class Log {
private:
  std::vector<LogEntry> logs;

public:
  void message(LogLevel level, const std::string &tag, const std::string &msg);
  std::vector<LogEntry> getLogs() const;
  std::vector<LogEntry> getLogs(LogLevel Level) const;
  std::vector<LogEntry> getLogs(std::string tag) const;
  void cleanLogs();
  void cleanLogs(LogLevel Level);
  void cleanLogs(std::string tag);
  void SaveToFile(const std::string &filename) const;
};
extern Log g_log_tool;