#include "../hpp/libs.hpp"
#include "../hpp/PacMan.hpp"
#include <cmath>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
PacMan::PacMan(sf::Vector2f spawnPosition, bool &gameOver)
    : Entity(), hasAppearedOnScreen(false), lifeTimer(0.0f), lifeDuration(5.0f), speed(100.0f), degrees(0.0f), gameOver(&gameOver)
{
    setPosition(spawnPosition);
    loadSprite();
}

void PacMan::loadSprite()
{
    loadSpritesheet("../imgs/pacmann.png", 32, 32);
    addAnimation("default", 0, 3);
    setAnimation("default");
    getSprite().setOrigin(16, 16); // Set origin to center of sprite
}

void PacMan::updateDirection()
{
    // Invert the angle by subtracting from 360
    float adjustedDegrees = 360.0f - degrees;
    
    // Convert the adjusted angle to radians
    float radians = adjustedDegrees * M_PI / 180.0f;

    // Calculate velocity based on the adjusted angle
    velocity.x = std::cos(radians) * speed;
    velocity.y = std::sin(radians) * speed;
    
    // Set the sprite's rotation as usual
    float rotation = std::atan2(velocity.y, velocity.x) * 180 / M_PI;
    getSprite().setRotation(rotation);
}

void PacMan::update(float deltaTime, Map &map, const sf::Vector2u &screenres)
{
    if (!hasAppearedOnScreen)
    {
        if (isOnScreen(map.getPartBounds()))
        {
            hasAppearedOnScreen = true;
            lifeTimer = 0.0f;  // Reset the timer when it first appears
        }
        else
        {
            return; // Do nothing if not on screen yet
        }
    }

    updateDirection();
    position += velocity * deltaTime;
    setPosition(position);

    lifeTimer += deltaTime;
    if(getSprite().getGlobalBounds().intersects(*playerBounds)){
        *gameOver = true;
    }

    // Set shouldBeDead when lifetime is over
    if (lifeTimer >= lifeDuration)
    {
        shouldBeDead = true;
    }
    Animation::update(deltaTime);
}

bool PacMan::shouldRemove() { return lifeTimer >= lifeDuration; }

void PacMan::draw(sf::RenderWindow &window)
{
    window.draw(getSprite());
}

std::vector<std::pair<std::string, std::string>> PacMan::getEditableProperties() const {
    return {
        {"name", name},
        {"speed", std::to_string(speed)},
        {"degrees", std::to_string(degrees)}
    };
}

void PacMan::setProperty(const std::string& name, const std::string& value) {
    if (name == "name") {
        this->name = value;
    } else if (name == "speed") {
        this->speed = std::stof(value);
    } else if (name == "degrees") {
        this->degrees = std::stof(value);
        updateDirection();
    }
}