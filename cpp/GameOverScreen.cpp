#include "../hpp/libs.hpp"

GameOverScreen::GameOverScreen(sf::RenderWindow & window) : windowr(window) {
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

void GameOverScreen::draw(){
        // Save the current view
        sf::View originalView = windowr.getView();

        // Reset the view to the default (window coordinates)
        windowr.setView(windowr.getDefaultView());

        // Center the game over text
        gameOverText.setPosition(
            windowr.getSize().x / 2 - gameOverText.getGlobalBounds().width / 2,
            windowr.getSize().y / 3 - gameOverText.getGlobalBounds().height / 2
        );

        // Position the button
        returnToMenuButton.setPosition(
            windowr.getSize().x / 2 - returnToMenuButton.getSize().x / 2,
            windowr.getSize().y * 2 / 3 - returnToMenuButton.getSize().y / 2
        );

        // Center the text on the button
        returnToMenuText.setPosition(
            returnToMenuButton.getPosition().x + (returnToMenuButton.getSize().x - returnToMenuText.getGlobalBounds().width) / 2,
            returnToMenuButton.getPosition().y + (returnToMenuButton.getSize().y - returnToMenuText.getGlobalBounds().height) / 2 - 5
        );

        windowr.draw(gameOverText);
        windowr.draw(returnToMenuButton);
        windowr.draw(returnToMenuText);

        // Restore the original view
        windowr.setView(originalView);
    }

bool GameOverScreen::handleEvent(const sf::Event& event) {
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            // Convert mouse position to window coordinates
            sf::Vector2i mousePosition = sf::Mouse::getPosition(windowr);
            sf::Vector2f worldPos = windowr.mapPixelToCoords(mousePosition);

            if (returnToMenuButton.getGlobalBounds().contains(mousePosition.x,mousePosition.y)) {
                return true;  // Return true to indicate that the button was clicked
            }
        }
        return false;
    }