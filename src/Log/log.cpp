#include "log.hpp"
void Log::message(LogLevel Level, const std::string &tag, const std::string &message) {
  logs.push_back(LogEntry(Level, tag, message));
}

std::vector<LogEntry> Log::getLogs() const {
  return logs;
}
std::vector<LogEntry> Log::getLogs(LogLevel Level) const {
  std::vector<LogEntry> result;
  for (const auto &log : logs) { // 使用范围 for 循环
    if (log.level == Level) {
      result.push_back(log);
    }
  }
  return result;
}

std::vector<LogEntry> Log::getLogs(std::string tag) const {
  std::vector<LogEntry> result;
  for (const auto &log : logs) { // 使用范围 for 循环
    if (log.tag == tag) {
      result.push_back(log);
    }
  }
  return result;
}

void Log::cleanLogs() {
  logs.clear();
}
void Log::cleanLogs(LogLevel Level) {
  logs.erase(std::remove_if(logs.begin(), logs.end(),
                            [&](const LogEntry &log) { return log.level == Level; }),
             logs.end());
}
void Log::cleanLogs(std::string tag) {
  logs.erase(
      std::remove_if(logs.begin(), logs.end(), [&](const LogEntry &log) { return log.tag == tag; }),
      logs.end());
}
void Log::SaveToFile(const std::string &filename) const {
  std::ofstream ofs(filename, std::ios::out);
  if (!ofs.is_open()) {
    throw std::runtime_error("Failed to open file: " + filename);
  }
  for (const auto &log : logs) {
    ofs << log.level << " " << log.tag << " " << log.message << std::endl;
  }
  ofs.close();
}
Log g_log_tool;