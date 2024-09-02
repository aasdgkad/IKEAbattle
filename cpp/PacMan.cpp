#include "../hpp/libs.hpp"

PacMan::PacMan(sf::Vector2f spawnPosition,bool &gameOver)
    : Entity(), hasAppearedOnScreen(false), lifeTimer(0.0f), lifeDuration(3.0f), speed(200.0f),gameOver(&gameOver)
{
    setPosition(spawnPosition);
    loadSprite();
}

void PacMan::loadSprite()
{
    loadSpritesheet("../imgs/pacman.png", 64, 64);
    addAnimation("default", 0, 1);
    setAnimation("default");
}

void PacMan::update(float deltaTime, Map &map, const sf::Vector2u &screenres, sf::FloatRect playerBounds)
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

    // Move right
    velocity.x = speed;
    position += velocity * deltaTime;
    getSprite().setPosition(position);

    lifeTimer += deltaTime;

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