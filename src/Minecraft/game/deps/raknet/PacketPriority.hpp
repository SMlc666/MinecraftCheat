#pragma once
enum class PacketPriority : int {
  ImmediatePriority = 0,
  HighPriority = 1,
  MediumPriority = 2,
  LowPriority = 3,
  NumberOfPriorities = 4,
};
