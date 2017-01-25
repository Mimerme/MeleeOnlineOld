// Copyright 2015 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once

#include <string>
#include "InputCommon/ControllerEmu.h"
#include "InputCommon/InputConfig.h"

enum Hotkey
{
  //MELEE NET Hotkeys
  HK_MELEE_TEST,
  HK_MELEE_DOWN,
  HK_MELEE_UP,
  HK_MELEE_SELECT,
  HK_MELEE_BACK,

  HK_FULLSCREEN,
  HK_SCREENSHOT,
  HK_EXIT,

  NUM_HOTKEYS,
};

struct HotkeyStatus
{
  u32 button[(NUM_HOTKEYS + 31) / 32];
  s8 err;
};

class HotkeyManager : public ControllerEmu
{
public:
  HotkeyManager();
  ~HotkeyManager();

  void GetInput(HotkeyStatus* const hk);
  std::string GetName() const override;
  void LoadDefaults(const ControllerInterface& ciface) override;

private:
  Buttons* m_keys[(NUM_HOTKEYS + 31) / 32];
  ControlGroup* m_options;
};

namespace HotkeyManagerEmu
{
void Initialize();
void Shutdown();
void LoadConfig();

InputConfig* GetConfig();
void GetStatus();
bool IsEnabled();
void Enable(bool enable_toggle);
bool IsPressed(int Id, bool held);
}
