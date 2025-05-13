// UI.cpp
// --------------------------------------------------
// Implements the rendering of UI screens and elements
// --------------------------------------------------

#include "UI.h"
#include "Utils.h"
#include "Config.h"

UI::UI(sf::Font& f, float w, float h) : font(f), width(w), height(h) {
    buttonTexture.loadFromFile("images/button.png");
    resize(w, h);
}

void UI::resize(float w, float h) {
    width = w;
    height = h;

    const float buttonSpacing = 15.f;
    const float buttonHeight = 40.f;
    const float startY = height / 2.f - 80.f;

    // --- Main Menu Buttons ---
    centerButton(continueButton, "Continue", startY, sf::Color(100, 180, 220));
    centerButton(startButton, "Start", startY + 1 * (buttonHeight + buttonSpacing), sf::Color(180, 100, 100));
    centerButton(settingsButton, "Settings", startY + 2 * (buttonHeight + buttonSpacing), sf::Color(150, 150, 150));
    centerButton(exitButton, "Exit", startY + 3 * (buttonHeight + buttonSpacing), sf::Color(150, 150, 150));

    // --- Pause Menu ---
    float pauseY = height / 2.f - 60.f;
    centerButton(resumeButton, "Resume", pauseY, sf::Color(100, 180, 100));
    centerButton(menuButton, "Start Menu", pauseY + buttonHeight + buttonSpacing, sf::Color(100, 140, 200));

    // --- Game Over Buttons (reuse Restart + Exit) ---
    centerButton(restartButton, "Restart", height / 2.f + 40.f, sf::Color(180, 100, 100));

    // --- Settings Titles ---
    float titleY = 50.f;
    settingsTitle.setFont(font);
    settingsTitle.setCharacterSize(16);
    settingsTitle.setStyle(sf::Text::Bold);
    settingsTitle.setFillColor(sf::Color::Black);
    settingsTitle.setString("Settings");  // will be updated dynamically
    sf::FloatRect titleBounds = settingsTitle.getLocalBounds();
    settingsTitle.setOrigin(titleBounds.left + titleBounds.width / 2, titleBounds.top + titleBounds.height / 2);
    settingsTitle.setPosition(width / 2.f, titleY);

    // --- Mode Select Titles ---
    float modeTitleY = 50.f;
    modeSelectTitle.setFont(font);
    modeSelectTitle.setCharacterSize(16);
    modeSelectTitle.setStyle(sf::Text::Bold);
    modeSelectTitle.setFillColor(sf::Color::Black);
    modeSelectTitle.setString("Game Mode");
    sf::FloatRect modeTitleBounds = modeSelectTitle.getLocalBounds();
    modeSelectTitle.setOrigin(modeTitleBounds.left + modeTitleBounds.width / 2, modeTitleBounds.top + modeTitleBounds.height / 2);
    modeSelectTitle.setPosition(width / 2.f, modeTitleY);

    // --- Settings Root Buttons ---
    float settingsMenuStartY = height / 2.f - (buttonHeight + buttonSpacing);
    centerButton(resolutionButton, "Resolution", settingsMenuStartY, sf::Color(120, 120, 180));
    centerButton(soundButton, "Sound", settingsMenuStartY + buttonHeight + buttonSpacing, sf::Color(120, 120, 180));

    // --- Shared Back Button (bottom of all submenus) ---
    float backButtonY = height - 80.f;
    centerButton(backButton, "Back", backButtonY, sf::Color(120, 120, 120));

    // --- Resolution Options ---
    float resStartY = height / 2.f - 80.f;
    centerButton(res800Button, "800 x 600", resStartY, sf::Color(100, 100, 200), 14);
    centerButton(res1600Button, "1600 x 900", resStartY + (buttonHeight + buttonSpacing), sf::Color(100, 100, 200), 14);
    centerButton(fullscreenButton, "Fullscreen", resStartY + 2 * (buttonHeight + buttonSpacing), sf::Color(100, 100, 200), 14);

    // --- Mode Selection ---
    float modeStartY = height / 2.f - 80.f;
    centerButton(onlySelfCollisionButton, "Borderless", modeStartY, sf::Color(100, 100, 200), 14);
    centerButton(withWallCollisionButton, "Bounded", modeStartY + (buttonHeight + buttonSpacing), sf::Color(100, 100, 200), 14);

    // --- Pause Circle + Bars ---
    float radius = 20.f;
    pauseCircle.setRadius(radius);
    pauseCircle.setFillColor(sf::Color(200, 200, 120));
    pauseCircle.setPosition(width - radius * 2 - 10, 5);
    pauseCircle.setOutlineColor(sf::Color::Black);
    pauseCircle.setOutlineThickness(2);

    pauseBar1.setSize({ 5.f, 14.f });
    pauseBar2.setSize({ 5.f, 14.f });
    pauseBar1.setFillColor(sf::Color::Black);
    pauseBar2.setFillColor(sf::Color::Black);
    sf::Vector2f center = pauseCircle.getPosition() + sf::Vector2f(radius, radius);
    pauseBar1.setPosition(center.x - 8, center.y - 7);
    pauseBar2.setPosition(center.x + 3, center.y - 7);

    // --- Game Over Text ---
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(24);
    gameOverText.setFillColor(sf::Color::Black);

    // --- Confirmation Box ---
    confirmText.setFont(font);
    confirmText.setCharacterSize(14);
    confirmText.setFillColor(sf::Color::Black);
    confirmText.setString("Start a new game?\nYour current session will be lost.");

    sf::FloatRect textBounds = confirmText.getLocalBounds();
    float padding = 20.f;
    float boxWidth = textBounds.width + padding * 2.f;
    float boxHeight = textBounds.height + padding * 2.f + buttonHeight;
    confirmBox.setSize({ boxWidth, boxHeight });
    confirmBox.setFillColor(sf::Color(240, 240, 240));
    confirmBox.setOutlineColor(sf::Color::Black);
    confirmBox.setOutlineThickness(3);
    confirmBox.setPosition((width - boxWidth) / 2.f, (height - boxHeight) / 2.f);

    confirmText.setPosition(
        confirmBox.getPosition().x + padding,
        confirmBox.getPosition().y + padding
    );

    float btnY = confirmBox.getPosition().y + boxHeight - buttonHeight;
    float btnWidth = boxWidth / 2;

    yesButton.shape.setSize({ btnWidth, buttonHeight });
    yesButton.shape.setFillColor(sf::Color(120, 180, 120));
    yesButton.shape.setPosition(confirmBox.getPosition().x, btnY);
    yesButton.label.setFont(font);
    yesButton.label.setCharacterSize(14);
    yesButton.label.setString("Yes");
    yesButton.label.setFillColor(sf::Color::White);
    sf::FloatRect yesBounds = yesButton.label.getLocalBounds();
    yesButton.label.setOrigin(yesBounds.left + yesBounds.width / 2, yesBounds.top + yesBounds.height / 2);
    yesButton.label.setPosition(yesButton.shape.getPosition().x + btnWidth / 2, btnY + buttonHeight / 2);

    noButton.shape.setSize({ btnWidth, buttonHeight });
    noButton.shape.setFillColor(sf::Color(180, 120, 120));
    noButton.shape.setPosition(confirmBox.getPosition().x + btnWidth, btnY);
    noButton.label.setFont(font);
    noButton.label.setCharacterSize(14);
    noButton.label.setString("No");
    noButton.label.setFillColor(sf::Color::White);
    sf::FloatRect noBounds = noButton.label.getLocalBounds();
    noButton.label.setOrigin(noBounds.left + noBounds.width / 2, noBounds.top + noBounds.height / 2);
    noButton.label.setPosition(noButton.shape.getPosition().x + btnWidth / 2, btnY + buttonHeight / 2);
}

