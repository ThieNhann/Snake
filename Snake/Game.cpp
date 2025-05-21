// Game.cpp
// --------------------------------------------------
// Implements the core game logic and lifecycle
// --------------------------------------------------

#include "Game.h"
#include "Config.h"
#include "Utils.h"
#include "SaveManager.h"

#include <iostream>
#include <fstream>
#include <sstream>

sf::Clock buttonTimer;  

Game::Game(sf::Vector2u windowSize)
    : gridSize(getPlayAreaSize(windowSize)),
    snake(gridSize.x, gridSize.y),
    objects(gridSize.x, gridSize.y) {
    srand(static_cast<unsigned>(time(nullptr)));
    loadAssets();

    std::ifstream in(SAVE_FILE_PATH, std::ios::binary);
    if (in) {
        in.read(reinterpret_cast<char*>(&highScore), sizeof(highScore));
    }
    loadSessionFromFile();

    ui = new UI(font, windowSize.x, windowSize.y);  

    soundButton.setVolume(setting.volume);
    soundGameOver.setVolume(setting.volume);
    eatSound.setVolume(setting.volume);
    ui->syncVolumeSlider(setting.volume);

    state = MENU;
}

void Game::loadAssets() {
    // --- UI Sprites ---
    font.loadFromFile(FONT_PATH);

    bgMenu.loadFromFile(TEXTURE_BG_MENU);
    bgOver.loadFromFile(TEXTURE_BG_GAMEOVER);

    // --- Sound Load ---
    if (!eatBuffer.loadFromFile(SOUND_EAT)) {
        std::cerr << "Couldn't load eat sound\n";
    }
    eatSound.setBuffer(eatBuffer);

    if (!bufferButton.loadFromFile(SOUND_BUTTON)) {
        std::cerr << "Failed to load button.wav\n";
    }
    soundButton.setBuffer(bufferButton);

    if (!bufferGameOver.loadFromFile(SOUND_GAMEOVER)) {
        std::cerr << "Failed to load game_over.wav\n";
    }
    soundGameOver.setBuffer(bufferGameOver);

    if (!bufferExplode.loadFromFile(SOUND_EXPLODE)) {
        std::cerr << "Failed to load explode.wav\n";
    }
    soundExplode.setBuffer(bufferExplode);

    if (!bufferSuperEat.loadFromFile(SOUND_SUPER_EAT)) {
        std::cerr << "Failed to load bonus.wav\n";
    }
    soundSuperEat.setBuffer(bufferSuperEat);

    // Apply initial volume from settings
    soundButton.setVolume(setting.volume);
    soundGameOver.setVolume(setting.volume);
    eatSound.setVolume(setting.volume);
    soundExplode.setVolume(setting.volume);
    soundSuperEat.setVolume(setting.volume);

    // --- Objects Sprites ---
    snakeTex.loadFromFile(TEXTURE_SNAKE);
    fruitTex.loadFromFile(TEXTURE_FRUIT);
    bombTex.loadFromFile(TEXTURE_BOMB);
    superFruitTex.loadFromFile(TEXTURE_SUPER_FRUIT);

    snakeSprite.setTexture(snakeTex);
    fruitSprite.setTexture(fruitTex);
    bombSprite.setTexture(bombTex);
    superFruitSprite.setTexture(superFruitTex);
}

void Game::reset() {
    // Recompute grid size and update systems
    sf::Vector2u winSize(setting.getVideoMode().width, setting.getVideoMode().height);
    gridSize = getPlayAreaSize(winSize);
    snake.setGridSize(gridSize.x, gridSize.y);
    objects.setGridSize(gridSize.x, gridSize.y);

    objects.reset();

    snake.reset();
    score = 0;
    delay = INITIAL_DELAY;
    timer = 0;
    objects.spawnFruit(snake);
    snake.setWrappingEnabled(mode == NORMAL_MODE);
    gameClock.restart();
    buttonTimer.restart();
    state = PLAYING;
}

