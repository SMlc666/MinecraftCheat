#pragma once
#include <any>
#include <string>
#include <unordered_map>
class Module;

class GUI {
public:
  GUI(Module *m_module, const std::unordered_map<std::string, std::any> &m_GUIMap);
  bool SliderInt(std::string &second, std::string &text, int min, int max);
  bool CheckBox(std::string &second, std::string &text);
  bool SliderFloat(std::string &second, std::string &text, float min, float max);
  template <typename T> T Get(std::string &second);

private:
  std::unordered_map<std::string, std::any> GUIMap_orig;
  Module *module;
  std::string first;
};