void UI::centerButton(UIButton& button, const std::string& text, float topY, sf::Color color, unsigned size) {
    float buttonWidth = 200.f;
    float buttonHeight = 40.f;

    button.shape.setSize({ buttonWidth, buttonHeight });
    button.shape.setTexture(&buttonTexture);
    button.shape.setFillColor(sf::Color::White);  // Optional: resets tint color
    button.shape.setPosition((width - buttonWidth) / 2.f, topY);

    button.label.setFont(font);
    button.label.setCharacterSize(size);
    button.label.setString(text);
    button.label.setFillColor(sf::Color::White);

    sf::FloatRect textBounds = button.label.getLocalBounds();
    button.label.setOrigin(textBounds.left + textBounds.width / 2.f, textBounds.top + textBounds.height / 2.f);
    button.label.setPosition(button.shape.getPosition().x + buttonWidth / 2.f, topY + buttonHeight / 2.f);
}

void UI::drawMenu(sf::RenderWindow& window, bool showContinue) {
    if (showContinue) {
        window.draw(continueButton.shape);
        window.draw(continueButton.label);
    }
    window.draw(startButton.shape);
    window.draw(startButton.label);
    window.draw(settingsButton.shape);
    window.draw(settingsButton.label);
    window.draw(exitButton.shape);
    window.draw(exitButton.label);
}

