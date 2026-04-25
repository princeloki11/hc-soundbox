#pragma once
#include <Arduino.h>

void startBluetoothMode(const char* deviceName);
bool handleBluetoothMode();
void stopBluetoothMode();
bool isBluetoothConnected();
