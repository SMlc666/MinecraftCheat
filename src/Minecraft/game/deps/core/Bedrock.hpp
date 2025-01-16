#pragma once
#include "NonOwnerPointer.hpp"
#include "gsl/gsl"
namespace Bedrock {
template <typename T> using NotNullNonOwnerPtr = gsl::not_null<NonOwnerPointer<T>>;
}