#include "../hpp/libs.hpp"

Boss::Boss(const std::string &imagePath, sf::Vector2u windowSize,bool &gameOverr)
    : targetWidth(200.0f), windowSize(windowSize),gameover(&gameOverr)
{
    sprite.setOrigin(514, 366);
    loadAndScaleImage(imagePath);
    setInitialPosition();
    atimer.restart();
    ptimer.restart();
    ltimer.restart();
}

void Boss::loadAndScaleImage(const std::string &imagePath)
{
    assert(texture.loadFromFile(imagePath));
    assert(eyetexture.loadFromFile("../imgs/redeye.png"));

    eyesprite.setTexture(eyetexture);
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
    position.x = x;
    position.y = y;
    sprite.setPosition(position);
    eyesprite.setPosition(x - eyesprite.getGlobalBounds().width / 2.0f, y - eyesprite.getGlobalBounds().height / 2.0f);
}

void Boss::update(float deltaTime, Map &map, const sf::Vector2u& screenres)
{
    // Calculate direction to player
    sf::Vector2f bossPosition = sprite.getPosition();
    sf::Vector2f playerCenter(playerBounds->left + playerBounds->width / 2.0f, playerBounds->top + playerBounds->height / 2.0f);
    sf::Vector2f direction = playerCenter - bossPosition;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    // Normalize direction
    if (distance > 0)
    {
        direction /= distance;
    }

    // Set movement speed (you can adjust this value)
    float movementSpeed = 100.0f; // pixels per second

    // Move towards player
    sprite.move(direction * movementSpeed * deltaTime);
    position = sprite.getPosition();

    // Update eye sprite position
    eyesprite.setPosition(sprite.getPosition().x - eyesprite.getGlobalBounds().width / 2.0f,
                          sprite.getPosition().y - eyesprite.getGlobalBounds().height / 2.0f);

    // Face the player
    if (playerCenter.x < bossPosition.x && sprite.getScale().y > 0)
    {
        sprite.scale(1, -1);
    }
    else if (playerCenter.x > bossPosition.x && sprite.getScale().y < 0)
    {
        sprite.scale(1, -1);
    }

    // Rotate to face player
    float angle = std::atan2(direction.y, direction.x) * 180 / 3.14159;
    sprite.setRotation(angle);

    // Create Attacks (keeping the existing attack logic)
    if (isOnScreen((map.getPartBounds())))
    {
        if (ptimer.getElapsedTime().asSeconds() >= 3.5)
        {
            this->attacks.push_back(new Plank(this->sprite.getPosition(), *gameover));
            ptimer.restart();
        }

        if (ltimer.getElapsedTime().asSeconds() >= 0.8)
        {
            this->attacks.push_back(new LaserBeam(this->sprite.getPosition(), this->sprite.getRotation(), *gameover));
            ltimer.restart();
        }

        if (ttimer.getElapsedTime().asSeconds() >= 2.3)
        {
            this->attacks.push_back(new TableFall(sf::Vector2f(playerBounds->left + playerBounds->width / 2.0f, map.getPartBounds().top), *gameover));
            ttimer.restart();
        }
    }
    // Manage attacks behavior
    for (int i = 0; i < this->attacks.size(); i++)
    {
        if (this->attacks[i]->update(*playerBounds))
        {
            attacks.erase(attacks.begin() + i);
            i--;
        }
    }
}
void Boss::draw(sf::RenderWindow &window)
{
    window.draw(sprite);

    // Draw all attacks
    for (const auto &at : attacks)
    {
        at->draw(window);
    }
    // Draw the red eye
    window.draw(this->eyesprite);
}

void Boss::resetTimers()
{
    this->ttimer.restart();
    this->ltimer.restart();
    this->atimer.restart();
    this->ptimer.restart();
}

Boss::~Boss()
{
    for (int i = 0; i < this->attacks.size(); i++)
    {
        delete attacks[i];
    }
}