void UI::drawPause(sf::RenderWindow& window) {
    window.draw(resumeButton.shape); window.draw(resumeButton.label);
    window.draw(menuButton.shape); window.draw(menuButton.label);
}

void UI::drawGameOver(sf::RenderWindow& window, int score, int highScore) {
    sf::Text line1("Game Over!", font, 20);
    sf::Text line2("Final Score: " + std::to_string(score), font, 16);
    sf::Text line3("High Score: " + std::to_string(highScore), font, 16);

    line1.setFillColor(sf::Color::Black);
    line2.setFillColor(sf::Color::Black);
    line3.setFillColor(sf::Color::Black);

    // Center and position each line using existing helper
    centerText(line1, width, height / 2.f - 80.f);
    centerText(line2, width, height / 2.f - 40.f);
    centerText(line3, width, height / 2.f - 10.f);

    window.draw(line1);
    window.draw(line2);
    window.draw(line3);

    // Buttons
    window.draw(restartButton.shape);
    window.draw(restartButton.label);
    window.draw(exitButton.shape);
    window.draw(exitButton.label);
}

void UI::drawHUD(sf::RenderWindow& window, int score, int highScore) {
    sf::RectangleShape topBar(sf::Vector2f(width, 30));
    topBar.setFillColor(RetroGreen);
    window.draw(topBar);

    window.draw(pauseCircle);
    window.draw(pauseBar1);
    window.draw(pauseBar2);

    // Combine into one centered block
    std::string scoreStr = "Score: " + std::to_string(score);
    std::string highScoreStr = "High Score: " + std::to_string(highScore);
    sf::Text scoreBlock(scoreStr + "    " + highScoreStr, font, 14);
    scoreBlock.setFillColor(sf::Color::Black);

    // Center the block
    sf::FloatRect bounds = scoreBlock.getLocalBounds();
    scoreBlock.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    scoreBlock.setPosition(width / 2.f, 15);  // Middle of topBar

    window.draw(scoreBlock);
}

void UI::drawConfirmation(sf::RenderWindow& window) {
    window.draw(confirmBox);
    window.draw(confirmText);
    window.draw(yesButton.shape); window.draw(yesButton.label);
    window.draw(noButton.shape); window.draw(noButton.label);
}

void UI::drawSettingsRoot(sf::RenderWindow& window) {
    sf::FloatRect titleBounds = settingsTitle.getLocalBounds();
    settingsTitle.setOrigin(titleBounds.left + titleBounds.width / 2.f, titleBounds.top + titleBounds.height / 2.f);
    settingsTitle.setPosition(width / 2.f, 50.f);
    settingsTitle.setString("Settings");
    window.draw(settingsTitle);
    window.draw(resolutionButton.shape); window.draw(resolutionButton.label);
    window.draw(soundButton.shape); window.draw(soundButton.label);
    window.draw(backButton.shape); window.draw(backButton.label);
}

void UI::drawResolutionMenu(sf::RenderWindow& window) {
    sf::FloatRect titleBounds = settingsTitle.getLocalBounds();
    settingsTitle.setOrigin(titleBounds.left + titleBounds.width / 2.f, titleBounds.top + titleBounds.height / 2.f);
    settingsTitle.setPosition(width / 2.f, 50.f);
    settingsTitle.setString("Resolution Settings");
    window.draw(settingsTitle);
    window.draw(res800Button.shape); window.draw(res800Button.label);
    window.draw(res1600Button.shape); window.draw(res1600Button.label);
    window.draw(fullscreenButton.shape); window.draw(fullscreenButton.label);
    window.draw(backButton.shape); window.draw(backButton.label);
}

