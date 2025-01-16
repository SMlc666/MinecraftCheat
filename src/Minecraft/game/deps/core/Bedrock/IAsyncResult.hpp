#pragma once

#include <memory>
namespace Bedrock::Threading {
template <typename T0>
class IAsyncResult : public std::enable_shared_from_this<IAsyncResult<T0>> {};
} // namespace Bedrock::Threading
