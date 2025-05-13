// Game.cpp
// --------------------------------------------------
// Implements the core game logic and lifecycle
// --------------------------------------------------

#include "Game.h"
#include "Config.h"
#include "Utils.h"

#include <fstream>
#include <sstream>

sf::Clock buttonTimer;  // global button cooldown timer

Game::Game(sf::Vector2u windowSize)
    : gridSize(getPlayAreaSize(windowSize)),
    snake(gridSize.x, gridSize.y),
    objects(gridSize.x, gridSize.y) {
    srand(static_cast<unsigned>(time(nullptr)));
    loadAssets();
    loadSessionFromFile();
    ui = new UI(font, windowSize.x, windowSize.y);  // use actual window size
    state = MENU;
}

void Game::loadAssets() {
    font.loadFromFile("fonts/PressStart2P.ttf");
    snakeTex.loadFromFile("images/snake.png");
    fruitTex.loadFromFile("images/fruit.png");
    bgMenu.loadFromFile("images/bg_menu.png");
    bgPlay.loadFromFile("images/bg_play.png");
    bgOver.loadFromFile("images/bg_gameover.png");

    snakeSprite.setTexture(snakeTex);
    fruitSprite.setTexture(fruitTex);
}

void Game::reset() {
    // Recompute grid size and update systems
    sf::Vector2u winSize(setting.getVideoMode().width, setting.getVideoMode().height);
    gridSize = getPlayAreaSize(winSize);
    snake.setGridSize(gridSize.x, gridSize.y);
    objects.setGridSize(gridSize.x, gridSize.y);

    snake.reset();
    score = 0;
    delay = INITIAL_DELAY;
    timer = 0;
    objects.spawnFruit(snake);
    gameClock.restart();
    buttonTimer.restart();
    state = PLAYING;
}

void Game::update(float dt) {
    if (state != PLAYING) return;

    timer += dt;
    if (timer > delay) {
        timer = 0;
        snake.move();

        if (snake.checkSelfCollision() || snake.checkWallCollision()) {
            if (score > highScore) {
                highScore = score;
            }
            saveHighScoreAndSettingsOnly();
            state = GAME_OVER;
            return;
        }

        if (objects.checkFruitCollision(snake)) {
            snake.grow();
            score++;
            if (score % SPEED_SCORE_THRESHOLD == 0) delay *= SPEED_INCREMENT;
            objects.spawnFruit(snake);
        }
    }
}

