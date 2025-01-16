#pragma once
#include "NonOwnerPointer.hpp"
#include "gsl/gsl"
namespace Bedrock {
template <typename T> using NotNullNonOwnerPtr = gsl::not_null<NonOwnerPointer<T>>;
namespace Threading {
template <typename T, typename Alloc = std::allocator<T>> class ThreadLocalObject;
}
} // namespace Bedrock