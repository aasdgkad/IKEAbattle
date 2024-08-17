#include "../hpp/libs.hpp"

GameOverScreen::GameOverScreen() {
    if (!font.loadFromFile("../fonts//ARIAL.TTF")) {
        // Handle font loading error
    }

    initializeText();
    initializeButton();
}

void GameOverScreen::initializeText() {
    gameOverText.setFont(font);
    gameOverText.setString("Game Over");
    gameOverText.setCharacterSize(50);
    gameOverText.setFillColor(sf::Color::White);

    returnToMenuText.setFont(font);
    returnToMenuText.setString("Return to Menu");
    returnToMenuText.setCharacterSize(30);
    returnToMenuText.setFillColor(sf::Color::Black);
}

void GameOverScreen::initializeButton() {
    returnToMenuButton.setSize(sf::Vector2f(270, 50));
    returnToMenuButton.setFillColor(sf::Color::White);
}

void GameOverScreen::draw(sf::RenderWindow& window) {
    // Center the game over text
    gameOverText.setPosition(
        window.getSize().x / 2 - gameOverText.getGlobalBounds().width / 2,
        window.getSize().y / 3 - gameOverText.getGlobalBounds().height / 2
    );

    // Position the button
    returnToMenuButton.setPosition(
        window.getSize().x / 2 - returnToMenuButton.getSize().x / 2,
        window.getSize().y * 2 / 3 - returnToMenuButton.getSize().y / 2
    );

    // Center the text on the button
    returnToMenuText.setPosition(
        returnToMenuButton.getPosition().x + (returnToMenuButton.getSize().x - returnToMenuText.getGlobalBounds().width) / 2,
        returnToMenuButton.getPosition().y + (returnToMenuButton.getSize().y - returnToMenuText.getGlobalBounds().height) / 2 - 5
    );

    window.draw(gameOverText);
    window.draw(returnToMenuButton);
    window.draw(returnToMenuText);
}

bool GameOverScreen::handleEvent(const sf::Event& event, const sf::Vector2i& mousePosition) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        if (returnToMenuButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition))) {
            return true;  // Return true to indicate that the button was clicked
        }
    }
    return false;
}