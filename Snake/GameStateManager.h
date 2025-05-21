#pragma once

#ifndef GAMESTATEMANAGER_H
#define GAMESTATEMANAGER_H

enum GameState { MENU, MENU_WITH_SAVED, PLAYING, PAUSED, GAME_OVER, CONFIRMATION, 
                 SETTINGS_MENU, SETTINGS_RESOLUTION, SETTINGS_SOUND, MODE_SELECTION };
enum ConfirmationType { CONFIRM_EXIT, CONFIRM_NEW_GAME, CONFIRM_RESIZE };
enum GameMode { NORMAL_MODE, FIRE_BORDER_MODE };

class GameStateManager {
public:
    GameStateManager() : state(MENU), confirmationType(CONFIRM_EXIT), hasSaved(false), mode(NORMAL_MODE) {}

    GameState getState() const { return state; }
    void setState(GameState s) { state = s; }

    ConfirmationType getConfirmationType() const { return confirmationType; }
    void setConfirmationType(ConfirmationType c) { confirmationType = c; }

    bool hasSavedSession() const { return hasSaved; }
    void setHasSavedSession(bool v) { hasSaved = v; }

    GameMode getMode() const { return mode; }
    void setMode(GameMode m) { mode = m; }

private:
    GameState state;
    ConfirmationType confirmationType;
    bool hasSaved;
    GameMode mode;
};

#endif
