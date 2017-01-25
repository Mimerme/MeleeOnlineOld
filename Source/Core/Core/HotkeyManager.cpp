// Copyright 2015 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#include <string>
#include <vector>

#include "Common/Common.h"
#include "Core/ConfigManager.h"
#include "Core/HotkeyManager.h"
#include "InputCommon/GCPadStatus.h"

const std::string hotkey_labels[] = {
	_trans("MeleeNET Activate Hook"),
	_trans("MeleeNET Down"),
	_trans("MeleeNET Up"),
	_trans("MeleeNET Select"),
	_trans("MeleeNET Back"),

    _trans("Toggle Fullscreen"),
    _trans("Take Screenshot"),
    _trans("Exit"),
};
static_assert(NUM_HOTKEYS == sizeof(hotkey_labels) / sizeof(hotkey_labels[0]),
              "Wrong count of hotkey_labels");

namespace HotkeyManagerEmu
{
static u32 s_hotkeyDown[(NUM_HOTKEYS + 31) / 32];
static HotkeyStatus s_hotkey;
static bool s_enabled;

static InputConfig s_config("Hotkeys", _trans("Hotkeys"), "Hotkeys");

InputConfig* GetConfig()
{
  return &s_config;
}

void GetStatus()
{
  s_hotkey.err = PAD_ERR_NONE;

  // Get input
  static_cast<HotkeyManager*>(s_config.GetController(0))->GetInput(&s_hotkey);
}

bool IsEnabled()
{
  return s_enabled;
}

void Enable(bool enable_toggle)
{
  s_enabled = enable_toggle;
}

bool IsPressed(int Id, bool held)
{
  unsigned int set = Id / 32;
  unsigned int setKey = Id % 32;
  if (s_hotkey.button[set] & (1 << setKey))
  {
    bool pressed = !!(s_hotkeyDown[set] & (1 << setKey));
    s_hotkeyDown[set] |= (1 << setKey);
    if (!pressed || held)
      return true;
  }
  else
  {
    s_hotkeyDown[set] &= ~(1 << setKey);
  }

  return false;
}

void Initialize()
{
  if (s_config.ControllersNeedToBeCreated())
    s_config.CreateController<HotkeyManager>();

  g_controller_interface.RegisterHotplugCallback(LoadConfig);

  // load the saved controller config
  s_config.LoadConfig(true);

  for (u32& key : s_hotkeyDown)
    key = 0;

  s_enabled = true;
}

void LoadConfig()
{
  s_config.LoadConfig(true);
}

void Shutdown()
{
  s_config.ClearControllers();
}
}

HotkeyManager::HotkeyManager()
{
  for (int key = 0; key < NUM_HOTKEYS; key++)
  {
    int set = key / 32;

    if (key % 32 == 0)
      groups.emplace_back(m_keys[set] = new Buttons(_trans("Keys")));

    m_keys[set]->controls.emplace_back(new ControlGroup::Input(hotkey_labels[key]));
  }

  groups.emplace_back(m_options = new ControlGroup(_trans("Options")));
  m_options->boolean_settings.emplace_back(
      std::make_unique<ControlGroup::BackgroundInputSetting>(_trans("Background Input")));
  m_options->boolean_settings.emplace_back(std::make_unique<ControlGroup::BooleanSetting>(
      _trans("Iterative Input"), false, ControlGroup::SettingType::VIRTUAL));
}

HotkeyManager::~HotkeyManager()
{
}

std::string HotkeyManager::GetName() const
{
  return std::string("Hotkeys") + char('1' + 0);
}

void HotkeyManager::GetInput(HotkeyStatus* const kb)
{
  auto lock = ControllerEmu::GetStateLock();
  for (int set = 0; set < (NUM_HOTKEYS + 31) / 32; set++)
  {
    std::vector<u32> bitmasks;
    for (int key = 0; key < std::min(32, NUM_HOTKEYS - set * 32); key++)
      bitmasks.push_back(1 << key);

    kb->button[set] = 0;
    m_keys[set]->GetState(&kb->button[set], bitmasks.data());
  }
}

void HotkeyManager::LoadDefaults(const ControllerInterface& ciface)
{
  ControllerEmu::LoadDefaults(ciface);

#ifdef _WIN32
  const std::string NON = "(!(LMENU | RMENU) & !(LSHIFT | RSHIFT) & !(LCONTROL | RCONTROL))";
  const std::string ALT = "((LMENU | RMENU) & !(LSHIFT | RSHIFT) & !(LCONTROL | RCONTROL))";
  const std::string SHIFT = "(!(LMENU | RMENU) & (LSHIFT | RSHIFT) & !(LCONTROL | RCONTROL))";
  const std::string CTRL = "(!(LMENU | RMENU) & !(LSHIFT | RSHIFT) & (LCONTROL | RCONTROL))";
#else
  const std::string NON = "(!`Alt_L` & !(`Shift_L` | `Shift_R`) & !(`Control_L` | `Control_R` ))";
  const std::string ALT = "(`Alt_L` & !(`Shift_L` | `Shift_R`) & !(`Control_L` | `Control_R` ))";
  const std::string SHIFT = "(!`Alt_L` & (`Shift_L` | `Shift_R`) & !(`Control_L` | `Control_R` ))";
  const std::string CTRL = "(!`Alt_L` & !(`Shift_L` | `Shift_R`) & (`Control_L` | `Control_R` ))";
#endif

  auto set_key_expression = [this](int index, const std::string& expression) {
    m_keys[index / 32]->controls[index % 32]->control_ref->expression = expression;
  };

  set_key_expression(HK_MELEE_TEST, "F1");
  set_key_expression(HK_MELEE_SELECT, "RETURN");
  set_key_expression(HK_MELEE_UP, "UP");
  set_key_expression(HK_MELEE_DOWN, "DOWN");


}
