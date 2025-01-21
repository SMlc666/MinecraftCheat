#pragma once
#include "nonstd/expected.hpp"
#include "ErrorInfo.hpp"
struct success_t {};
constexpr success_t success{};
namespace Bedrock {
template <typename T, typename Err> class Result : public nonstd::expected<T, ErrorInfo<Err>> {
public:
  using Base = nonstd::expected<T, ErrorInfo<Err>>;
  using Base::Base;
};
template <typename Err>
class Result<void, Err> : public nonstd::expected_lite::expected<success_t, ErrorInfo<Err>> {
  using Base = nonstd::expected_lite::expected<success_t, ErrorInfo<Err>>;
  using Base::Base;
};
}; // namespace Bedrock