void Game::render(sf::RenderWindow& window) {
    // --- Set background based on game state ---
    if (state == MENU) {
        bgSprite.setTexture(bgMenu);
        bgSprite.setColor(sf::Color::White);  // Reset to fully opaque
        sf::Vector2u winSize = window.getSize();
        bgSprite.setScale(
            float(winSize.x) / bgMenu.getSize().x,
            float(winSize.y) / bgMenu.getSize().y
        );
        window.draw(bgSprite);
    }
    else if (state == GAME_OVER) {
        bgSprite.setTexture(bgOver);
        sf::Vector2u winSize = window.getSize();
        bgSprite.setScale(
            float(winSize.x) / bgMenu.getSize().x,
            float(winSize.y) / bgMenu.getSize().y
        );
        window.draw(bgSprite);
    }

    else if (state == PLAYING) {
        window.clear(RetroGreen); // Retro green background

        sf::Vector2u winSize = window.getSize(); 
        int left = PLAY_AREA_MARGIN; 
        int top = TOP_BAR_HEIGHT + PLAY_AREA_MARGIN; 
        int width = winSize.x - 2 * PLAY_AREA_MARGIN; 
        int height = winSize.y - 2 * PLAY_AREA_MARGIN - TOP_BAR_HEIGHT; 

        sf::RectangleShape border;
        border.setSize({ float(width), float(height) });
        border.setPosition(left, top);
        border.setOutlineThickness(5);
        border.setOutlineColor(sf::Color::Black);
        border.setFillColor(sf::Color::Transparent);
        window.draw(border);
    }
    else if (state == PAUSED)
    {
        window.clear(RetroGreen);
    }
    else if (state == MENU_WITH_SAVED) {
        bgSprite.setTexture(bgMenu);
        bgSprite.setColor(sf::Color::White);  // Reset to fully opaque
        sf::Vector2u winSize = window.getSize();
        bgSprite.setScale(
            float(winSize.x) / bgMenu.getSize().x,
            float(winSize.y) / bgMenu.getSize().y
        );
        window.draw(bgSprite);
    }

    // --- Draw UI depending on state ---
    sf::Vector2u winSize = window.getSize();
    switch (state) {
    case MENU:
    case MENU_WITH_SAVED:
        ui->drawMenu(window, hasSavedSession);  // ✅ Pass in actual status
        break;
    case PAUSED:
        ui->drawPause(window);
        break;
    case GAME_OVER:
        ui->drawGameOver(window, score, highScore);
        break;
    case PLAYING:
    {
        // Top HUD (score, pause button, etc.)
        ui->drawHUD(window, score, highScore);

        sf::Vector2i offset = getPlayAreaOffset(); // in grid units
        sf::Vector2f pixelOffset(offset.x * TILE_SIZE, offset.y * TILE_SIZE);

        // Draw snake
        for (const auto& segment : snake.getBody()) {
            snakeSprite.setPosition(segment.x * TILE_SIZE + pixelOffset.x,
                segment.y * TILE_SIZE + pixelOffset.y);
            window.draw(snakeSprite);
        }

        // Draw fruit
        const Fruit& fruit = objects.getFruit();
        fruitSprite.setPosition(fruit.x * TILE_SIZE + pixelOffset.x,
            fruit.y * TILE_SIZE + pixelOffset.y);
        window.draw(fruitSprite);

        break;
    }
    case CONFIRMATION:
    {
        ui->drawConfirmation(window);
        break;
    }
    case SETTINGS_MENU: 
    {
        bgSprite.setTexture(bgMenu);
        bgSprite.setColor(sf::Color(255, 255, 255, 150)); // Add transparency
        sf::Vector2u winSize = window.getSize();
        bgSprite.setScale(
            float(winSize.x) / bgMenu.getSize().x,
            float(winSize.y) / bgMenu.getSize().y
        );
        window.draw(bgSprite);

        ui->drawSettingsRoot(window);
        break;
    }
    case SETTINGS_RESOLUTION:
    {
        bgSprite.setTexture(bgMenu);
        bgSprite.setColor(sf::Color(255, 255, 255, 150)); // Transparent overlay
        winSize = window.getSize();
        bgSprite.setScale(
            float(winSize.x) / bgMenu.getSize().x,
            float(winSize.y) / bgMenu.getSize().y
        );
        window.draw(bgSprite);
        ui->drawResolutionMenu(window);  // ✅ Correct method for submenu
        break;
    }
    case SETTINGS_SOUND:
    {
        bgSprite.setTexture(bgMenu);
        bgSprite.setColor(sf::Color(255, 255, 255, 150)); // Transparent overlay
        winSize = window.getSize();
        bgSprite.setScale(
            float(winSize.x) / bgMenu.getSize().x,
            float(winSize.y) / bgMenu.getSize().y
        );
        window.draw(bgSprite);
        ui->drawSoundSettings(window);  // ✅ Dummy screen with just "Back"
        break;
    }
    }
}

