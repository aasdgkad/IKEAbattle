#pragma once

class TextBox {
public:
    TextBox(const std::string& text, float letterDelay = 0.05f);
    void setString(const std::string& text);
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    void setCharacterImage(const std::string& imagePath);

private:
    sf::RectangleShape background;
    std::string textContent;
    sf::Text visibleContent;
    sf::Font font;
    float letterDelay;
    float elapsedTime;
    int currentLetter;
    bool visible;

    sf::Texture characterTexture;
    sf::Sprite characterSprite;
    bool hasCharacterImage;
};