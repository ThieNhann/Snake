#pragma once

#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H

#include <string>
#include <vector>
#include <fstream>
#include <filesystem>

#include "Config.h"
#include "Snake.h"
#include "ObjectManager.h"
#include "Settings.h"

namespace fs = std::filesystem;

// Handles saving and loading game sessions and high scores
class SaveManager {
public:
    // Validates the save file format and content
    static bool validate(const std::string& filename);

    // Saves the full game session to file
    static void saveSessionToFile(
        const std::string& filename,
        int highScore,
        const std::vector<SnakeSegment>& snakeBody,
        const Fruit& fruit,
        const Bomb& bomb,
        const SuperFruit& superFruit,
        int nextBombToggleScore,
        int nextSuperFruitScore,
        int score,
        float delay,
        int direction,
        int mode
    );

    // Loads the full game session from file
    static bool loadSessionFromFile(
        const std::string& filename,
        int& highScore,
        std::vector<SnakeSegment>& snakeBody,
        Fruit& fruit,
        Bomb& bomb,
        SuperFruit& superFruit,
        int& nextBombToggleScore,
        int& nextSuperFruitScore,
        int& score,
        float& delay,
        int& direction,
        int& mode
    );

    // Saves only the high score and settings to file
    static void saveHighScoreAndSettingsOnly(const std::string& filename, int highScore);
};

#endif

