#pragma once
#include "config/config.hpp"
#include <any>
#include <string>
#include <unordered_map>
class Module;

class GUI {
public:
  GUI(Module *m_module, const std::unordered_map<std::string, std::any> &m_GUIMap);
  bool SliderInt(const std::string &second, const std::string &text, int min, int max);
  bool CheckBox(const std::string &second, const std::string &text);
  bool SliderFloat(const std::string &second, const std::string &text, float min, float max);
  template <typename T> inline T Get(const std::string &second) {
    auto config = Config::getDocument()[first.c_str()].GetObject();
    if (!config.HasMember(second.c_str())) {
      throw std::runtime_error("config does not have " + second);
    }
    return config[second.c_str()].Get<T>();
  }

private:
  std::unordered_map<std::string, std::any> GUIMap_orig;
  Module *module;
  std::string first;
};