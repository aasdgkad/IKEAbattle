#pragma once

#include <SFML/Graphics.hpp>

class GameOverScreen {
public:
    GameOverScreen();
    void draw(sf::RenderWindow& window);
    bool handleEvent(const sf::Event& event, const sf::Vector2i& mousePosition);
    bool gameOver;

private:
    sf::Font font;
    sf::Text gameOverText;
    sf::Text returnToMenuText;
    sf::RectangleShape returnToMenuButton;

    void initializeText();
    void initializeButton();
};