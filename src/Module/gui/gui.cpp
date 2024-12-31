#include "gui.hpp"
#include "config/config.hpp"
#include "imgui/imgui.h"
#include "rapidjson/document.h"
GUI::GUI(std::shared_ptr<Module> &m_module,
         const std::unordered_map<std::string, std::any> &m_GUIMap)
    : GUIMap_orig(m_GUIMap), module(m_module), first(m_module->getName()) {
  if (GUIMap_orig.find("enabled") == GUIMap_orig.end()) {
    GUIMap_orig.insert(std::make_pair("enabled", false));
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
  if (GUIMap_orig.find(second) == GUIMap_orig.end()) {
    throw std::runtime_error("GUIMap does not have " + second);
  }
  if (!config.HasMember(second.c_str())) {
    config.AddMember(rapidjson::Value(second.c_str(), Config::getDocument().GetAllocator()).Move(),
                     rapidjson::Value(any_cast<int>(GUIMap_orig.at(second))).Move(),
                     Config::getDocument().GetAllocator());
  }
  int v = config[second.c_str()].GetInt();
  bool active = ImGui::SliderInt(text.c_str(), &v, min, max);
  if (active) {
    config[second.c_str()].SetInt(v);
  }
  return active;
}
bool GUI::CheckBox(std::string &second, std::string &text) {
  auto config = Config::getDocument()[first.c_str()].GetObject();
  if (GUIMap_orig.find(second) == GUIMap_orig.end()) {
    throw std::runtime_error("GUIMap does not have " + second);
  }
  if (!config.HasMember(second.c_str())) {
    config.AddMember(rapidjson::Value(second.c_str(), Config::getDocument().GetAllocator()).Move(),
                     rapidjson::Value(any_cast<bool>(GUIMap_orig.at(second))).Move(),
                     Config::getDocument().GetAllocator());
  }
  bool v = config[second.c_str()].GetBool();
  bool active = ImGui::Checkbox(text.c_str(), &v);
  if (active) {
    config[second.c_str()].SetBool(v);
  }
  return active;
}

template <typename T> T GUI::Get(std::string &second) {
  auto config = Config::getDocument()[first.c_str()].GetObject();
  if (!config.HasMember(second.c_str())) {
    throw std::runtime_error("config does not have " + second);
  }
  return config[second.c_str()].Get<T>();
}
