#include <SFML/Graphics.hpp>
#include "Game.h"
#include "Config.h"
#include "Settings.h"
#include "UI.h"
#include "GameStateManager.h"
#include <fstream>

int main() {
    // Load settings from file (e.g., window size, style, etc.)
    setting.loadFromFile();

    // Create the main window with settings
    sf::RenderWindow window(setting.getVideoMode(), "Snake", setting.getWindowStyle());
    window.setFramerateLimit(60); // Limit FPS to 60

    // Initialize game state manager and game logic
    GameStateManager gsm;
    Game game(window.getSize(), &gsm);

    // Main game loop
    while (window.isOpen()) {
        sf::Event e;
        // Handle window events (e.g., close event)
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed)
                window.close();
        }

        // Handle user input for the game
        game.handleInput(window);

        // Update game logic only if playing
        float dt = gsm.getState() == PLAYING ? 1.f / 60.f : 0.f;
        game.update(dt);

        // Render the game
        window.clear();
        game.render(window);
        window.display();
    }

    return 0;
}
