#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include "Snake.h"
#include "ObjectManager.h"
#include "UI.h"
#include "Settings.h"

enum GameState { MENU, MENU_WITH_SAVED, PLAYING, PAUSED, GAME_OVER, CONFIRMATION, 
                 SETTINGS_MENU, SETTINGS_RESOLUTION, SETTINGS_SOUND, MODE_SELECTION };
enum ConfirmationType { CONFIRM_EXIT, CONFIRM_NEW_GAME, CONFIRM_RESIZE };

enum GameMode { NORMAL_MODE, FIRE_BORDER_MODE };

class Game {
public:
    Game(sf::Vector2u windowSize);
    void reset();
    void update(float dt);
    void render(sf::RenderWindow& window);
    GameState getState() const;
    void setState(GameState newState);

    // ----------------------
    // --- Input handling ---
    // ----------------------
 
    void handleInput(sf::RenderWindow& window);

    // --------------------------
    // --- Save file handling ---
    // --------------------------
    void saveSessionToFile();
    void loadSessionFromFile();

    void applyResolutionChange(sf::RenderWindow& window);

    void saveHighScoreAndSettingsOnly();

private:
    Snake snake;
    bool isDying = false;
    float dyingTimer = 0.f;

    ObjectManager objects;
    sf::Vector2i gridSize;
    UI* ui = nullptr;

    sf::Font font;
    sf::Texture snakeTex, fruitTex, bombTex, superFruitTex, bgMenu, bgOver;
    sf::Sprite snakeSprite, fruitSprite, bombSprite, superFruitSprite, bgSprite;

    GameState state ;
    sf::Clock gameClock;
    float delay;
    float timer = 0;

    ResolutionOption pendingResolution = RES_800x600;

    int score = 0;
    int highScore = 0;

    // Sound buffers
    sf::SoundBuffer bufferButton;
    sf::SoundBuffer bufferGameOver;
    sf::SoundBuffer eatBuffer;
    sf::SoundBuffer bufferExplode;
    sf::SoundBuffer bufferSuperEat;
    
    // Sounds
    sf::Sound soundButton;
    sf::Sound soundGameOver;
    sf::Sound eatSound;
    sf::Sound soundExplode;
    sf::Sound soundSuperEat;

    bool hasSavedSession = false;
    bool skipNextBombUpdate = false;

    ConfirmationType confirmationType;  
    sf::RectangleShape confirmBox;
    sf::Text confirmText, yesText, noText;
    sf::RectangleShape yesButton, noButton;

    // Save state
    std::vector<SnakeSegment> savedSnake;
    Fruit savedFruit;
    Bomb savedBomb;
    int savedNextBombToggleScore;
    SuperFruit savedSuperFruit;
    int savedNextSuperFruitScore;

    int savedScore = 0;
    float savedDelay = 0;
    int savedDirection = 0;

    void loadAssets();

    void saveSession();
    void loadSession();

    GameMode mode = NORMAL_MODE;
};

#endif 
