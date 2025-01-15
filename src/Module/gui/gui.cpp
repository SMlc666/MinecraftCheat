#include "gui.hpp"
#include "Module.hpp"
#include "config/config.hpp"
#include "imgui/imgui.h"
#include "rapidjson/document.h"
#include <functional>
GUI::GUI(Module *m_module, const std::unordered_map<std::string, std::any> &m_GUIMap)
    : GUIMap_orig(m_GUIMap), module(m_module), first(m_module->getName()) {
  if (GUIMap_orig.find("enabled") == GUIMap_orig.end()) {
    throw std::runtime_error("GUIMap does not have enabled");
  }
  if ((!Config::getDocument().HasMember(first.c_str())) ||
      (!Config::getDocument()[first.c_str()].IsObject())) {
    rapidjson::Document doc;
    doc.SetObject();
    rapidjson::Value name(first.c_str(), Config::getDocument().GetAllocator());
    Config::getDocument().AddMember(name.Move(), doc.Move(), Config::getDocument().GetAllocator());
  }
}
bool GUI::SliderInt(const std::string &second, const std::string &text, int min, int max,
                    const std::function<void(int)> &callback) {
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
    if (callback) {
      callback(v);
    }
  }
  return active;
}
bool GUI::SliderFloat(const std::string &second, const std::string &text, float min, float max,
                      const std::function<void(float)> &callback) {
  auto config = Config::getDocument()[first.c_str()].GetObject();
  if (GUIMap_orig.find(second) == GUIMap_orig.end()) {
    throw std::runtime_error("GUIMap does not have " + second);
  }
  if (!config.HasMember(second.c_str())) {
    config.AddMember(rapidjson::Value(second.c_str(), Config::getDocument().GetAllocator()).Move(),
                     rapidjson::Value(any_cast<float>(GUIMap_orig.at(second))).Move(),
                     Config::getDocument().GetAllocator());
  }
  float v = config[second.c_str()].GetFloat();
  bool active = ImGui::SliderFloat(text.c_str(), &v, min, max);
  if (active) {
    config[second.c_str()].SetFloat(v);
    if (callback) {
      callback(v);
    }
  }
  return active;
}
bool GUI::CheckBox(const std::string &second, const std::string &text,
                   const std::function<void(bool)> &callback) {
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
    if (callback) {
      callback(v);
    }
  }
  return active;
}
