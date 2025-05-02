// Utils.h
// --------------------------------------------------
// Contains utility functions for text alignment and
// button cooldowns.
// --------------------------------------------------

#ifndef UTILS_H
#define UTILS_H

#include <SFML/Graphics.hpp>
#include <sstream>

// Forward declaration of cooldown clock
extern sf::Clock buttonTimer;

// Utility: Check if a button can be clicked based on cooldown
bool canClick();

// Utility: Check if mouse is over a button
bool isMouseOver(const sf::RectangleShape& button, const sf::Vector2f& mousePos);

// Utility: Center a text object horizontally and place at a specific Y
void centerText(sf::Text& text, float windowWidth, float yPos);

#endif // UTILS_H
