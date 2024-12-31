#include "gui.hpp"
#include "config/config.hpp"
#include "imgui/imgui.h"
#include "rapidjson/document.h"
GUI::GUI(std::shared_ptr<Module> &m_module,
         const std::unordered_map<std::string, std::any> &m_GUIMap)
    : GUIMap(m_GUIMap), module(m_module), first(m_module->getName()) {
  if (GUIMap.find("enabled") == GUIMap.end()) {
    GUIMap.insert(std::make_pair("enabled", false));
  }
  if ((!Config::getDocument().HasMember(first.c_str())) ||
      (!Config::getDocument()[first.c_str()].IsObject())) {
    rapidjson::Document doc;
    doc.SetObject();
    rapidjson::Value name(first.c_str(), Config::getDocument().GetAllocator());
    Config::getDocument().AddMember(name.Move(), doc.Move(), Config::getDocument().GetAllocator());
  }
}
bool GUI::SliderInt(std::string &second, std::string &text, int min, int max) {
  auto config = Config::getDocument()[first.c_str()].GetObject();

  if (!config.HasMember(second.c_str())) {
    config.AddMember(rapidjson::Value(second.c_str(), Config::getDocument().GetAllocator()).Move(),
                     rapidjson::Value(0).Move(), Config::getDocument().GetAllocator());
  }

  int v = config[second.c_str()].GetInt();
  bool active = ImGui::SliderInt(text.c_str(), &v, min, max);
  if (active) {
    config[second.c_str()].SetInt(v);
  }
  return active;
}