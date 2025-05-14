#include "SaveManager.h"

void ensureSaveDirectoryExists() {
    if (!fs::exists("Save files")) {
        fs::create_directory("Save files");
    }
}

bool SaveManager::validate(const std::string& filename) {
    std::ifstream in(filename, std::ios::binary);
    if (!in) return false;

    int highScore, snakeSize;
    if (!in.read(reinterpret_cast<char*>(&highScore), sizeof(highScore))) return false;
    if (!in.read(reinterpret_cast<char*>(&snakeSize), sizeof(snakeSize))) return false;
    if (snakeSize <= 0 || snakeSize > 1000) return false;

    in.seekg(sizeof(SnakeSegment) * snakeSize, std::ios::cur);
    if (!in) return false;

    Fruit dummyFruit;
    Bomb dummyBomb;
    int bombToggleScore;
    SuperFruit dummySuperFruit; 
    int dummySuperScore; 
    int dummyScore;
    float dummyDelay;
    int dummyDir;
    int dummyMode;

    return in.read(reinterpret_cast<char*>(&dummyFruit), sizeof(Fruit)) &&
        in.read(reinterpret_cast<char*>(&dummyBomb), sizeof(Bomb)) &&
        in.read(reinterpret_cast<char*>(&bombToggleScore), sizeof(bombToggleScore)) &&
        in.read(reinterpret_cast<char*>(&dummySuperFruit), sizeof(SuperFruit)) &&
        in.read(reinterpret_cast<char*>(&dummySuperScore), sizeof(dummySuperScore)) &&
        in.read(reinterpret_cast<char*>(&dummyScore), sizeof(dummyScore)) &&
        in.read(reinterpret_cast<char*>(&dummyDelay), sizeof(dummyDelay)) &&
        in.read(reinterpret_cast<char*>(&dummyDir), sizeof(dummyDir)) &&
        in.read(reinterpret_cast<char*>(&dummyMode), sizeof(dummyMode));
}

void SaveManager::saveSessionToFile(
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
) {
    ensureSaveDirectoryExists();

    std::ofstream out(filename, std::ios::binary);
    if (!out) return;

    // --- Save highscore ---
    out.write(reinterpret_cast<const char*>(&highScore), sizeof(highScore));

    // --- Save snake ---
    int size = snakeBody.size();
    out.write(reinterpret_cast<const char*>(&size), sizeof(size));
    for (const auto& segment : snakeBody) {
        out.write(reinterpret_cast<const char*>(&segment), sizeof(SnakeSegment));
    }

    // --- Save objects ---
    out.write(reinterpret_cast<const char*>(&fruit), sizeof(Fruit));
    out.write(reinterpret_cast<const char*>(&bomb), sizeof(Bomb));
    out.write(reinterpret_cast<const char*>(&nextBombToggleScore), sizeof(nextBombToggleScore));

    out.write(reinterpret_cast<const char*>(&superFruit), sizeof(SuperFruit));
    out.write(reinterpret_cast<const char*>(&nextSuperFruitScore), sizeof(nextSuperFruitScore));

    // --- Save game stats ---
    out.write(reinterpret_cast<const char*>(&score), sizeof(score));
    out.write(reinterpret_cast<const char*>(&delay), sizeof(delay));
    out.write(reinterpret_cast<const char*>(&direction), sizeof(direction));
    out.write(reinterpret_cast<const char*>(&mode), sizeof(mode));

    out.close();
}

bool SaveManager::loadSessionFromFile(
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
) {
    std::ifstream in(filename, std::ios::binary);
    if (!in) return false;

    // --- Load highscore ---
    if (!in.read(reinterpret_cast<char*>(&highScore), sizeof(highScore))) return false;

    // --- Load snake ---
    int snakeSize = 0;
    if (!in.read(reinterpret_cast<char*>(&snakeSize), sizeof(snakeSize))) return false;
    if (snakeSize <= 0 || snakeSize > 1000) return false;

    snakeBody.resize(snakeSize);
    for (int i = 0; i < snakeSize; ++i) {
        if (!in.read(reinterpret_cast<char*>(&snakeBody[i]), sizeof(SnakeSegment)))
            return false;
    }

    // --- Load objects ---
    if (!in.read(reinterpret_cast<char*>(&fruit), sizeof(Fruit))) return false;
    if (!in.read(reinterpret_cast<char*>(&bomb), sizeof(Bomb))) return false;
    if (!in.read(reinterpret_cast<char*>(&nextBombToggleScore), sizeof(nextBombToggleScore))) return false;  
    if (!in.read(reinterpret_cast<char*>(&superFruit), sizeof(SuperFruit))) return false;  
    if (!in.read(reinterpret_cast<char*>(&nextSuperFruitScore), sizeof(int))) return false;

    // --- Load game stats ---
    if (!in.read(reinterpret_cast<char*>(&score), sizeof(score))) return false;
    if (!in.read(reinterpret_cast<char*>(&delay), sizeof(delay))) return false;
    if (!in.read(reinterpret_cast<char*>(&direction), sizeof(direction))) return false;
    if (!in.read(reinterpret_cast<char*>(&mode), sizeof(mode))) return false;

    in.close();
    return true;
}

void SaveManager::saveHighScoreAndSettingsOnly(const std::string& filename, int highScore) {
    ensureSaveDirectoryExists();

    std::ofstream out(filename, std::ios::binary | std::ios::trunc);
    if (!out) return;

    out.write(reinterpret_cast<const char*>(&highScore), sizeof(highScore));

    int zero = 0;
    out.write(reinterpret_cast<const char*>(&zero), sizeof(zero));  // Snake size = 0

    Fruit dummyFruit = { -1, -1 };
    int dummyScore = 0;
    float dummyDelay = INITIAL_DELAY;
    int dummyDir = 0;

    out.write(reinterpret_cast<const char*>(&dummyFruit), sizeof(Fruit));
    out.write(reinterpret_cast<const char*>(&dummyScore), sizeof(dummyScore));
    out.write(reinterpret_cast<const char*>(&dummyDelay), sizeof(dummyDelay));
    out.write(reinterpret_cast<const char*>(&dummyDir), sizeof(dummyDir));

    out.close();

    // Save settings separately
    setting.saveToFile();
}
