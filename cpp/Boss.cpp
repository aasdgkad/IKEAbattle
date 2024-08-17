#include "../hpp/libs.hpp"

Boss::Boss(const std::string &imagePath, sf::Vector2u windowSize)
    : targetWidth(200.0f), windowSize(windowSize), bounceSpeed(200.0f),
      isMovingToTarget(false), spinSpeed(0), moveSpeed(0),gameOver(false)
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
if (isMovingToTarget)
    {
        sf::Vector2f currentPosition = sprite.getPosition();
        sf::Vector2f direction = targetLocation - currentPosition;
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        if (distance > moveSpeed * deltaTime)
        {
            // Normalize the direction vector
            direction /= distance;
            
            // Move towards the target
            sprite.move(direction * moveSpeed * deltaTime);
            
            // Spin the boss
            sprite.rotate(spinSpeed * deltaTime);
        }
        else
        {
            // Boss has reached the target location
            sprite.setPosition(targetLocation);
            isMovingToTarget = false;
            sprite.setRotation(0); // Reset rotation if needed
        }

        // Update eye sprite position
        eyesprite.setPosition(sprite.getPosition().x - eyesprite.getGlobalBounds().width/2.0f, 
                              sprite.getPosition().y - eyesprite.getGlobalBounds().height/2.0f);
    }
    else{
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

    // Create Attacks
    sf::Vector2f vdis = player.getPosition() + player.getSize() / 2.0f - this->sprite.getPosition();
    float distance = sqrt(vdis.x * vdis.x + vdis.y * vdis.y);

    if (ptimer.getElapsedTime().asSeconds() >= 3.5)
    {
        this->attacks.push_back(new Plank(this->sprite.getPosition(),gameOver));
        ptimer.restart();
    }

    if (ltimer.getElapsedTime().asSeconds() >= 0.8)
    {
        this->attacks.push_back(new LaserBeam(this->sprite.getPosition(), this->sprite.getRotation(),gameOver));
        ltimer.restart();
    }

    if(ttimer.getElapsedTime().asSeconds() >= 2.3){
        //int randx = rand()%int(3*player.width) + player.left - player.width;
        this->attacks.push_back(new TableFall(sf::Vector2f(player.left+player.width/2.0f,map.getPartBounds().top),gameOver));
        ttimer.restart();
    }
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

void Boss::moveToLocationWithSpin(const sf::Vector2f& targetLocation, float spinSpeed, float moveSpeed)
{
    this->targetLocation = targetLocation;
    this->isMovingToTarget = true;
    this->spinSpeed = spinSpeed;
    this->moveSpeed = moveSpeed;
}

void Boss::moveRandomlyOnXAxis(float minX, float maxX)
{
    float currentY = sprite.getPosition().y;
    float newX = minX + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (maxX - minX)));
    moveToLocationWithSpin(sf::Vector2f(newX, currentY), 1700.0f, 200.0f);
}

Boss::~Boss()
{
    for (int i = 0; i < this->attacks.size(); i++)
    {
        delete attacks[i];
    }
}