void Game::handleInput(sf::RenderWindow& window) {
    if (state == PLAYING) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) snake.setDirection(1);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) snake.setDirection(2);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) snake.setDirection(3);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) snake.setDirection(0);
    }

    sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(window);
    ui->updateHoverEffect(mousePos);

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && canClick()) {
        if (state == MENU || state == MENU_WITH_SAVED) {
            if (isMouseOver(ui->startButton.shape, mousePos)) {
                if (hasSavedSession) {
                    confirmationType = CONFIRM_NEW_GAME;
                    ui->confirmText.setString("Start a new game?\nYour current session will be lost.");
                    state = CONFIRMATION;
                }
                else {
                    // 💡 Ensure grid is recomputed
                    gridSize = getPlayAreaSize(window.getSize());
                    snake.setGridSize(gridSize.x, gridSize.y);
                    objects.setGridSize(gridSize.x, gridSize.y);
                    reset();
                }
            }
            else if (isMouseOver(ui->continueButton.shape, mousePos) && hasSavedSession) {
                loadSession();
            }
            else if (isMouseOver(ui->exitButton.shape, mousePos)) {
                confirmationType = CONFIRM_EXIT;
                ui->confirmText.setString("Are you sure you want to quit?");
                state = CONFIRMATION;
            }
            else if (isMouseOver(ui->settingsButton.shape, mousePos)) {
                state = SETTINGS_MENU;
            }
        }

        else if (state == GAME_OVER) {
            if (isMouseOver(ui->restartButton.shape, mousePos)) {
                reset();
            }
            else if (isMouseOver(ui->exitButton.shape, mousePos)) {
                confirmationType = CONFIRM_EXIT;
                saveHighScoreAndSettingsOnly();
                ui->confirmText.setString("Are you sure you want to quit?");
                state = CONFIRMATION;
            }
        }

        else if (state == PLAYING && ui->pauseCircle.getGlobalBounds().contains(mousePos)) {
            state = PAUSED;
        }

        else if (state == PAUSED) {
            if (isMouseOver(ui->resumeButton.shape, mousePos)) {
                state = PLAYING;
            }
            else if (isMouseOver(ui->menuButton.shape, mousePos)) {
                saveSession();  // Save before going to menu
                state = MENU_WITH_SAVED;
            }
        }

        else if (state == CONFIRMATION) {
            if (isMouseOver(ui->yesButton.shape, mousePos)) {
                if (confirmationType == CONFIRM_NEW_GAME) {
                    hasSavedSession = false;
                    reset();
                }
                else if (confirmationType == CONFIRM_EXIT) {
                    window.close();
                }
            }
            else if (isMouseOver(ui->noButton.shape, mousePos)) {
                state = hasSavedSession ? MENU_WITH_SAVED : MENU;
            }
        }

        else if (state == SETTINGS_MENU) {
            if (isMouseOver(ui->resolutionButton.shape, mousePos)) {
                state = SETTINGS_RESOLUTION;
            }
            else if (isMouseOver(ui->soundButton.shape, mousePos)) {
                state = SETTINGS_SOUND;
            }
            else if (isMouseOver(ui->backButton.shape, mousePos)) {
                state = MENU;
            }
        }
        else if (state == SETTINGS_RESOLUTION) {
            if (isMouseOver(ui->res800Button.shape, mousePos)) {
                setting.resolution = RES_800x600;
                saveSession();
                applyResolutionChange(window);
                ui->resize(window.getSize().x, window.getSize().y);
            }
            else if (isMouseOver(ui->res1600Button.shape, mousePos)) {
                setting.resolution = RES_1600x900;
                saveSession();
                applyResolutionChange(window);
                ui->resize(window.getSize().x, window.getSize().y);
            }
            else if (isMouseOver(ui->fullscreenButton.shape, mousePos)) {
                setting.resolution = RES_FULLSCREEN;
                saveSession();
                applyResolutionChange(window);
                ui->resize(window.getSize().x, window.getSize().y);
            }
            else if (isMouseOver(ui->backButton.shape, mousePos)) {
                state = SETTINGS_MENU;
            }
        }
        else if (state == SETTINGS_SOUND) {
            if (isMouseOver(ui->backButton.shape, mousePos)) {
                state = SETTINGS_MENU;
            }
        }

        buttonTimer.restart();
    }
}

GameState Game::getState() const {
    return state;
}

void Game::setState(GameState newState) {
    state = newState;
}

void Game::saveSession() {
    savedSnake = snake.getBody();
    savedDirection = snake.getDirection();
    savedFruit = objects.getFruit();
    savedScore = score;
    savedDelay = delay;
    hasSavedSession = true;
    saveSessionToFile();  
}

void Game::loadSession() {
    // Recompute grid size from current window resolution
    sf::Vector2u winSize = sf::Vector2u(setting.getVideoMode().width, setting.getVideoMode().height);
    gridSize = getPlayAreaSize(winSize);

    // Resize snake and objects grid context
    snake.setGridSize(gridSize.x, gridSize.y);
    objects.setGridSize(gridSize.x, gridSize.y);

    snake.load(savedSnake, savedDirection);
    objects.load(savedFruit);
    score = savedScore;
    delay = savedDelay;

    // Check if fruit is still in bounds
    sf::Vector2i offset = getPlayAreaOffset();
    if (savedFruit.x < offset.x || savedFruit.x >= offset.x + gridSize.x ||
        savedFruit.y < offset.y || savedFruit.y >= offset.y + gridSize.y)
    {
        objects.spawnFruit(snake);
    }

    gameClock.restart();
    state = PLAYING;
}

