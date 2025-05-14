// UI.cpp
// --------------------------------------------------
// Implements the rendering of UI screens and elements
// --------------------------------------------------

#include "UI.h"
#include "Utils.h"
#include "Config.h"

UI::UI(sf::Font& f, float w, float h) : font(f), width(w), height(h) {
    buttonTexture.loadFromFile(TEXTURE_BUTTON);
    resize(w, h);
}

void UI::layoutMenuButtons(float startY, float h, float spacing) {
    centerButton(continueButton, "Continue", startY, sf::Color(100, 180, 220));
    centerButton(startButton, "Start", startY + 1 * (h + spacing), sf::Color(180, 100, 100));
    centerButton(settingsButton, "Settings", startY + 2 * (h + spacing), sf::Color(150, 150, 150));
    centerButton(exitButton, "Exit", startY + 3 * (h + spacing), sf::Color(150, 150, 150));
}

void UI::layoutModeButtons(float startY, float spacing) {
    centerButton(normalModeButton, "Normal Mode", startY, sf::Color(100, 180, 100));
    centerButton(fireBorderModeButton, "Fire-Border Mode", startY + spacing, sf::Color(200, 80, 80));
}

void UI::layoutPauseMenu(float startY, float h, float spacing) {
    centerButton(resumeButton, "Resume", startY, sf::Color(100, 180, 100));
    centerButton(menuButton, "Start Menu", startY + h + spacing, sf::Color(100, 140, 200));
}

void UI::layoutSettingsRoot(float startY, float h, float spacing) {
    centerButton(resolutionButton, "Field Size", startY, sf::Color(120, 120, 180));
    centerButton(soundButton, "Sound", startY + h + spacing, sf::Color(120, 120, 180));
}

void UI::layoutResolutionOptions(float startY, float h, float spacing) {
    centerButton(res800Button, "800 x 600", startY, sf::Color(100, 100, 200), 14);
    centerButton(res1600Button, "1600 x 900", startY + (h + spacing), sf::Color(100, 100, 200), 14);
    centerButton(fullscreenButton, "Fullscreen", startY + 2 * (h + spacing), sf::Color(100, 100, 200), 14);
}

void UI::layoutVolumeControls(float sliderWidth) {
    const float sliderHeight = 6.f;
    const float sliderX = (width - sliderWidth) / 2.f;
    const float sliderY = height / 2.f - 10.f;

    volumeTrack.setSize({ sliderWidth, sliderHeight });
    volumeTrack.setFillColor(sf::Color(180, 180, 180));
    volumeTrack.setPosition(sliderX, sliderY);

    volumeKnob.setRadius(10.f);
    volumeKnob.setOrigin(10.f, 10.f);
    volumeKnob.setFillColor(sf::Color::White);

    float knobX = sliderX + (volumeLevel / 100.f) * sliderWidth;
    volumeKnob.setPosition(knobX, sliderY + sliderHeight / 2.f);

    volumeLabel.setFont(font);
    volumeLabel.setCharacterSize(14);
    volumeLabel.setFillColor(sf::Color::Black);
    volumeLabel.setString("Volume: 100%");
    volumeLabel.setPosition(width / 2.f - 50.f, volumeTrack.getPosition().y + 30.f);
}

void UI::layoutPauseIcon() {
    float radius = 20.f;
    pauseCircle.setRadius(radius);
    pauseCircle.setFillColor(sf::Color(200, 200, 120));
    pauseCircle.setOutlineColor(sf::Color::Black);
    pauseCircle.setOutlineThickness(2);
    pauseCircle.setPosition(width - radius * 2.f - 10.f, 5.f);

    pauseBar1.setSize({ 5.f, 14.f });
    pauseBar2.setSize({ 5.f, 14.f });
    pauseBar1.setFillColor(sf::Color::Black);
    pauseBar2.setFillColor(sf::Color::Black);

    sf::Vector2f center = pauseCircle.getPosition() + sf::Vector2f(radius, radius);
    pauseBar1.setPosition(center.x - 8.f, center.y - 7.f);
    pauseBar2.setPosition(center.x + 3.f, center.y - 7.f);
}

