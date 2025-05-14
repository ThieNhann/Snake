#include "Settings.h"
#include <SFML/Graphics.hpp> 

GameSettings setting;

void GameSettings::save(std::ofstream& out) {
    int r = static_cast<int>(resolution);
    out.write(reinterpret_cast<char*>(&r), sizeof(r));

    // Manual clamp (equivalent to std::clamp)
    int clampedVolume = volume;
    if (clampedVolume < 0) clampedVolume = 0;
    if (clampedVolume > 100) clampedVolume = 100;

    out.write(reinterpret_cast<char*>(&clampedVolume), sizeof(clampedVolume));
}

void GameSettings::load(std::ifstream& in) {
    int r;
    in.read(reinterpret_cast<char*>(&r), sizeof(r));
    if (r >= 0 && r <= 2) resolution = static_cast<ResolutionOption>(r);

    if (in.peek() != EOF) {
        in.read(reinterpret_cast<char*>(&volume), sizeof(volume));
        // 🛡️ Clamp to range just in case
        if (volume < 0 || volume > 100) volume = 100;
    }
    else {
        volume = 100;
    }
}

void GameSettings::saveToFile() {
    std::ofstream out(SETTINGS_FILE_PATH, std::ios::binary);
    if (out) save(out);
}

void GameSettings::loadFromFile() {
    std::ifstream in(SETTINGS_FILE_PATH, std::ios::binary);
    if (in) load(in);
}

sf::VideoMode GameSettings::getVideoMode() const {
    switch (resolution) {
    case RES_800x600: return sf::VideoMode(800, 600);
    case RES_1600x900: return sf::VideoMode(1600, 900);
    case RES_FULLSCREEN: return sf::VideoMode::getDesktopMode();
    default: return sf::VideoMode(800, 600);
    }
}

sf::Uint32 GameSettings::getWindowStyle() const {
    return (resolution == RES_FULLSCREEN) ? sf::Style::Fullscreen : sf::Style::Close;
}

