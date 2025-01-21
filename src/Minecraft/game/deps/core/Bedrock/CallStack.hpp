#pragma once
#include "base/mcint.hpp"
#include "game/deps/core/Bedrock/LogLevel.hpp"
#include "game/deps/core/enums/LogAreaID.hpp"
#include <optional>
#include <vector>
#include <string>
namespace Bedrock {
struct CallStack {
  struct Context {
  public:
    std::string mMessage;
    std::optional<Bedrock::LogLevel> mLogLevel;
    std::optional<LogAreaID> mLogAreaID;
  };
  struct Frame {
    uint64 mFilenameHash;
    std::string_view mFilename;
    uint32_t mLine;
  };
  struct FrameWithContext {
  public:
    Bedrock::CallStack::Frame mFrame;
    std::optional<Bedrock::CallStack::Context> mContext;
  };

public:
  std::vector<Bedrock::CallStack::FrameWithContext> vector;
};
} // namespace Bedrock