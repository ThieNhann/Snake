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

    // Read raw data for Fruit, Bomb, SuperFruit
    int fruit_x, fruit_y;
    int bomb_x, bomb_y, bomb_active;
    int bombToggleScore;
    int super_x, super_y, super_active;
    float super_timer;
    int superScore;
    int score;
    float delay;
    int dir;
    int mode;

    return in.read(reinterpret_cast<char*>(&fruit_x), sizeof(fruit_x)) &&
        in.read(reinterpret_cast<char*>(&fruit_y), sizeof(fruit_y)) &&
        in.read(reinterpret_cast<char*>(&bomb_x), sizeof(bomb_x)) &&
        in.read(reinterpret_cast<char*>(&bomb_y), sizeof(bomb_y)) &&
        in.read(reinterpret_cast<char*>(&bomb_active), sizeof(bomb_active)) &&
        in.read(reinterpret_cast<char*>(&bombToggleScore), sizeof(bombToggleScore)) &&
        in.read(reinterpret_cast<char*>(&super_x), sizeof(super_x)) &&
        in.read(reinterpret_cast<char*>(&super_y), sizeof(super_y)) &&
        in.read(reinterpret_cast<char*>(&super_active), sizeof(super_active)) &&
        in.read(reinterpret_cast<char*>(&super_timer), sizeof(super_timer)) &&
        in.read(reinterpret_cast<char*>(&superScore), sizeof(superScore)) &&
        in.read(reinterpret_cast<char*>(&score), sizeof(score)) &&
        in.read(reinterpret_cast<char*>(&delay), sizeof(delay)) &&
        in.read(reinterpret_cast<char*>(&dir), sizeof(dir)) &&
        in.read(reinterpret_cast<char*>(&mode), sizeof(mode));
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

    out.write(reinterpret_cast<const char*>(&highScore), sizeof(highScore));

    int size = snakeBody.size();
    out.write(reinterpret_cast<const char*>(&size), sizeof(size));
    for (const auto& segment : snakeBody) {
        out.write(reinterpret_cast<const char*>(&segment), sizeof(SnakeSegment));
    }

    // Write Fruit
    int fruit_x = fruit.getX(), fruit_y = fruit.getY();
    out.write(reinterpret_cast<const char*>(&fruit_x), sizeof(fruit_x));
    out.write(reinterpret_cast<const char*>(&fruit_y), sizeof(fruit_y));

    // Write Bomb
    int bomb_x = bomb.getX(), bomb_y = bomb.getY();
    int bomb_active = bomb.isActive() ? 1 : 0;
    out.write(reinterpret_cast<const char*>(&bomb_x), sizeof(bomb_x));
    out.write(reinterpret_cast<const char*>(&bomb_y), sizeof(bomb_y));
    out.write(reinterpret_cast<const char*>(&bomb_active), sizeof(bomb_active));
    out.write(reinterpret_cast<const char*>(&nextBombToggleScore), sizeof(nextBombToggleScore));

    // Write SuperFruit
    int super_x = superFruit.getX(), super_y = superFruit.getY();
    int super_active = superFruit.isActive() ? 1 : 0;
    float super_timer = superFruit.getTimer();
    out.write(reinterpret_cast<const char*>(&super_x), sizeof(super_x));
    out.write(reinterpret_cast<const char*>(&super_y), sizeof(super_y));
    out.write(reinterpret_cast<const char*>(&super_active), sizeof(super_active));
    out.write(reinterpret_cast<const char*>(&super_timer), sizeof(super_timer));
    out.write(reinterpret_cast<const char*>(&nextSuperFruitScore), sizeof(nextSuperFruitScore));

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

    if (!in.read(reinterpret_cast<char*>(&highScore), sizeof(highScore))) return false;

    int snakeSize = 0;
    if (!in.read(reinterpret_cast<char*>(&snakeSize), sizeof(snakeSize))) return false;
    if (snakeSize <= 0 || snakeSize > 1000) return false;

    snakeBody.resize(snakeSize);
    for (int i = 0; i < snakeSize; ++i) {
        if (!in.read(reinterpret_cast<char*>(&snakeBody[i]), sizeof(SnakeSegment)))
            return false;
    }

    // Read Fruit
    int fruit_x, fruit_y;
    if (!in.read(reinterpret_cast<char*>(&fruit_x), sizeof(fruit_x))) return false;
    if (!in.read(reinterpret_cast<char*>(&fruit_y), sizeof(fruit_y))) return false;
    fruit.setPosition(fruit_x, fruit_y);

    // Read Bomb
    int bomb_x, bomb_y, bomb_active;
    if (!in.read(reinterpret_cast<char*>(&bomb_x), sizeof(bomb_x))) return false;
    if (!in.read(reinterpret_cast<char*>(&bomb_y), sizeof(bomb_y))) return false;
    if (!in.read(reinterpret_cast<char*>(&bomb_active), sizeof(bomb_active))) return false;
    bomb.setPosition(bomb_x, bomb_y);
    bomb.setActive(bomb_active != 0);
    if (!in.read(reinterpret_cast<char*>(&nextBombToggleScore), sizeof(nextBombToggleScore))) return false;

    // Read SuperFruit
    int super_x, super_y, super_active;
    float super_timer;
    if (!in.read(reinterpret_cast<char*>(&super_x), sizeof(super_x))) return false;
    if (!in.read(reinterpret_cast<char*>(&super_y), sizeof(super_y))) return false;
    if (!in.read(reinterpret_cast<char*>(&super_active), sizeof(super_active))) return false;
    if (!in.read(reinterpret_cast<char*>(&super_timer), sizeof(super_timer))) return false;
    superFruit.setPosition(super_x, super_y);
    superFruit.setActive(super_active != 0);
    superFruit.setTimer(super_timer);
    if (!in.read(reinterpret_cast<char*>(&nextSuperFruitScore), sizeof(nextSuperFruitScore))) return false;

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
    out.write(reinterpret_cast<const char*>(&zero), sizeof(zero));  

    // Write dummy Fruit
    int fruit_x = -1, fruit_y = -1;
    out.write(reinterpret_cast<const char*>(&fruit_x), sizeof(fruit_x));
    out.write(reinterpret_cast<const char*>(&fruit_y), sizeof(fruit_y));

    // Write dummy Bomb
    int bomb_x = -1, bomb_y = -1, bomb_active = 0;
    out.write(reinterpret_cast<const char*>(&bomb_x), sizeof(bomb_x));
    out.write(reinterpret_cast<const char*>(&bomb_y), sizeof(bomb_y));
    out.write(reinterpret_cast<const char*>(&bomb_active), sizeof(bomb_active));
    int dummyBombToggle = 0;
    out.write(reinterpret_cast<const char*>(&dummyBombToggle), sizeof(dummyBombToggle));

    // Write dummy SuperFruit
    int super_x = -1, super_y = -1, super_active = 0;
    float super_timer = 0.f;
    out.write(reinterpret_cast<const char*>(&super_x), sizeof(super_x));
    out.write(reinterpret_cast<const char*>(&super_y), sizeof(super_y));
    out.write(reinterpret_cast<const char*>(&super_active), sizeof(super_active));
    out.write(reinterpret_cast<const char*>(&super_timer), sizeof(super_timer));
    int dummySuperScore = 0;
    out.write(reinterpret_cast<const char*>(&dummySuperScore), sizeof(dummySuperScore));

    int dummyScore = 0;
    float dummyDelay = INITIAL_DELAY;
    int dummyDir = 0;
    int dummyMode = 0;

    out.write(reinterpret_cast<const char*>(&dummyScore), sizeof(dummyScore));
    out.write(reinterpret_cast<const char*>(&dummyDelay), sizeof(dummyDelay));
    out.write(reinterpret_cast<const char*>(&dummyDir), sizeof(dummyDir));
    out.write(reinterpret_cast<const char*>(&dummyMode), sizeof(dummyMode));

    out.close();

    // Save settings separately
    setting.saveToFile();
}
