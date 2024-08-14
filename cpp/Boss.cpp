#include "../hpp/libs.hpp"
#include <iostream>

Boss::Boss(const std::string &imagePath, sf::Vector2u windowSize)
    : targetWidth(200.0f), windowSize(windowSize), bounceSpeed(200.0f)
{
    sprite.setOrigin(514,366);
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
    sprite.setPosition(x, y);
    eyesprite.setPosition(x-eyesprite.getGlobalBounds().width/2.0f, y-eyesprite.getGlobalBounds().height/2.0f);
}

void Boss::update(float deltaTime, Map &map, const sf::Vector2u &screenres, sf::FloatRect player)
{

    sf::Vector2f ptb = sprite.getPosition() - player.getPosition();
    // 514, 366
    if (((player.left < sprite.getPosition().x) && (sprite.getScale().x > 0)) || ((player.left > sprite.getPosition().x) && (sprite.getScale().x < 0)))
    {
        sprite.scale(-1, 1);
    }
    if (sprite.getScale().x > 0)
        sprite.setRotation(atan(ptb.y / ptb.x) * 180 / 3.14159);
    else
        sprite.setRotation(atan(ptb.y / ptb.x) * 180 / 3.14159);

    // Bounce off the left and right edges of the window
    if (position.x <= 0 || position.x + sprite.getGlobalBounds().width >= windowSize.x)
    {
        velocity.x = -velocity.x;
    }

    // Create Attacks
    sf::Vector2f vdis = player.getPosition() + player.getSize() / 2.0f - this->sprite.getPosition();
    float distance = sqrt(vdis.x * vdis.x + vdis.y * vdis.y);

    if (ptimer.getElapsedTime().asSeconds() >= 3.5)
    {
        this->attacks.push_back(new Plank(this->sprite.getPosition()));
        ptimer.restart();
    }

    if (ltimer.getElapsedTime().asSeconds() >= 0.8)
    {
        this->attacks.push_back(new LaserBeam(this->sprite.getPosition(), this->sprite.getRotation()));
        ltimer.restart();
    }

    if(ttimer.getElapsedTime().asSeconds() >= 2.3){
        //int randx = rand()%int(3*player.width) + player.left - player.width;
        this->attacks.push_back(new TableFall(sf::Vector2f(player.left+player.width/2.0f,map.getPartBounds().top)));
        ttimer.restart();
    }

    // Manage attacks behavior
    for (int i = 0; i < this->attacks.size(); i++)
    {
        if (this->attacks[i]->update(player))
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

void Boss::resetTimers(){
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