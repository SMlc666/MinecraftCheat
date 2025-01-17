#include "SpinAttack.hpp"
#include "MemTool.hpp"
#include "Module.hpp"
#include "menu/menu.hpp"
#include <unordered_map>
static const uintptr_t SpinAttack_pattern = 0x734A0C8;
static const uintptr_t velX_pattern = 0x733EFCC;
static const uintptr_t velY_pattern = 0x733EFD0;
static const uintptr_t rag1_pattern = 0x733EE0C;
static const uintptr_t rag2_pattern = 0x733EE10;
static const uintptr_t rag3_pattern = 0x733EE14;
static const uintptr_t rag4_pattern = 0x733EE30;
static std::byte SpinAttack_backup[4];
static std::byte vel_backup[8];
static std::byte rag1_backup[4];
static std::byte rag2_backup[4];
static std::byte rag3_backup[4];
static std::byte rag4_backup[4];
static std::unordered_map<std::string, std::any> ConfigData = {
    {"enabled", false},
    {"velocity", false},
    {"range", 4.0F},
};
static void velPatch() {
  MemTool::writeASM(velX_pattern, "NOP");
  MemTool::writeASM(velY_pattern, "NOP");
}
static void velRestore() {
  MemTool::write(velX_pattern, &vel_backup, sizeof(vel_backup));
}
static void ragPatch(float range) {
  MemTool::writeASM(rag1_pattern, std::format("FMOV            S3, #-{:.1f}", range));
  MemTool::writeASM(rag2_pattern, std::format("FMOV            V1.2S, #-{:.1f}", range));
  MemTool::writeASM(rag3_pattern, std::format("FMOV            V5.2S, #{:.1f}", range));
  MemTool::writeASM(rag4_pattern, std::format(" FMOV            S3, #{:.1f}", range));
}
static void ragRestore() {
  MemTool::write(rag1_pattern, &rag1_backup, sizeof(rag1_backup));
  MemTool::write(rag2_pattern, &rag2_backup, sizeof(rag2_backup));
  MemTool::write(rag3_pattern, &rag3_backup, sizeof(rag3_backup));
  MemTool::write(rag4_pattern, &rag4_backup, sizeof(rag4_backup));
}

static void SpinAttackPatch() {
  MemTool::writeASM(SpinAttack_pattern, "NOP");
}
static void SpinAttackRestore() {
  MemTool::write(SpinAttack_pattern, &SpinAttack_backup, sizeof(SpinAttack_backup));
}
SpinAttack::SpinAttack() : Module("SpinAttack", MenuType::COMBAT_MENU, ConfigData) {
  MemTool::read(velX_pattern, &vel_backup, sizeof(vel_backup));
  MemTool::read(SpinAttack_pattern, &SpinAttack_backup, sizeof(SpinAttack_backup));
  MemTool::read(rag1_pattern, &rag1_backup, sizeof(rag1_backup));
  MemTool::read(rag2_pattern, &rag2_backup, sizeof(rag2_backup));
  MemTool::read(rag3_pattern, &rag3_backup, sizeof(rag3_backup));
  MemTool::read(rag4_pattern, &rag4_backup, sizeof(rag4_backup));
  setOnEnable([](Module *module) {
    SpinAttackPatch();
    ragPatch(module->getGUI().Get<float>("range"));
    velPatch();
  });
  setOnDisable([](Module *module) {
    SpinAttackRestore();
    ragRestore();
    velRestore();
  });
  setOnDraw([](Module *module) {
    module->getGUI().CheckBox("velocity", "velocity", [module](bool value) {
      if (module->getGUI().Get<bool>("enabled")) {
        if (value) {
          velPatch();
        } else {
          velRestore();
        }
      }
    });
    module->getGUI().SliderFloat("range", "range", 0.0F, 10.0F, [module](float value) {
      if (module->getGUI().Get<bool>("enabled")) {
        ragPatch(value);
      }
    });
  });
}