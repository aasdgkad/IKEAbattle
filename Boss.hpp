// Boss.hpp
#pragma once
#include <SFML/Graphics.hpp>

class Boss {
public:
    Boss(const std::string& imagePath, sf::Vector2u windowSize);
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);

private:
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Vector2f velocity;
    float bounceSpeed;
    float targetWidth;
    sf::Vector2u windowSize;

    void loadAndScaleImage(const std::string& imagePath);
    void setInitialPosition();
};