void Game::update(float dt) {
    if (state != PLAYING) return;

    // Always update super fruit timer
    objects.updateSuperFruit(dt, score, snake);
    // Update bomb visibility after score change
    if (skipNextBombUpdate) {
        skipNextBombUpdate = false;  
    }
    else {
        objects.updateBomb(score, snake);
    }

    if (isDying) {
        dyingTimer -= dt;
        if (dyingTimer <= 0.f) {
            isDying = false;
            state = GAME_OVER;
        }
        return; 
    }

    timer += dt;
    if (timer > delay) {
        timer = 0;
        snake.move();

        // --- Check border collision (in fire-border mode)
        if (!snake.isWithinBounds()) {
            soundGameOver.play();
            if (score > highScore) highScore = score;
            saveHighScoreAndSettingsOnly();

            isDying = true;
            dyingTimer = 2.f;
            return;
        }

        // --- Check self bite ---
        if (snake.checkSelfCollision()) {
            soundGameOver.play();
            if (score > highScore) highScore = score;
            saveHighScoreAndSettingsOnly();

            isDying = true;
            dyingTimer = 2.f; 
            return;
        }

        // --- Check super fruit ---
        if (objects.checkSuperFruitCollision(snake)) {
            score += 5;
            soundSuperEat.play();
        }

        // --- Check bomb ---
        if (objects.checkBombCollision(snake)) {
            soundExplode.play();
            if (score > highScore) highScore = score;
            saveHighScoreAndSettingsOnly();

            isDying = true;
            dyingTimer = 2.f;
            return;
        }

        // --- Check Fruit ---
        if (objects.checkFruitCollision(snake)) {
            snake.grow();
            score++;
            eatSound.play();

            if (score % SPEED_SCORE_THRESHOLD == 0) delay *= SPEED_INCREMENT;
            objects.spawnFruit(snake);
        }
    }
}

