#include "../hpp/libs.hpp"

PacMan::PacMan(sf::Vector2f spawnPosition,bool &gameOver)
    : Entity(), hasAppearedOnScreen(false), lifeTimer(0.0f), lifeDuration(5.0f), speed(0.0f),gameOver(&gameOver)
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

    // Move right
    velocity.x = -speed;
    position += velocity * deltaTime;
    getSprite().setPosition(position);

    lifeTimer += deltaTime;
    if(getSprite().getGlobalBounds().intersects(*playerBounds)){
        *gameOver=true;
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
            {"speed", std::to_string(speed)}
        };
    }

    void PacMan::setProperty(const std::string& name, const std::string& value) {
        if (name == "name") {
            this->name = value;
        } else if (name == "speed") {
            this->speed = std::stof(value);
        }
    }