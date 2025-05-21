#include <SFML/Graphics.hpp>
#include "Game.h"
#include "Config.h"
#include "Settings.h"
#include "UI.h"
#include <fstream>

int main() {

    setting.loadFromFile();

    sf::RenderWindow window(setting.getVideoMode(), "Snake Game", setting.getWindowStyle());
    window.setFramerateLimit(60);

    Game game(window.getSize());

    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed)
                window.close();
        }

        game.handleInput(window);
        float dt = game.getState() == PLAYING ? 1.f / 60.f : 0.f;
        game.update(dt);

        window.clear();
        game.render(window);
        window.display();
    }

    return 0;
}
