#ifndef SETTINGS_H
#define SETTINGS_H

#include <SFML/Graphics.hpp>
#include <fstream>
#include "Config.h"

// Supported window resolutions
enum ResolutionOption {
    RES_800x600,
    RES_1600x900,
    RES_FULLSCREEN
};

// Stores and manages game settings (resolution, volume, speed)
class GameSettings {
public:
    ResolutionOption resolution = RES_800x600;
    int volume = 100;      // Volume: 0-100
    int snakeSpeed = 2;    // 1: Slow, 2: Normal, 3: Fast

    void save(std::ofstream& out);
    void load(std::ifstream& in);
    void saveToFile();
    void loadFromFile();

    sf::VideoMode getVideoMode() const;
    sf::Uint32 getWindowStyle() const;
};

// Global settings instance
extern GameSettings setting;

#endif