void Game::render(sf::RenderWindow& window) {
    // --- Set background based on game state ---
    if (state == MENU) {
        bgSprite.setTexture(bgMenu);
        bgSprite.setColor(sf::Color::White);  
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
        window.clear(RetroGreen); 

        sf::Vector2u winSize = window.getSize(); 
        int left = PLAY_AREA_MARGIN; 
        int top = TOP_BAR_HEIGHT + PLAY_AREA_MARGIN; 
        int width = winSize.x - 2 * PLAY_AREA_MARGIN; 
        int height = winSize.y - 2 * PLAY_AREA_MARGIN - TOP_BAR_HEIGHT; 

        sf::RectangleShape border;
        border.setSize({ float(width) - 10, float(height) - 3});  
        border.setPosition(left + 8.5f, top + 5.0f);              
        border.setOutlineThickness(5.f);
        border.setOrigin(2.5f, 2.5f);
        border.setOutlineColor(mode == FIRE_BORDER_MODE ? sf::Color::Red : sf::Color::Black);
        border.setFillColor(sf::Color::Transparent);
        window.draw(border);

    }
    else if (state == PAUSED)
    {
        window.clear(RetroGreen); 
    }
    else if (state == MENU_WITH_SAVED) {
        bgSprite.setTexture(bgMenu);
        bgSprite.setColor(sf::Color::White);  
        sf::Vector2u winSize = window.getSize();
        bgSprite.setScale(
            float(winSize.x) / bgMenu.getSize().x,
            float(winSize.y) / bgMenu.getSize().y
        );
        window.draw(bgSprite);
    }

    // --- Draw UI and objects depending on state ---
    sf::Vector2u winSize = window.getSize();
    switch (state) {
    case MENU:
    case MENU_WITH_SAVED:
        ui->drawMenu(window, hasSavedSession);  
        break;
    case MODE_SELECTION:
        ui->drawModeSelection(window);
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

        sf::Vector2i offset = getPlayAreaOffset(); 
        sf::Vector2f pixelOffset(
            PLAY_AREA_MARGIN + 7,
            PLAY_AREA_MARGIN + TOP_BAR_HEIGHT + 3
        );

        // Draw snake
        for (const auto& segment : snake.getBody()) {
            snakeSprite.setPosition(
                segment.x * TILE_SIZE + pixelOffset.x,
                segment.y * TILE_SIZE + pixelOffset.y
            );

            if (isDying) {
                // Flicker: alternate white every ~0.2 seconds
                float flicker = fmod(dyingTimer, 0.4f);
                if (flicker < 0.2f)
                    snakeSprite.setColor(sf::Color::White);
                else
                    snakeSprite.setColor(sf::Color::Red);  
            }
            else {
                snakeSprite.setColor(sf::Color::White); 
            }

            window.draw(snakeSprite);
        }

        // --- Draw Bomb ---
        const Bomb& bomb = objects.getBomb();
        if (bomb.active) {
            bombSprite.setPosition(
                bomb.x * TILE_SIZE + pixelOffset.x,
                bomb.y * TILE_SIZE + pixelOffset.y
            );

            // Flicker every 0.5 seconds
            float flicker = fmod(gameClock.getElapsedTime().asSeconds(), 1.0f);
            if (flicker < 0.5f)
                bombSprite.setColor(sf::Color::White);
            else
                bombSprite.setColor(sf::Color::Black); 

            window.draw(bombSprite);
        }

        // --- Draw super fruit ---
        const SuperFruit& sfruit = objects.getSuperFruit();
        if (sfruit.active) {
            float pulseTime = gameClock.getElapsedTime().asSeconds();
            float scale = 1.0f + 0.2f * sin(pulseTime * 8); 

            superFruitSprite.setScale(scale, scale);

            superFruitSprite.setPosition(
                sfruit.x * TILE_SIZE + pixelOffset.x,
                sfruit.y * TILE_SIZE + pixelOffset.y
            );

            // Adjust origin to center pulse 
            superFruitSprite.setOrigin(8, 8);
            superFruitSprite.setPosition(
                sfruit.x * TILE_SIZE + pixelOffset.x + 8,
                sfruit.y * TILE_SIZE + pixelOffset.y + 8
            );

            window.draw(superFruitSprite);
        }

        // --- Draw fruit ---
        const Fruit& fruit = objects.getFruit();
        fruitSprite.setPosition(
            fruit.x * TILE_SIZE + pixelOffset.x,
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
        bgSprite.setColor(sf::Color(255, 255, 255, 150)); 
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
        bgSprite.setColor(sf::Color(255, 255, 255, 150)); 
        winSize = window.getSize();
        bgSprite.setScale(
            float(winSize.x) / bgMenu.getSize().x,
            float(winSize.y) / bgMenu.getSize().y
        );
        window.draw(bgSprite);
        ui->drawResolutionMenu(window); 
        break;
    }
    case SETTINGS_SOUND:
    {
        bgSprite.setTexture(bgMenu);
        bgSprite.setColor(sf::Color(255, 255, 255, 150)); 
        winSize = window.getSize();
        bgSprite.setScale(
            float(winSize.x) / bgMenu.getSize().x,
            float(winSize.y) / bgMenu.getSize().y
        );
        window.draw(bgSprite);
        ui->drawSoundSettings(window);  
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

    // --- Sound Slider ---
    // Check if mouse is pressed
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        // Only start dragging if clicking on the knob
        if (!ui->draggingVolume && ui->volumeKnob.getGlobalBounds().contains(mousePos)) {
            ui->draggingVolume = true;
        }
    }
    else {
        // Stop dragging when mouse is released
        ui->draggingVolume = false;
    }
    if (ui->draggingVolume) {
        float sliderX = ui->volumeTrack.getPosition().x;
        float sliderW = ui->volumeTrack.getSize().x;

        float newX = mousePos.x;
        if (newX < sliderX) newX = sliderX;
        if (newX > sliderX + sliderW) newX = sliderX + sliderW;

        ui->volumeKnob.setPosition(newX, ui->volumeKnob.getPosition().y);

        ui->volumeLevel = static_cast<int>(((newX - sliderX) / sliderW) * 100);

        // Live update
        setting.volume = ui->volumeLevel;

        soundButton.setVolume(ui->volumeLevel);
        soundGameOver.setVolume(ui->volumeLevel);
        eatSound.setVolume(ui->volumeLevel);
    }

    // --- Button Behavior ---
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && canClick()) {
        if (state == MENU || state == MENU_WITH_SAVED) {
            if (isMouseOver(ui->startButton.shape, mousePos)) {
                soundButton.play();
                if (hasSavedSession) {
                    confirmationType = CONFIRM_NEW_GAME;
                    ui->confirmText.setString("Start a new game?\nYour current session will be lost.");
                    state = CONFIRMATION;
                }
                else {
                    // Ensure grid is recomputed
                    gridSize = getPlayAreaSize(window.getSize());
                    snake.setGridSize(gridSize.x, gridSize.y);
                    objects.setGridSize(gridSize.x, gridSize.y);
                    state = MODE_SELECTION;
                }
            }
            else if (isMouseOver(ui->continueButton.shape, mousePos) && hasSavedSession) {
                soundButton.play();
                loadSession();
            }
            else if (isMouseOver(ui->exitButton.shape, mousePos)) {
                soundButton.play();
                confirmationType = CONFIRM_EXIT;
                ui->confirmText.setString("Are you sure you want to quit?");
                state = CONFIRMATION;
            }
            else if (isMouseOver(ui->settingsButton.shape, mousePos)) {
                soundButton.play();
                state = SETTINGS_MENU;
            }
        }

        else if (state == MODE_SELECTION) {
            if (isMouseOver(ui->normalModeButton.shape, mousePos)) {
                soundButton.play();
                mode = NORMAL_MODE;
                snake.setWrappingEnabled(true);
                gridSize = getPlayAreaSize(window.getSize());
                snake.setGridSize(gridSize.x, gridSize.y);
                objects.setGridSize(gridSize.x, gridSize.y);
                reset();
            }
            else if (isMouseOver(ui->fireBorderModeButton.shape, mousePos)) {
                soundButton.play();
                mode = FIRE_BORDER_MODE;
                snake.setWrappingEnabled(false);
                gridSize = getPlayAreaSize(window.getSize());
                snake.setGridSize(gridSize.x, gridSize.y);
                objects.setGridSize(gridSize.x, gridSize.y);
                reset();
            }
            else if (isMouseOver(ui->backButton.shape, mousePos)) {
                soundButton.play();
                state = MENU;
            }
        }

        else if (state == GAME_OVER) {
            if (isMouseOver(ui->restartButton.shape, mousePos)) {
                soundButton.play();
                state = MODE_SELECTION;
            }
            else if (isMouseOver(ui->exitButton.shape, mousePos)) {
                soundButton.play();

                //  Make sure highScore is up-to-date before saving
                if (score > highScore) {
                    highScore = score;
                }

                confirmationType = CONFIRM_EXIT;
                saveHighScoreAndSettingsOnly();
                ui->confirmText.setString("Are you sure you want to quit?");
                state = CONFIRMATION;
            }
        }

        else if (state == PLAYING && ui->pauseCircle.getGlobalBounds().contains(mousePos)) {
            soundButton.play();
            state = PAUSED;
        }

        else if (state == PAUSED) {
            if (isMouseOver(ui->resumeButton.shape, mousePos)) {
                soundButton.play();
                state = PLAYING;
            }
            else if (isMouseOver(ui->menuButton.shape, mousePos)) {
                soundButton.play();
                saveSession(); 
                state = MENU_WITH_SAVED;
            }
        }

        else if (state == CONFIRMATION) {
            if (isMouseOver(ui->yesButton.shape, mousePos)) {
                if (confirmationType == CONFIRM_NEW_GAME) {
                    soundButton.play();
                    hasSavedSession = false;
                    state = MODE_SELECTION;
                }
                else if (confirmationType == CONFIRM_EXIT) {
                    soundButton.play();
                    window.close();
                }
                else if (confirmationType == CONFIRM_RESIZE) {
                    setting.resolution = pendingResolution;     
                    setting.volume = ui->volumeLevel;
                    setting.saveToFile();

                    applyResolutionChange(window);
                    ui->resize(window.getSize().x, window.getSize().y);

                    hasSavedSession = false;
                    state = SETTINGS_RESOLUTION;
                }
            }
            else if (isMouseOver(ui->noButton.shape, mousePos)) {
                soundButton.play();
                state = hasSavedSession ? MENU_WITH_SAVED : MENU;
            }
        }

        else if (state == SETTINGS_MENU) {
            if (isMouseOver(ui->resolutionButton.shape, mousePos)) {
                soundButton.play();
                state = SETTINGS_RESOLUTION;
            }
            else if (isMouseOver(ui->soundButton.shape, mousePos)) {
                soundButton.play();
                state = SETTINGS_SOUND;
            }
            else if (isMouseOver(ui->backButton.shape, mousePos)) {
                soundButton.play();
                state = MENU;
            }
        }
        else if (state == SETTINGS_RESOLUTION) {
            if (setting.resolution != RES_800x600 && isMouseOver(ui->res800Button.shape, mousePos)) {
                soundButton.play();
                pendingResolution = RES_800x600;
                confirmationType = CONFIRM_RESIZE;
                ui->confirmText.setCharacterSize(14);
                ui->confirmText.setString("Changing field size will delete \nyour current session. \nContinue?");
                state = CONFIRMATION;
            }

            else if (setting.resolution != RES_1600x900 && isMouseOver(ui->res1600Button.shape, mousePos)) {
                soundButton.play();
                pendingResolution = RES_1600x900;
                confirmationType = CONFIRM_RESIZE;
                ui->confirmText.setString("Changing field size will delete \nyour current session. \nContinue?");
                state = CONFIRMATION;
            }

            else if (setting.resolution != RES_FULLSCREEN && isMouseOver(ui->fullscreenButton.shape, mousePos)) {
                soundButton.play();
                pendingResolution = RES_FULLSCREEN;
                confirmationType = CONFIRM_RESIZE;
                ui->confirmText.setString("Changing field size will delete \nyour current session. \nContinue?");
                state = CONFIRMATION;
            }

            else if (
                (setting.resolution == RES_800x600 && isMouseOver(ui->res800Button.shape, mousePos)) ||
                (setting.resolution == RES_1600x900 && isMouseOver(ui->res1600Button.shape, mousePos)) ||
                (setting.resolution == RES_FULLSCREEN && isMouseOver(ui->fullscreenButton.shape, mousePos))
                ) {
                // Already selected, optional feedback
                soundButton.play();
            }

            else if (isMouseOver(ui->backButton.shape, mousePos)) {
                soundButton.play();
                state = SETTINGS_MENU;
            }
        }
        else if (state == SETTINGS_SOUND) {
            if (!ui->draggingVolume && isMouseOver(ui->backButton.shape, mousePos)) {
                soundButton.play();
                setting.volume = ui->volumeLevel;

                setting.saveToFile();  

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
    // Update game state
    state = newState;
}

void Game::applyResolutionChange(sf::RenderWindow& window) {
    sf::VideoMode mode = setting.getVideoMode();
    sf::Uint32 style = setting.getWindowStyle();

    saveSession(); 

    window.create(mode, "Snake Game", style);
    window.setFramerateLimit(60);

    // Recreate UI with updated layout
    delete ui;
    ui = new UI(font, mode.width, mode.height);
    ui->resize(mode.width, mode.height);

    ui->syncVolumeSlider(setting.volume);  

    // Update grid size and inform systems that depend on it
    gridSize = getPlayAreaSize(window.getSize());
    snake.setGridSize(gridSize.x, gridSize.y);
    objects.setGridSize(gridSize.x, gridSize.y);
}

void Game::saveSession() {
    savedSnake = snake.getBody();
    savedDirection = snake.getDirection();
    savedFruit = objects.getFruit();
    savedBomb = objects.getBomb();  
    savedNextBombToggleScore = objects.getNextBombToggleScore();
    savedSuperFruit = objects.getSuperFruitState(); 
    savedNextSuperFruitScore = objects.getNextSuperFruitScore();
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

    objects.loadBomb(savedBomb);  
    objects.setNextBombToggleScore(savedNextBombToggleScore);

    objects.loadSuperFruit(savedSuperFruit);  
    objects.setNextSuperFruitScore(savedNextSuperFruitScore);

    score = savedScore;
    delay = savedDelay;

    // Check if fruit is still in bounds
    sf::Vector2i offset = getPlayAreaOffset();
    if (savedFruit.x < offset.x || savedFruit.x >= offset.x + gridSize.x ||
        savedFruit.y < offset.y || savedFruit.y >= offset.y + gridSize.y)
    {
        objects.spawnFruit(snake);
    }

    skipNextBombUpdate = true;

    gameClock.restart();
    state = PLAYING;
}

void Game::saveSessionToFile() {
    SaveManager::saveSessionToFile(
        SAVE_FILE_PATH, 
        highScore, 
        savedSnake, 
        savedFruit, 
        savedBomb,                   
        savedSuperFruit,
        savedNextBombToggleScore,     
        savedNextSuperFruitScore,
        savedScore, 
        savedDelay, 
        savedDirection,
        static_cast<int>(mode));
}

void Game::loadSessionFromFile() {
    if (!SaveManager::validate(SAVE_FILE_PATH)) {
        hasSavedSession = false;
        return;
    }

    Bomb loadedBomb;
    int bombToggleScore = 0;
    SuperFruit loadedSuper;
    int superScore = 0;
    int modeInt = 0;

    if (SaveManager::loadSessionFromFile(
        SAVE_FILE_PATH, 
        highScore, 
        savedSnake, 
        savedFruit, 
        loadedBomb, 
        loadedSuper,
        bombToggleScore,
        superScore,
        savedScore, 
        savedDelay, 
        savedDirection,
        modeInt)) {
        savedBomb = loadedBomb;
        savedNextBombToggleScore = bombToggleScore;
        savedSuperFruit = loadedSuper;
        savedNextSuperFruitScore = superScore;
        mode = static_cast<GameMode>(modeInt);
        snake.setWrappingEnabled(mode == NORMAL_MODE);
        hasSavedSession = true;
    }
    else {
        hasSavedSession = false;
    }
}

void Game::saveHighScoreAndSettingsOnly() {
    SaveManager::saveHighScoreAndSettingsOnly(SAVE_FILE_PATH, highScore);
    hasSavedSession = false;
}

