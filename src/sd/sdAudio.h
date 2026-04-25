#pragma once
#include <Arduino.h>

unsigned long getWAVLength(File file);
unsigned long getMP3Length(File file);
unsigned long getAudioLength(String fileLocation);
void audioTask(void* param);
void handleStartSong(String fileLocation, String fileName, String type);
void handlePause();
void handleResume();
