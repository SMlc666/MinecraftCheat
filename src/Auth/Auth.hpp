#pragma once
#include <string>
#include <sstream>
#include <iomanip>
#include <chrono>
#include "RC4/ARC4.h"
#include "md5/md5.h"
#include "httplib/httplib.h"
class APIClient {
public:
  struct Config {
    std::string host;
    std::string appkey;
    std::string id;
    std::string apitoken;
    std::string rc4key;
  };

  APIClient(const Config &config) : m_config(config) {
  }

  std::string executeRequest(const std::string &kami, const std::string &markcode) {
    // 生成请求要素
    auto timestamp = generateTimestamp();
    auto sign = generateSign(kami, markcode, timestamp);
    auto value = generateRandomValue();

    // 构建并加密请求数据
    auto postData = buildPostData(kami, markcode, timestamp, sign, value);
    auto encrypted = encryptData(postData);

    // 发送请求并处理响应
    auto response = sendRequest(encrypted);
    return decryptResponse(response);
  }

private:
  Config m_config;

  // 工具方法
  static std::string bytesToHex(const char *buffer, size_t length) {
    std::stringstream ss;
    ss << std::hex << std::uppercase;
    for (size_t i = 0; i < length; ++i) {
      ss << std::setw(2) << std::setfill('0') << static_cast<int>(buffer[i] & 0xFF);
    }
    return ss.str();
  }

  static std::vector<unsigned char> hexStringToBytes(const std::string &hexString) {
    if (hexString.length() % 2 != 0) {
      throw std::runtime_error("Invalid hex string length");
    }

    std::vector<unsigned char> bytes;
    for (size_t i = 0; i < hexString.length(); i += 2) {
      unsigned char byte = 0;
      for (int j = 0; j < 2; j++) {
        char c = hexString[i + j];
        byte <<= 4;
        if (c >= '0' && c <= '9')
          byte |= c - '0';
        else if (c >= 'A' && c <= 'F')
          byte |= c - 'A' + 10;
        else if (c >= 'a' && c <= 'f')
          byte |= c - 'a' + 10;
        else
          throw std::runtime_error("Invalid hex character");
      }
      bytes.push_back(byte);
    }
    return bytes;
  }

  // 请求要素生成
  long generateTimestamp() const {
    return std::chrono::duration_cast<std::chrono::seconds>(
               std::chrono::system_clock::now().time_since_epoch())
        .count();
  }

  std::string generateSign(const std::string &kami, const std::string &markcode,
                           long timestamp) const {
    std::string sign_str = "kami=" + kami + "&markcode=" + markcode +
                           "&t=" + std::to_string(timestamp) + "&" + m_config.appkey;
    return MD5(sign_str.c_str()).toStr();
  }

  std::string generateRandomValue() const {
    return std::to_string(rand() % 10000);
  }

  // 请求构建
  std::string buildPostData(const std::string &kami, const std::string &markcode, long timestamp,
                            const std::string &sign, const std::string &value) const {
    return "kami=" + kami + "&markcode=" + markcode + "&t=" + std::to_string(timestamp) +
           "&sign=" + sign + "&value=" + value;
  }

  // 加密解密
  std::string encryptData(const std::string &data) {
    ARC4 rc4;
    rc4.setKey((unsigned char *)m_config.rc4key.c_str(), m_config.rc4key.length());

    std::unique_ptr<char[]> buffer(new char[data.length()]);
    rc4.encrypt((char *)data.c_str(), buffer.get(), data.length());
    return bytesToHex(buffer.get(), data.length());
  }

  std::string decryptResponse(const std::string &response) {
    ARC4 rc4;
    rc4.setKey((unsigned char *)m_config.rc4key.c_str(), m_config.rc4key.length());

    auto bytes = hexStringToBytes(response);
    std::unique_ptr<char[]> buffer(new char[bytes.size() + 1]{0});
    rc4.encrypt((char *)bytes.data(), buffer.get(), bytes.size());
    return std::string(buffer.get(), bytes.size());
  }

  // 修改 sendRequest 方法
  std::string sendRequest(const std::string &encryptedData) {
    // 解析主机和路径
    std::string url = m_config.host + encryptedData;
    size_t pos = url.find("://");
    if (pos == std::string::npos) {
      throw std::runtime_error("Invalid URL format");
    }
    pos += 3; // 跳过 "://"
    size_t path_pos = url.find('/', pos);
    std::string host = url.substr(pos, path_pos - pos);
    std::string path = url.substr(path_pos);

    // 创建 httplib 客户端
    httplib::Client client(host.c_str());

    // 发送 GET 请求
    auto response = client.Get(path.c_str());
    if (!response || response->status != 200) {
      throw std::runtime_error("HTTP request failed: " +
                               (response ? std::to_string(response->status) : "No response"));
    }

    // 返回响应内容
    return response->body;
  }
};

void AuthSetup();