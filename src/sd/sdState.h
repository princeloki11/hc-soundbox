#pragma once
#include <Arduino.h>

bool updateDirContents(const char* workingDirName);
void updateMenuState(MenuState& state, int itemCount, int rotaryDirection);
