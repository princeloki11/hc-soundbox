#include <vector>
#include "sdGlobals.h"
#include "sdState.h"
#include "../globals/globals.h"

void updateMenuState(MenuState& state, int itemCount, int rotaryDirection) {
    if(rotaryDirection == 1) {
        if(state.selectedIndex != itemCount - 1) {
            state.selectedIndex++;
        }
        else {
            state.selectedIndex = 0;
        }
    }
    else if(rotaryDirection == -1) {
        if(state.selectedIndex != 0) {
            state.selectedIndex--;
        }
        else {
            state.selectedIndex = itemCount - 1;
        }
    }

    if(state.selectedIndex < state.scrollOffset) {
        state.scrollOffset = state.selectedIndex;
    }
    if(state.selectedIndex >= state.scrollOffset + maxViewableItems) {
        state.scrollOffset = state.selectedIndex - maxViewableItems + 1;
    }

    selectedScroll.offset = 0;
}

bool updateDirContents(const char* workingDirName) {
    File workingDir = SD.open(workingDirName);

    if(!workingDir) {
        return false;
    }
    
    std::vector<String> fileNamesVec;
    std::vector<bool> isDirVec;

    while(true) {
        File entry = workingDir.openNextFile();
        if(!entry) break;
        fileNamesVec.push_back(entry.name());
        isDirVec.push_back(entry.isDirectory());
    }

    int fileCount = fileNamesVec.size();

    String* fileNames = new String[fileCount];
    bool* isDir = new bool[fileCount];

    for(int i = 0; i < fileCount; i++) {
        fileNames[i] = fileNamesVec[i];
        isDir[i] = isDirVec[i];
    }    
    
    dirContents.fileCount = fileCount;
    dirContents.fileNames = fileNames;
    dirContents.isDir = isDir;

    return true;
}
