#pragma once
#include <string>
class ILevel;
class DimensionType;
class DimensionHeightRange;
class Scheduler;
class Dimension {
public:
public:
  Dimension *Dimension_(ILevel &level, DimensionType dimId, DimensionHeightRange heightRange,
                        Scheduler &callbackContext, std::string dimensionName);

public:
  virtual ~Dimension();
};