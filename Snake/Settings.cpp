#include "Settings.h"
#include <SFML/Graphics.hpp> 

GameSettings setting;

void GameSettings::save(std::ofstream& out) {
    int r = static_cast<int>(resolution);
    out.write(reinterpret_cast<char*>(&r), sizeof(r));
}

void GameSettings::load(std::ifstream& in) {
    int r;
    in.read(reinterpret_cast<char*>(&r), sizeof(r));
    if (r >= 0 && r <= 2) resolution = static_cast<ResolutionOption>(r);
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

void GameSettings::saveToFile() {
    std::ofstream out("settings.dat", std::ios::binary);
    if (out) save(out);
}
void GameSettings::loadFromFile() {
    std::ifstream in("settings.dat", std::ios::binary);
    if (in) load(in);
}
