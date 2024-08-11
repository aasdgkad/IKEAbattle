#include "libs.hpp"
#include <iostream>

Boss::Boss(const std::string& imagePath, sf::Vector2u windowSize)
    : targetWidth(400.0f), windowSize(windowSize), bounceSpeed(200.0f), spawnTimer(0.0f)
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

void Boss::update(float deltaTime, Map& map)
{
    sf::Vector2f position = sprite.getPosition();

    // Update position
    position += velocity * deltaTime;

    // Bounce off the left and right edges of the window
    if (position.x <= 0 || position.x + sprite.getGlobalBounds().width >= windowSize.x) {
        velocity.x = -velocity.x;
    }

    sprite.setPosition(position);

    // Spawn Idk objects
    spawnTimer += deltaTime;
    if (spawnTimer >= 2.0f) {  // Spawn every 2 seconds
        spawnIdk();
        spawnTimer = 0.0f;  // Reset the timer
    }

    // Update all Idk objects
    for (auto it = idkObjects.begin(); it != idkObjects.end();) {
        (*it)->update(deltaTime, map, windowSize);
        if ((*it)->isOutOfBounds(windowSize)) {
            it = idkObjects.erase(it);
        } else {
            ++it;
        }
    }
}

void Boss::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
    
    // Draw all Idk objects
    for (const auto& idk : idkObjects) {
        idk->draw(window);
    }
}

void Boss::spawnIdk()
{
    sf::Vector2f spawnPosition = sprite.getPosition();
    spawnPosition.y += sprite.getGlobalBounds().height;  // Spawn below the boss
    float moveDistance = 200.0f;
    idkObjects.push_back(std::make_unique<Idk>(spawnPosition, moveDistance));
}