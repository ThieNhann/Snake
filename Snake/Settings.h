#ifndef SETTINGS_H
#define SETTINGS_H

#include <SFML/Graphics.hpp>  // Needed for sf::VideoMode, sf::Uint32
#include <fstream> 

enum ResolutionOption {
    RES_800x600,
    RES_1600x900,
    RES_FULLSCREEN
};

class GameSettings {
public:
    ResolutionOption resolution = RES_800x600;

    void save(std::ofstream& out);
    void load(std::ifstream& in);

    void loadFromFile();  // Add this to GameSettings
    void saveToFile();

    // ✅ Declare these properly here
    sf::VideoMode getVideoMode() const;
    sf::Uint32 getWindowStyle() const;
};

// ✅ Global instance
extern GameSettings setting;

#endif // SETTINGS_H
