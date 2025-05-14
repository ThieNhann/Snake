#ifndef SETTINGS_H
#define SETTINGS_H

#include <SFML/Graphics.hpp>  // Needed for sf::VideoMode, sf::Uint32
#include <fstream> 

#include "Config.h"

enum ResolutionOption {
    RES_800x600,
    RES_1600x900,
    RES_FULLSCREEN
};

class GameSettings {
public:
    // default size
    ResolutionOption resolution = RES_800x600;
    // default volume
    int volume = 100;  

    void save(std::ofstream& out);
    void load(std::ifstream& in);
    void saveToFile();
    void loadFromFile();

    // ✅ Declare these properly here
    sf::VideoMode getVideoMode() const;
    sf::Uint32 getWindowStyle() const;
};

// ✅ Global instance
extern GameSettings setting;

#endif // SETTINGS_H
