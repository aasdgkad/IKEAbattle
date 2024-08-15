class Menu {
public:
    Menu(const sf::Vector2u& windowSize);
    void draw(sf::RenderWindow& window);
    bool isPlayButtonClicked(const sf::Vector2i& mousePosition);
    void updateButtonColor(const sf::Vector2i& mousePosition);

private:
    sf::RectangleShape playButton;
    sf::Text playText;
    sf::Font font;
};