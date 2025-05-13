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

    void resize(float windowWidth, float windowHeight);  // NEW

    void drawMenu(sf::RenderWindow& window, bool showContinue);
    void drawPause(sf::RenderWindow& window);
    void drawGameOver(sf::RenderWindow& window, int score, int highScore);
    void drawHUD(sf::RenderWindow& window, int score, int highScore);
    void drawConfirmation(sf::RenderWindow& window);
    void drawSettingsRoot(sf::RenderWindow& window);     // for "Resolution", "Sound", "Back"
    void drawResolutionMenu(sf::RenderWindow& window);
    void drawSoundSettings(sf::RenderWindow& window);
    void drawModeSelectionMenu(sf::RenderWindow& window);
    void updateHoverEffect(const sf::Vector2f& mousePos);

    UIButton startButton;

    sf::CircleShape pauseCircle;
    sf::RectangleShape pauseBar1, pauseBar2;

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

    UIButton onlySelfCollisionButton;
    UIButton withWallCollisionButton;

private:
    sf::Font& font;
    float width, height;
    sf::Text gameOverText;
    sf::Text settingsTitle;
    sf::Text modeSelectTitle;
    sf::Texture buttonTexture;

    void centerButton(UIButton& button, const std::string& text, float topY, sf::Color color, unsigned size = 15);
};

#endif // UI_H