void UI::drawModeSelectionMenu(sf::RenderWindow& window) {
    sf::FloatRect titleBounds = modeSelectTitle.getLocalBounds();
    modeSelectTitle.setOrigin(titleBounds.left + titleBounds.width / 2.f, titleBounds.top + titleBounds.height / 2.f);
    modeSelectTitle.setPosition(width / 2.f, 50.f);
    modeSelectTitle.setString("Game Mode");
    window.draw(modeSelectTitle);
    window.draw(onlySelfCollisionButton.shape); window.draw(onlySelfCollisionButton.label);
    window.draw(withWallCollisionButton.shape); window.draw(withWallCollisionButton.label);
}

void UI::drawSoundSettings(sf::RenderWindow& window) {
    sf::FloatRect titleBounds = settingsTitle.getLocalBounds();
    settingsTitle.setOrigin(titleBounds.left + titleBounds.width / 2.f, titleBounds.top + titleBounds.height / 2.f);
    settingsTitle.setPosition(width / 2.f, 50.f);
    settingsTitle.setString("Sound Settings");
    window.draw(settingsTitle);
    window.draw(backButton.shape);
    window.draw(backButton.label);
}

void UI::updateHoverEffect(const sf::Vector2f& mousePos) {
    std::vector<UIButton*> buttons = {
    &startButton, &resumeButton, &menuButton, &restartButton,
    &exitButton, &continueButton, &yesButton, &noButton,
    &settingsButton, &res800Button, &res1600Button, &fullscreenButton, &backButton,
    &resolutionButton, &soundButton, &onlySelfCollisionButton, &withWallCollisionButton
    };

    for (auto btn : buttons) {
        if (btn->shape.getSize().x == 0) continue; // not used
        if (btn->shape.getGlobalBounds().contains(mousePos)) {
            if (btn == &yesButton) {
                btn->shape.setFillColor(sf::Color(80, 140, 80));  // darker green
            }
            else if (btn == &noButton) {
                btn->shape.setFillColor(sf::Color(140, 80, 80));  // darker red
            }
            else {
                btn->shape.setFillColor(sf::Color(100, 100, 255, 255));  // visible blue tint
            }
        }
        else {
            // Reset to original color based on button
            if (btn == &startButton || btn == &restartButton)
                btn->shape.setFillColor(sf::Color(255, 255, 255, 200));  // normal semi-transparent
            else if (btn == &resumeButton)
                btn->shape.setFillColor(sf::Color(255, 255, 255, 200));  // normal semi-transparent
            else if (btn == &menuButton)
                btn->shape.setFillColor(sf::Color(255, 255, 255, 200));  // normal semi-transparent
            else if (btn == &exitButton)
                btn->shape.setFillColor(sf::Color(255, 255, 255, 200));  // normal semi-transparent
            else if (btn == &continueButton)
                btn->shape.setFillColor(sf::Color(255, 255, 255, 200));  // normal semi-transparent
            else if (btn == &yesButton)
                btn->shape.setFillColor(sf::Color(120, 180, 120));
            else if (btn == &noButton)
                btn->shape.setFillColor(sf::Color(180, 120, 120));
            else if (btn == &settingsButton || btn == &backButton)
                btn->shape.setFillColor(sf::Color(255, 255, 255, 200));  // normal semi-transparent
            else if (btn == &res800Button || btn == &res1600Button || btn == &fullscreenButton)
                btn->shape.setFillColor(sf::Color(255, 255, 255, 200));  // normal semi-transparent
            else if (btn == &resolutionButton || btn == &soundButton)
                btn->shape.setFillColor(sf::Color(255, 255, 255, 200));  // normal semi-transparent
            else if (btn == &onlySelfCollisionButton || btn == &withWallCollisionButton)
                btn->shape.setFillColor(sf::Color(255, 255, 255, 200));  // normal semi-transparent
        }
        if (pauseCircle.getGlobalBounds().contains(mousePos)) {
            pauseCircle.setFillColor(sf::Color(160, 160, 90));  // ← Hover color
        }
        else {
            pauseCircle.setFillColor(sf::Color(200, 200, 120)); // ← Default color
        }
    }
}
