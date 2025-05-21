#include <SFML/Graphics.hpp>
#include "Game.h"
#include "Config.h"
#include "Settings.h"
#include "UI.h"
#include "GameStateManager.h"
#include <fstream>

int main() {

    setting.loadFromFile();

    sf::RenderWindow window(setting.getVideoMode(), "Snake Game", setting.getWindowStyle());
    window.setFramerateLimit(60);

    GameStateManager gsm;
    Game game(window.getSize(), &gsm);

    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed)
                window.close();
        }

        game.handleInput(window);
        float dt = gsm.getState() == PLAYING ? 1.f / 60.f : 0.f;
        game.update(dt);

        window.clear();
        game.render(window);
        window.display();
    }

    return 0;
}
