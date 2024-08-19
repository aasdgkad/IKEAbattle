#include "../hpp/libs.hpp"

PacMan::PacMan(sf::Vector2f spawnPosition)
    : Entity(), hasAppearedOnScreen(false), lifeTimer(0.0f), lifeDuration(3.0f), speed(200.0f)
{
    setPosition(spawnPosition);
    loadSprite();
}

PacMan::~PacMan()
{
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
    if(hasAppearedOnScreen){
        lifeTimer += deltaTime;
    }
    if (lifeTimer >= lifeDuration)
    {
        
    }

    Animation::update(deltaTime);
}
bool PacMan::shouldRemove() { return lifeTimer >= lifeDuration; }
void PacMan::draw(sf::RenderWindow& window)
{
    window.draw(getSprite());
}