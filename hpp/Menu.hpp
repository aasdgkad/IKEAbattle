#pragma once
class Menu {
public:
    Menu(sf::RenderWindow &window);
    void draw();
    bool isPlayButtonClicked();
    void updateButtonColor();

private:
    sf::RenderWindow &windowr;
    sf::RectangleShape playButton;
    sf::Text playText;
    sf::Font font;
};