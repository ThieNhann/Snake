#include <SFML/Graphics.hpp>
#include "Game.h"
#include "Config.h"
#include "Settings.h"  // Add this to use the global `setting`
#include <fstream>

int main() {
    // Load saved settings first (especially resolution)
    std::ifstream in("save.dat", std::ios::binary);
    if (in) {
        int dummyHighScore, dummySize;
        in.read(reinterpret_cast<char*>(&dummyHighScore), sizeof(dummyHighScore));
        in.read(reinterpret_cast<char*>(&dummySize), sizeof(dummySize));
        in.seekg(sizeof(SnakeSegment) * dummySize + sizeof(Fruit) + sizeof(int) * 2 + sizeof(int), std::ios::cur);
        setting.load(in);  // Load saved resolution
        in.close();
    }

    // Apply setting when creating window
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
