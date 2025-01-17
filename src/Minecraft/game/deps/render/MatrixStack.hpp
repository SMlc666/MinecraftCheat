#pragma once
#include "base/alias.hpp"
//NOLINTBEGIN
class MatrixStack {
public:
  struct MatrixStackRef {
  public:
    alias::UntypedStorage<8, 8> mUnka8f7f2;
    alias::UntypedStorage<8, 8> mUnk88f465;

  public:
    MatrixStackRef &operator=(MatrixStackRef const &);
    MatrixStackRef(MatrixStackRef const &);
    MatrixStackRef();
  };

public:
  MatrixStack &operator=(MatrixStack const &);
  MatrixStack(MatrixStack const &);
  MatrixStack();
};
//NOLINTEND