void Game::saveSessionToFile() {
    std::ofstream out("save.dat", std::ios::binary);
    if (!out) return;

    // Write high score first
    out.write(reinterpret_cast<char*>(&highScore), sizeof(highScore));

    int snakeSize = savedSnake.size();
    out.write(reinterpret_cast<char*>(&snakeSize), sizeof(snakeSize));
    for (const auto& segment : savedSnake) {
        out.write(reinterpret_cast<const char*>(&segment), sizeof(SnakeSegment));
    }

    out.write(reinterpret_cast<char*>(&savedFruit), sizeof(Fruit));
    out.write(reinterpret_cast<char*>(&savedScore), sizeof(savedScore));
    out.write(reinterpret_cast<char*>(&savedDelay), sizeof(savedDelay));
    out.write(reinterpret_cast<char*>(&savedDirection), sizeof(savedDirection));

    setting.save(out);

    out.close();
}

void Game::loadSessionFromFile() {
    std::ifstream in("save.dat", std::ios::binary);
    if (!in) {
        hasSavedSession = false;
        return;
    }

    // Read high score first
    in.read(reinterpret_cast<char*>(&highScore), sizeof(highScore));

    int snakeSize = 0;
    in.read(reinterpret_cast<char*>(&snakeSize), sizeof(snakeSize));
    if (snakeSize <= 0) {
        hasSavedSession = false;
        setting.load(in);  // Still load resolution
        return;
    }

    // Valid session, load rest
    savedSnake.resize(snakeSize);
    for (int i = 0; i < snakeSize; ++i) {
        in.read(reinterpret_cast<char*>(&savedSnake[i]), sizeof(SnakeSegment));
    }

    in.read(reinterpret_cast<char*>(&savedFruit), sizeof(Fruit));
    in.read(reinterpret_cast<char*>(&savedScore), sizeof(savedScore));
    in.read(reinterpret_cast<char*>(&savedDelay), sizeof(savedDelay));
    in.read(reinterpret_cast<char*>(&savedDirection), sizeof(savedDirection));

    setting.load(in);

    hasSavedSession = true;
    in.close();
}

void Game::applyResolutionChange(sf::RenderWindow& window) {
    sf::VideoMode mode = setting.getVideoMode();
    sf::Uint32 style = setting.getWindowStyle();

    saveSession();  // Save before changing resolution

    window.create(mode, "Snake Game", style);
    window.setFramerateLimit(60);

    // Recreate UI with updated layout
    delete ui;
    ui = new UI(font, mode.width, mode.height);
    ui->resize(mode.width, mode.height);

    // Update grid size and inform systems that depend on it
    gridSize = getPlayAreaSize(window.getSize());
    snake.setGridSize(gridSize.x, gridSize.y);
    objects.setGridSize(gridSize.x, gridSize.y);
}

void Game::saveHighScoreAndSettingsOnly() {
    std::ofstream out("save.dat", std::ios::binary | std::ios::trunc);  // 🔧 truncates file

    if (!out) return;

    out.write(reinterpret_cast<char*>(&highScore), sizeof(highScore));

    int zero = 0;
    out.write(reinterpret_cast<char*>(&zero), sizeof(zero));  // Snake size = 0

    // Dummy placeholders for required structure
    Fruit dummyFruit = { -1, -1 };
    float dummyDelay = 0;
    int dummyScore = 0;
    int dummyDir = 0;

    out.write(reinterpret_cast<char*>(&dummyFruit), sizeof(Fruit));
    out.write(reinterpret_cast<char*>(&dummyScore), sizeof(int));
    out.write(reinterpret_cast<char*>(&dummyDelay), sizeof(float));
    out.write(reinterpret_cast<char*>(&dummyDir), sizeof(int));

    setting.save(out);
    out.close();

    hasSavedSession = false;
}

