#include <fstream>
#include <string>
#include "Auth.hpp"
#include "rapidjson/document.h"
#include "rapidjson/error/en.h"
static const std::string AUTH_FILE = "/sdcard/MinecraftCheat/auth.txt";
static const std::string IMEI_FILE = "/sdcard/MinecraftCheat/imei.txt";
static const int SUCCESS_CODE = 722;
void AuthSetup() {
  std::ifstream file(AUTH_FILE);
  std::ifstream imei(IMEI_FILE);
  if (!file.is_open()) {
    throw std::runtime_error("Auth file not found");
  }
  if (!imei.is_open()) {
    throw std::runtime_error("IMEI file not found");
  }
  std::string auth;
  file >> auth;
  file.close();
  std::string imei_str;
  imei >> imei_str;
  imei.close();
  if (auth.empty() || imei_str.empty()) {
    throw std::runtime_error("Auth or IMEI is empty");
  }
  APIClient::Config config{.host = "http://wy.llua.cn/api/?id=kmlogon&app=39475&data=",
                           .appkey = "y75fd910cdf25abaaa0c510f03b45a38",
                           .id = "EzsEpt1Fsf1",
                           .apitoken = "ce8f2069c9c62e00eecef5bf009a864f",
                           .rc4key = "c4131f17d8f502a4caa8a809ba3f05410d1"};
  APIClient client(config);
  try {
    std::string result = client.executeRequest(auth, imei_str);
    rapidjson::Document result_doc;
    result_doc.Parse(result.c_str());
    if (result_doc.HasParseError()) {
      throw std::runtime_error(rapidjson::GetParseError_En(result_doc.GetParseError()));
    }
    int code = result_doc["code"].GetInt();
    if (code == SUCCESS_CODE) {
      return;
    }
  } catch (const std::exception &e) {
    throw std::runtime_error("Auth failed: " + std::string(e.what()));
  }
}