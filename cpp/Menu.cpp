#include "../hpp/libs.hpp"

Menu::Menu(sf::RenderWindow &window) : windowr(window)
{
    if (!font.loadFromFile("../fonts/ARIAL.TTF"))
    {
        // Handle font loading error
    }

    playButton.setSize(sf::Vector2f(200, 50));
    playButton.setFillColor(sf::Color::Yellow);
    playButton.setPosition((windowr.getSize().x - playButton.getSize().x) / 2,
                           (windowr.getSize().y - playButton.getSize().y) / 2);

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

void Menu::draw()
{
    // Save the current view
    sf::View originalView = windowr.getView();

    // Reset the view to the default (window coordinates)
    windowr.setView(windowr.getDefaultView());
    windowr.draw(playButton);
    windowr.draw(playText);
    // Restore the original view
    windowr.setView(originalView);
}

bool Menu::isPlayButtonClicked()
{
    return playButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(windowr)));
}
void Menu::updateButtonColor()
{
    if (playButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(windowr))))
    {
        playButton.setFillColor(sf::Color::Red);
    }
    else
    {
        playButton.setFillColor(sf::Color::Yellow);
    }
}