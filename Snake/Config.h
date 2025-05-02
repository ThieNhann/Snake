#ifndef CONFIG_H
#define CONFIG_H

#include <SFML/Graphics.hpp>

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
const float SPEED_INCREMENT = 0.9f;
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

#endif // CONFIG_H
