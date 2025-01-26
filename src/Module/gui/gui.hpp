#pragma once
#include "config/config.hpp"
#include <any>
#include <functional>
#include <string>
#include <unordered_map>
class Module;
class GUI {
public:
  struct Vec2 {
    float x, y;
  };
  struct Color {
    float r, g, b, a;
  };

public:
  GUI(Module *m_module, const std::unordered_map<std::string, std::any> &m_GUIMap);
  bool SliderInt(const std::string &second, const std::string &text, int min, int max,
                 const std::function<void(int)> &callback = nullptr);
  bool CheckBox(const std::string &second, const std::string &text,
                const std::function<void(bool)> &callback = nullptr);
  bool SliderFloat(const std::string &second, const std::string &text, float min, float max,
                   const std::function<void(float)> &callback = nullptr);
  bool SliderFloat2(const std::string &second, const std::string &text, float min, float max,
                    const std::function<void(float, float)> &callback = nullptr);
  bool ColorPicker(const std::string &second, const std::string &text,
                   const std::function<void(Color)> &callback = nullptr);
  template <typename T> inline T Get(const std::string &second) {
    auto config = Config::getDocument()[first.c_str()].GetObject();
    if (!config.HasMember(second.c_str())) {
      throw std::runtime_error("config does not have " + second);
    }
    return config[second.c_str()].Get<T>();
  }
  Vec2 GetVec2(const std::string &second);
  template <typename T> inline void Set(const std::string &second, const T &value) {
    auto config = Config::getDocument()[first.c_str()].GetObject();
    config[second.c_str()].Set(value);
  }
  bool Has(const std::string &second);

private:
  std::unordered_map<std::string, std::any> GUIMap_orig;
  Module *module;
  std::string first;

private:
  void validateKeyExists(const std::string &second);
};