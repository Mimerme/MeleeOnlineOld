#include "MeleeNETUI.h"

int MeleeNETUI::gameWindowWidth = 0;
int MeleeNETUI::gameWindowHeight = 0;
bool MeleeNETUI::uiActive = false;
std::string MeleeNETUI::menuOptions[4];
std::string MeleeNETUI::matchOptions[3];
std::string MeleeNETUI::about[3];
std::string MeleeNETUI::matchConfig[4];
std::string MeleeNETUI::options[3];
std::string MeleeNETUI::lookingForMatch[3];

int MeleeNETUI::SCENE_STATE = 0;
int MeleeNETUI::MAX_OPTIONS = 4;
int MeleeNETUI::activeOption = 1;
int MeleeNETUI::activeColor = 0xffffff66;