void UI::layoutConfirmationBox(float buttonHeight) {
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

    // --- Yes Button ---
    yesButton.shape.setSize({ btnWidth, buttonHeight });
    yesButton.shape.setFillColor(sf::Color(120, 180, 120));
    yesButton.shape.setPosition(confirmBox.getPosition().x, btnY);

    yesButton.label.setFont(font);
    yesButton.label.setCharacterSize(14);
    yesButton.label.setString("Yes");
    yesButton.label.setFillColor(sf::Color::White);
    sf::FloatRect yesBounds = yesButton.label.getLocalBounds();
    yesButton.label.setOrigin(yesBounds.left + yesBounds.width / 2.f, yesBounds.top + yesBounds.height / 2.f);
    yesButton.label.setPosition(yesButton.shape.getPosition().x + btnWidth / 2.f, btnY + buttonHeight / 2.f);

    // --- No Button ---
    noButton.shape.setSize({ btnWidth, buttonHeight });
    noButton.shape.setFillColor(sf::Color(180, 120, 120));
    noButton.shape.setPosition(confirmBox.getPosition().x + btnWidth, btnY);

    noButton.label.setFont(font);
    noButton.label.setCharacterSize(14);
    noButton.label.setString("No");
    noButton.label.setFillColor(sf::Color::White);
    sf::FloatRect noBounds = noButton.label.getLocalBounds();
    noButton.label.setOrigin(noBounds.left + noBounds.width / 2.f, noBounds.top + noBounds.height / 2.f);
    noButton.label.setPosition(noButton.shape.getPosition().x + btnWidth / 2.f, btnY + buttonHeight / 2.f);
}

void UI::resize(float w, float h) {
    width = w;
    height = h;

    const float spacing = 15.f;
    const float buttonHeight = 40.f;

    layoutMenuButtons(height / 2.f - 80.f, buttonHeight, spacing);
    layoutModeButtons(height / 2.f - 40.f, 60.f);
    layoutPauseMenu(height / 2.f - 60.f, buttonHeight, spacing);
    centerButton(restartButton, "Restart", height / 2.f + 40.f, sf::Color(180, 100, 100));

    // Settings Title
    settingsTitle.setFont(font);
    settingsTitle.setCharacterSize(16);
    settingsTitle.setStyle(sf::Text::Bold);
    settingsTitle.setFillColor(sf::Color::Black);
    settingsTitle.setString("Settings");
    sf::FloatRect titleBounds = settingsTitle.getLocalBounds();
    settingsTitle.setOrigin(titleBounds.left + titleBounds.width / 2, titleBounds.top + titleBounds.height / 2);
    settingsTitle.setPosition(width / 2.f, 50.f);

    layoutSettingsRoot(height / 2.f - (buttonHeight + spacing), buttonHeight, spacing);
    centerButton(backButton, "Back", height - 80.f, sf::Color(120, 120, 120));

    layoutResolutionOptions(height / 2.f - 80.f, buttonHeight, spacing);
    layoutVolumeControls(200.f);
    layoutPauseIcon();
    layoutConfirmationBox(buttonHeight);
}

