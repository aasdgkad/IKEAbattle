#pragma once

class GameOverScreen {
public:
    GameOverScreen(sf::RenderWindow &window);
    void draw();
    bool handleEvent(const sf::Event& event);
    bool gameOver;

private:
    sf::RenderWindow &windowr;
    sf::Font font;
    sf::Text gameOverText;
    sf::Text returnToMenuText;
    sf::RectangleShape returnToMenuButton;

    void initializeText();
    void initializeButton();
};