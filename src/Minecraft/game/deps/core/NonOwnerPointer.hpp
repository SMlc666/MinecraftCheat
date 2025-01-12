#pragma once
#include <memory>
namespace Bedrock {
class EnableNonOwnerReferences {
public:
  struct ControlBlock {
    EnableNonOwnerReferences *ptr;

    explicit ControlBlock(EnableNonOwnerReferences *ptr) : ptr(ptr) {
    }
  };

private:
  std::shared_ptr<ControlBlock> controlBlock;

public:
  EnableNonOwnerReferences() : controlBlock(std::make_shared<ControlBlock>(this)) {
  }

  EnableNonOwnerReferences(const EnableNonOwnerReferences &other) = default;

  virtual ~EnableNonOwnerReferences() = default;

  EnableNonOwnerReferences &operator=(const EnableNonOwnerReferences &other) = default;
};
} // namespace Bedrock