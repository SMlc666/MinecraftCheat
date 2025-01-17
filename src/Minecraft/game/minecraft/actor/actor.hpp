#pragma once
#include "actorflags.hpp"
class HashedString;
class Actor {
public:
  virtual bool getStatusFlag(ActorFlags);
  virtual void setStatusFlag(ActorFlags, bool);
  virtual bool hasComponent(HashedString const &name) const;
  virtual void outOfWorld();
};