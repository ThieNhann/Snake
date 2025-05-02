// Utils.cpp
// --------------------------------------------------
// Implements utility functions declared in Utils.h
// --------------------------------------------------

#include "Utils.h"

bool canClick() {
    return buttonTimer.getElapsedTime().asSeconds() > 0.5f;
}

bool isMouseOver(const sf::RectangleShape& button, const sf::Vector2f& mousePos) {
    return button.getGlobalBounds().contains(mousePos);
}

void centerText(sf::Text& text, float windowWidth, float yPos) {
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(bounds.width / 2, bounds.height / 2);
    text.setPosition(windowWidth / 2, yPos);
}
