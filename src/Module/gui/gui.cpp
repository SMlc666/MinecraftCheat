#include "gui.hpp"
#include "Module.hpp"
#include "config/config.hpp"
#include "imgui/imgui.h"
#include "rapidjson/document.h"
#include "rapidjson/rapidjson.h"
#include <format>
#include <functional>
void GUI::validateKeyExists(const std::string &key) {
  if (GUIMap_orig.find(key) == GUIMap_orig.end()) {
    throw std::runtime_error(std::format("GUIMap does not have {}", key));
  }
}
GUI::GUI(Module *m_module, const std::unordered_map<std::string, std::any> &m_GUIMap)
    : GUIMap_orig(m_GUIMap), module(m_module), first(m_module->getName()) {
  validateKeyExists("enabled");
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
  validateKeyExists(second);
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
  validateKeyExists(GUIMap_orig, second);
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
bool GUI::SliderFloat2(const std::string &second, const std::string &text, float min, float max,
                       const std::function<void(float, float)> &callback) {
  auto config = Config::getDocument()[first.c_str()].GetObject();
  validateKeyExists(GUIMap_orig, second);
  if (!Has(second)) {
    config.AddMember(rapidjson::Value(second.c_str(), Config::getDocument().GetAllocator()).Move(),
                     rapidjson::Value(rapidjson::kArrayType).Move(),
                     Config::getDocument().GetAllocator());
  }
  auto json_arr = config[second.c_str()].GetArray();
  if (json_arr.Size() < 2) {
    json_arr.PushBack(rapidjson::Value(any_cast<Vec2>(GUIMap_orig.at(second)).x).Move(),
                      Config::getDocument().GetAllocator());
    json_arr.PushBack(rapidjson::Value(any_cast<Vec2>(GUIMap_orig.at(second)).y).Move(),
                      Config::getDocument().GetAllocator());
  }
  std::array<float, 2> float_arr = {json_arr[0].GetFloat(), json_arr[1].GetFloat()};
  bool active = ImGui::SliderFloat2(text.c_str(), float_arr.data(), min, max);
  if (active) {
    json_arr[0].SetFloat(float_arr[0]);
    json_arr[1].SetFloat(float_arr[1]);
    if (callback) {
      callback(float_arr[0], float_arr[1]);
    }
  }
  return active;
}
bool GUI::CheckBox(const std::string &second, const std::string &text,
                   const std::function<void(bool)> &callback) {
  auto config = Config::getDocument()[first.c_str()].GetObject();
  validateKeyExists(second);
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
bool GUI::Has(const std::string &second) {
  auto config = Config::getDocument()[first.c_str()].GetObject();
  return config.HasMember(second.c_str());
}
GUI::Vec2 GUI::GetVec2(const std::string &second) {
  auto config = Config::getDocument()[first.c_str()].GetObject();
  if (!config.HasMember(second.c_str())) {
    throw std::runtime_error(std::format("does not have {}", second));
  }
  if (!config[second.c_str()].IsArray()) {
    throw std::runtime_error(std::format("{} is not an array", second));
  }
  auto json_arr = config[second.c_str()].GetArray();
  if (json_arr.Size() != 2) {
    throw std::runtime_error(std::format("{} does not have 2 elements", second));
  }
  return {json_arr[0].GetFloat(), json_arr[1].GetFloat()};
}
