// Utils.h
// --------------------------------------------------
// Contains utility functions for text alignment and
// button cooldowns.
// --------------------------------------------------

#ifndef UTILS_H
#define UTILS_H

#include <SFML/Graphics.hpp>
#include <sstream>

// Global cooldown timer for button clicks
extern sf::Clock buttonTimer;

// Returns true if enough time has passed since the last button click
bool canClick();

// Returns true if the mouse is over the given button shape
bool isMouseOver(const sf::RectangleShape& button, const sf::Vector2f& mousePos);

// Centers a text object horizontally at a given y position
void centerText(sf::Text& text, float windowWidth, float yPos);

#endif
