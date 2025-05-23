#ifndef CONFIG_H
#define CONFIG_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

// --------------------------------------------------
// Constants for tile/grid and timing logic
// --------------------------------------------------

// Grid unit size (tile) — still static
const int TILE_SIZE = 16;

// Top bar height (fixed pixels)
const int TOP_BAR_HEIGHT = 30;

// Margin around the play area (in pixels)
const int PLAY_AREA_MARGIN = 50;

// Game speed and logic
const float INITIAL_DELAY = 0.2f;
const float SPEED_INCREMENT = 0.925f;
const int SPEED_SCORE_THRESHOLD = 3;

// Button input cooldown time
const float BUTTON_COOLDOWN = 0.5f;

// Custom retro color for background
const sf::Color RetroGreen = sf::Color(102, 187, 0);

// --------------------------------------------------
// Helpers for dynamic layout
// --------------------------------------------------

// Calculate how many tiles fit in the play area given window size
inline sf::Vector2i getPlayAreaSize(sf::Vector2u windowSize) {
    int gridWidth = (windowSize.x - 2 * PLAY_AREA_MARGIN) / TILE_SIZE;
    int gridHeight = (windowSize.y - 2 * PLAY_AREA_MARGIN - TOP_BAR_HEIGHT) / TILE_SIZE;
    return sf::Vector2i(gridWidth, gridHeight);
}

// Calculate tile offset in grid units to center the play area
inline sf::Vector2i getPlayAreaOffset() {
    return sf::Vector2i(PLAY_AREA_MARGIN / TILE_SIZE, (PLAY_AREA_MARGIN + TOP_BAR_HEIGHT) / TILE_SIZE);
}

// -----------------------------
// Resource file paths
// -----------------------------
const std::string FONT_PATH = "Resources/fonts/PressStart2P.ttf";

const std::string TEXTURE_SNAKE = "Resources/images/snake.png";
const std::string TEXTURE_FRUIT = "Resources/images/fruit.png";
const std::string TEXTURE_BUTTON = "Resources/images/button.png";
const std::string TEXTURE_BOMB = "Resources/images/bomb.png";
const std::string TEXTURE_SUPER_FRUIT = "Resources/images/super_fruit.png";

const std::string TEXTURE_BG_MENU = "Resources/images/bg_menu.png";
const std::string TEXTURE_BG_GAMEOVER = "Resources/images/bg_gameover.png";

const std::string SOUND_EAT = "Resources/audio/eat.wav";
const std::string SOUND_BUTTON = "Resources/audio/button.wav";
const std::string SOUND_GAMEOVER = "Resources/audio/game_over.wav";
const std::string SOUND_EXPLODE = "Resources/audio/explode.wav";
const std::string SOUND_SUPER_EAT = "Resources/audio/bonus.wav";

// -----------------------------
// Save file paths
// -----------------------------
const std::string SAVE_FILE_PATH = "Save files/save.dat";
const std::string SETTINGS_FILE_PATH = "Save files/saved_setting.dat";


#endif 
