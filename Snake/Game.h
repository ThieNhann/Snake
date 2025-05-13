#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "Snake.h"
#include "ObjectManager.h"
#include "UI.h"
#include "Settings.h"

enum GameState { MENU, MENU_WITH_SAVED, PLAYING, PAUSED, GAME_OVER, CONFIRMATION, 
                 SETTINGS_MENU, SETTINGS_RESOLUTION, SETTINGS_SOUND, MODE_SELECTION};
enum ConfirmationType { CONFIRM_EXIT, CONFIRM_NEW_GAME };

class Game {
public:
    Game(sf::Vector2u windowSize);
    void reset();
    void update(float dt);
    void render(sf::RenderWindow& window);
    void handleInput(sf::RenderWindow& window);
    GameState getState() const;
    void setState(GameState newState);
    bool wallCollisionMode();
    void saveSessionToFile();
    void loadSessionFromFile();

    void applyResolutionChange(sf::RenderWindow& window);

    void saveHighScoreAndSettingsOnly();

private:
    Snake snake;
    ObjectManager objects;
    sf::Vector2i gridSize;
    UI* ui = nullptr;

    sf::Font font;
    sf::Texture snakeTex, fruitTex, bgMenu, bgPlay, bgOver;
    sf::Sprite snakeSprite, fruitSprite, bgSprite;

    GameState state ;
    sf::Clock gameClock;
    float delay;
    float timer = 0;

    int score = 0;
    int highScore = 0;

    bool hasSavedSession = false;

    ConfirmationType confirmationType;  // What we're confirming
    sf::RectangleShape confirmBox;
    sf::Text confirmText, yesText, noText;
    sf::RectangleShape yesButton, noButton;

    // Save state
    std::vector<SnakeSegment> savedSnake;
    Fruit savedFruit;
    int savedScore = 0;
    float savedDelay = 0;
    int savedDirection = 0;

    //Gamemode
    bool withWallCollision = false;

    void loadAssets();

    void saveSession();
    void loadSession();
};

#endif // GAME_H
