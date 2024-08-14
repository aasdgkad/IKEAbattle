#include "../hpp/libs.hpp"

Menu::Menu(const sf::Vector2u& windowSize) {
    if (!font.loadFromFile("../fonts/ARIAL.TTF")) {
        // Handle font loading error
    }

    playButton.setSize(sf::Vector2f(200, 50));
    playButton.setFillColor(sf::Color::Yellow);
    playButton.setPosition((windowSize.x - playButton.getSize().x) / 2, 
                           (windowSize.y - playButton.getSize().y) / 2);

    playText.setFont(font);
    playText.setString("Play");
    playText.setCharacterSize(24);
    playText.setFillColor(sf::Color::Black);
    sf::FloatRect textRect = playText.getLocalBounds();
    playText.setOrigin(textRect.left + textRect.width / 2.0f,
                       textRect.top + textRect.height / 2.0f);
    playText.setPosition(playButton.getPosition() + 
                         sf::Vector2f(playButton.getSize().x / 2, playButton.getSize().y / 2));
}

void Menu::draw(sf::RenderWindow& window) {
    window.draw(playButton);
    window.draw(playText);
}

bool Menu::isPlayButtonClicked(const sf::Vector2i& mousePosition) {
    return playButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition));
}
void Menu::updateButtonColor(const sf::Vector2i& mousePosition) {
    if (playButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition))) {
        playButton.setFillColor(sf::Color::Red);
    } else {
        playButton.setFillColor(sf::Color::Yellow);
    }
}