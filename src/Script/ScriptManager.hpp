#pragma once
#include <string>
#include <vector>
#include <mutex>
static const std::string NormalScriptPath = "/sdcard/MinecraftCheat/Scripts";
class Script {
public:
private:
};
namespace ScriptManager {
struct ScriptInfo {
  std::string file;
};
void reloadScripts(const std::filesystem::path &path);
const std::vector<ScriptInfo> &getScripts();

} // namespace ScriptManager
void ScriptSetup();