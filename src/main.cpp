#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Bounce2.h>

#include "globals/globals.h"
#include "sd/sdGlobals.h"
#include "display/display.h"
#include "helpers/helpers.h"
#include "sd/sdState.h"
#include "sd/sdMenu.h"
#include "bt/bt.h"

enum AppMode { MODE_SELECT, MODE_SD, MODE_BLUETOOTH };
AppMode appMode = MODE_SELECT;

String modeItems[] = {"SD Card", "Bluetooth"};
const int modeItemCount = 2;
const char* bluetoothName = "HC Soundbox";

void initSdAudioOutput() {
    if(output == nullptr) {
        output = new AudioOutputI2S();
    }
    output->SetPinout(26, 25, 27);
    output->SetGain(volume / 100.0);
}

void releaseSdAudioOutput() {
    if(output != nullptr) {
        delete output;
        output = nullptr;
    }
}


void handleModeSelect() {
    int rotaryReadings = readRotary();
    if(rotaryReadings != 0) {
        updateMenuState(modeMenuState, modeItemCount, rotaryReadings);
        drawMenu("Select Mode", modeItems, modeItemCount, modeMenuState.selectedIndex, 0, selectedScroll);
    }

    swRotary.update();
    if(swRotary.pressed()) {
        if(modeMenuState.selectedIndex == 0) {
            appMode = MODE_SD;
            initSdAudioOutput();
            while(true) {
                showInsertSdMessage();
                awaitSdInit();
                if(updateDirContents(currentDir.c_str())) break;
            }
            menuState.selectedIndex = 0;
            menuState.scrollOffset = 0;
            drawFileMenu();
        }
        else {
            appMode = MODE_BLUETOOTH;
            releaseSdAudioOutput();
            startBluetoothMode(bluetoothName);
        }
    }
}

void drawModeMenu() {
    drawMenu("Select Mode", modeItems, modeItemCount, modeMenuState.selectedIndex, 0, selectedScroll);
}

void setup() {
    Serial.begin(115200);

    pinMode(clkPin, INPUT_PULLUP);
    pinMode(dtPin, INPUT_PULLUP);
    swRotary.attach(swPin, INPUT);
    swRotary.interval(debounceInterval);

    backBtn.attach(backBtnPin, INPUT_PULLUP);
    backBtn.interval(debounceInterval);

    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println("OLED failed");
        while (true);
    }
    Wire.setClock(400000);
    display.clearDisplay();
    display.display();

    drawModeMenu();
}

void loop() {
    if(appMode == MODE_SELECT) {
        handleModeSelect();
    }
    else if(appMode == MODE_SD) {
        if(handleSdMode()) {
            drawModeMenu();
            appMode = MODE_SELECT;
        }
    }
    else if(appMode == MODE_BLUETOOTH) {
        if(handleBluetoothMode()) {
            initSdAudioOutput();
            drawModeMenu();
            appMode = MODE_SELECT;
        }
    }
    delay(1);
}
