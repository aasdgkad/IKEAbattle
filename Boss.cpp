// Boss.cpp
#include "Boss.hpp"
#include <iostream>

Boss::Boss(const std::string& imagePath, sf::Vector2u windowSize)
    : targetWidth(400.0f), windowSize(windowSize), bounceSpeed(200.0f)
{
    loadAndScaleImage(imagePath);
    setInitialPosition();
    velocity = sf::Vector2f(bounceSpeed, 0);
}

void Boss::loadAndScaleImage(const std::string& imagePath)
{
    if (!texture.loadFromFile(imagePath)) {
        std::cerr << "Failed to load image: " << imagePath << std::endl;
        return;
    }

    sprite.setTexture(texture);

    // Calculate the scaling factor to achieve the target width
    float scaleFactor = targetWidth / sprite.getLocalBounds().width;
    sprite.setScale(scaleFactor, scaleFactor);
}

void Boss::setInitialPosition()
{
    // Position the sprite a little above the middle of the screen
    float x = (windowSize.x - sprite.getGlobalBounds().width) / 2.0f;
    float y = (windowSize.y - sprite.getGlobalBounds().height) / 2.0f - 150.0f;
    sprite.setPosition(x, y);
}

void Boss::update(float deltaTime)
{
    sf::Vector2f position = sprite.getPosition();

    // Update position
    position += velocity * deltaTime;

    // Bounce off the left and right edges of the window
    if (position.x <= 0 || position.x + sprite.getGlobalBounds().width >= windowSize.x) {
        velocity.x = -velocity.x;
    }

    sprite.setPosition(position);
}

void Boss::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}