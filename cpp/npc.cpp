#include "../hpp/libs.hpp"
class TextBox; 
Npc::Npc(sf::Vector2f position)
    : Entity(), gravity(980.0f), isColliding(false)
{
       textBox = std::make_unique<TextBox>("", 0.007f);
       setPosition(position);
       loadSprite();
}

void Npc::loadSprite()
{
       loadSpritesheet("../imgs/capybara.png", 64, 64); // Adjust path and dimensions as needed
       addAnimation("idle", 1, 8);                      // Assuming the idle animation has 4 frames
       setAnimation("idle");
}

void Npc::update(float deltaTime, Map &map, const sf::Vector2u &screenres, sf::FloatRect playerBounds)
{
       if (isOnScreen(map.getPartBounds()))
       {
              velocity.y += gravity * deltaTime;
              position.y += velocity.y * deltaTime;
              getSprite().setPosition(position);
              manageCollisions(map.getObjectBounds());
              checkCollisionWithPlayer(playerBounds);
              Animation::update(deltaTime);

              if (textBox)
              {
                     textBox->update(deltaTime);
              }
       }
}

void Npc::draw(sf::RenderWindow &window)
{
       window.draw(getSprite());
       textBox->draw(window);
}

void Npc::checkCollisionWithPlayer(const sf::FloatRect &playerBounds)
{
       sf::FloatRect npcBounds = getSprite().getGlobalBounds();
       if (npcBounds.intersects(playerBounds))
       {
              if (!isColliding)
              {
                     if (textBox)
                     {
                            textBox->setString("Marijuana Cocaina \nHeroina Crack");
                     }
                     isColliding = true;
              }
       }
       else
       {
              if (isColliding)
              {
                     textBox->setString("");
              }
              isColliding = false;
       }
}

void Npc::manageCollisions(const std::vector<sf::FloatRect> &objectBounds)
{
       sf::FloatRect npcBounds = getSprite().getGlobalBounds();
       for (const auto &obstacle : objectBounds)
       {
              CollisionInfo collision = checkCollision(npcBounds, {obstacle});
              if (collision.collided)
              {
                     switch (collision.side)
                     {
                     case CollisionSide::Bottom:
                            position.y = obstacle.top - npcBounds.height;
                            velocity.y = 0;
                            break;
                     case CollisionSide::Top:
                            position.y = obstacle.top + obstacle.height;
                            velocity.y = 0;
                            break;
                     default:
                            break;
                     }
                     setPosition(position);
                     getSprite().setPosition(position);
              }
       }
}