#ifndef UI_H
#define UI_H

#include <SFML/Graphics.hpp>
#include <vector>

struct UIButton {
    sf::RectangleShape shape;
    sf::Text label;
};

class UI {
public:
    UI(sf::Font& font, float windowWidth, float windowHeight);

    void resize(float windowWidth, float windowHeight);  

    // --- Adjust button layout ---
    void layoutMenuButtons(float startY, float buttonHeight, float spacing);
    void layoutModeButtons(float startY, float spacing);
    void layoutPauseMenu(float startY, float buttonHeight, float spacing);
    void layoutSettingsRoot(float startY, float buttonHeight, float spacing);
    void layoutResolutionOptions(float startY, float buttonHeight, float spacing);
    void layoutConfirmationBox(float buttonHeight);
    void layoutVolumeControls(float sliderWidth);
    void layoutPauseIcon();
    void layoutSpeedSettings(float startY, float buttonHeight, float spacing);

    // --- Draw UI ---
    void drawMenu(sf::RenderWindow& window, bool showContinue);
    void drawModeSelection(sf::RenderWindow& window);
    void drawPause(sf::RenderWindow& window);
    void drawGameOver(sf::RenderWindow& window, int score, int highScore);
    void drawHUD(sf::RenderWindow& window, int score, int highScore);
    void drawConfirmation(sf::RenderWindow& window);
    void drawSettingsRoot(sf::RenderWindow& window);     
    void drawResolutionMenu(sf::RenderWindow& window);
    void drawSoundSettings(sf::RenderWindow& window);
    void drawSpeedSettings(sf::RenderWindow& window);

    // --- Hover effect ---
    void applyHoverEffect(UIButton* btn, const sf::Vector2f& mousePos);
    void updateHoverEffect(const sf::Vector2f& mousePos);

    sf::CircleShape pauseCircle;
    sf::RectangleShape pauseBar1, pauseBar2;

    UIButton startButton;
    UIButton resumeButton;
    UIButton continueButton;
    UIButton menuButton;
    UIButton restartButton;
    UIButton exitButton;

    sf::RectangleShape confirmBox;
    sf::Text confirmText;
    UIButton yesButton;
    UIButton noButton;

    UIButton settingsButton;
    UIButton res800Button;
    UIButton res1600Button;
    UIButton fullscreenButton;
    UIButton backButton;

    UIButton resolutionButton;
    UIButton soundButton;

    UIButton normalModeButton;
    UIButton fireBorderModeButton;

    UIButton speedButton;
    UIButton speedSlowButton;
    UIButton speedNormalButton;
    UIButton speedFastButton;

    sf::RectangleShape volumeTrack;
    sf::CircleShape volumeKnob;
    int volumeLevel = 50; 
    bool draggingVolume = false;
    sf::Text volumeLabel;
    void syncVolumeSlider(int volume);

private:
    sf::Font& font;
    float width, height;
    sf::Text gameOverText;
    sf::Text settingsTitle;

    sf::Texture buttonTexture;

    void centerButton(UIButton& button, const std::string& text, float topY, sf::Color color, unsigned size = 15);
};

#endif