void UI::centerButton(UIButton& button, const std::string& text, float topY, sf::Color color, unsigned size) {
    const float defaultWidth = 200.f;
    const float horizontalPadding = 30.f;
    const float verticalPadding = 20.f;

    // --- Label ---
    button.label.setFont(font);
    button.label.setCharacterSize(size);
    button.label.setString(text);
    button.label.setFillColor(sf::Color::White);

    sf::FloatRect textBounds = button.label.getLocalBounds();
    float labelWidth = textBounds.width + horizontalPadding;
    float labelHeight = textBounds.height + verticalPadding;

    // --- Auto-expand if label doesn't fit ---
    float buttonWidth = std::max(defaultWidth, labelWidth) + 40.f;
    float buttonHeight = labelHeight + 20.f; 

    // --- Shape ---
    button.shape.setSize({ buttonWidth, buttonHeight });
    button.shape.setTexture(&buttonTexture);
    button.shape.setFillColor(sf::Color::White); 
    button.shape.setPosition((width - buttonWidth) / 2.f, topY);

    // --- Label center ---
    button.label.setOrigin(textBounds.left + textBounds.width / 2.f,
        textBounds.top + textBounds.height / 2.f);
    button.label.setPosition(
        button.shape.getPosition().x + buttonWidth / 2.f,
        button.shape.getPosition().y + buttonHeight / 2.f
    );
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

void UI::drawModeSelection(sf::RenderWindow& window) {
    sf::Text title("Select Game Mode", font, 18);
    title.setFillColor(sf::Color::Black);
    centerText(title, width, height / 2.f - 80.f);
    window.draw(title);

    window.draw(normalModeButton.shape);        window.draw(normalModeButton.label);
    window.draw(fireBorderModeButton.shape);    window.draw(fireBorderModeButton.label);
    window.draw(backButton.shape);              window.draw(backButton.label);
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
    settingsTitle.setString("Field Size Settings");
    window.draw(settingsTitle);
    window.draw(res800Button.shape); window.draw(res800Button.label);
    window.draw(res1600Button.shape); window.draw(res1600Button.label);
    window.draw(fullscreenButton.shape); window.draw(fullscreenButton.label);
    window.draw(backButton.shape); window.draw(backButton.label);
}

void UI::drawSoundSettings(sf::RenderWindow& window) {
    sf::FloatRect titleBounds = settingsTitle.getLocalBounds();
    settingsTitle.setOrigin(titleBounds.left + titleBounds.width / 2.f, titleBounds.top + titleBounds.height / 2.f);
    settingsTitle.setPosition(width / 2.f, 50.f);
    settingsTitle.setString("Sound Settings");
    window.draw(settingsTitle);
    window.draw(volumeTrack);
    window.draw(volumeKnob);

    // Update label text dynamically before drawing
    volumeLabel.setString("Volume: " + std::to_string(volumeLevel) + "%");
    window.draw(volumeLabel);

    window.draw(backButton.shape);
    window.draw(backButton.label);
    window.draw(backButton.shape);
    window.draw(backButton.label);
}

void UI::applyHoverEffect(UIButton* btn, const sf::Vector2f& mousePos) {
    bool hovered = btn->shape.getGlobalBounds().contains(mousePos);

    if (btn == &yesButton) {
        btn->shape.setFillColor(hovered ? sf::Color(80, 140, 80) : sf::Color(120, 180, 120));
    }
    else if (btn == &noButton) {
        btn->shape.setFillColor(hovered ? sf::Color(140, 80, 80) : sf::Color(180, 120, 120));
    }
    else {
        btn->shape.setFillColor(
            hovered ? sf::Color(100, 100, 255, 255) : sf::Color(255, 255, 255, 200)
        );
    }
}

void UI::updateHoverEffect(const sf::Vector2f& mousePos) {
    std::vector<UIButton*> buttons = {
        &startButton, &resumeButton, &menuButton, &restartButton,
        &exitButton, &continueButton, &yesButton, &noButton,
        &settingsButton, &res800Button, &res1600Button, &fullscreenButton,
        &backButton, &resolutionButton, &soundButton, &normalModeButton, &fireBorderModeButton
    };

    for (UIButton* btn : buttons) {
        if (btn->shape.getSize().x == 0) continue; // not used
        applyHoverEffect(btn, mousePos);
    }

    // --- Pause button hover effect ---
    if (pauseCircle.getGlobalBounds().contains(mousePos)) {
        pauseCircle.setFillColor(sf::Color(160, 160, 90));  // Hover color
    }
    else {
        pauseCircle.setFillColor(sf::Color(200, 200, 120)); // Default color
    }

    // --- Volume knob hover effect ---
    if (volumeKnob.getGlobalBounds().contains(mousePos)) {
        volumeKnob.setFillColor(sf::Color(200, 200, 200)); // Hover color
    }
    else if (!draggingVolume) {
        volumeKnob.setFillColor(sf::Color(160, 160, 160)); // Default
    }
}

void UI::syncVolumeSlider(int volume) {
    volumeLevel = volume;

    float trackX = volumeTrack.getPosition().x;
    float trackW = volumeTrack.getSize().x;

    float knobX = trackX + (volume / 100.f) * trackW;
    volumeKnob.setPosition(knobX, volumeKnob.getPosition().y);

    volumeLabel.setString(std::to_string(volume) + "%");
    sf::FloatRect bounds = volumeLabel.getLocalBounds();
    volumeLabel.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    volumeLabel.setPosition(knobX, volumeTrack.getPosition().y + 30.f);  // Adjust Y as needed
}
