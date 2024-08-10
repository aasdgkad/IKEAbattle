#include "libs.hpp"
#include <iostream>

Player::Player(sf::Vector2f position) : Entity() {
    loadAnimations();
    setPosition(position);

    gravity = 980.0f;
    jumpForce = -700.0f;
    moveSpeed = 200.0f;
    isGrounded = false;
}

void Player::loadAnimations()
{
       loadSpritesheet("./imgs/player.png", 29, 38);
       addAnimation("idle", 0, 4);
       addAnimation("running", 1, 6);
       addAnimation("jumping", 2, 3);
       setAnimation("idle");
}

void Player::handleInput()
{
       if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
       {
              velocity.x = -moveSpeed;
              isFacingRight = false;
       }
       else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
       {
              velocity.x = moveSpeed;
              isFacingRight = true;
       }
       else
       {
              velocity.x = 0;
       }

       if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && isGrounded)
       {
              velocity.y = jumpForce;
       }
}

void Player::updateAnimation()
{
       if (!isGrounded)
       {
              pause();
              setAnimation("jumping");
              if (velocity.y < 0) // Rising
              {
                     setCurrentFrame(0);
              }
              else if (std::abs(velocity.y) < 200) // Near apex
              {
                     setCurrentFrame(1);
              }
              else // Falling
              {
                     setCurrentFrame(2);
              }
       }
       else if (std::abs(velocity.x) > 0)
       {
              resume();
              setAnimation("running");
       }
       else
       {
              resume();
              setAnimation("idle");
       }
}

void Player::update(float deltaTime, Map &map, const sf::Vector2u &screenres)
{
       handleInput();
       velocity.y += gravity * deltaTime;
       position += velocity * deltaTime;
       getSprite().setPosition(position);
       manageCollisions(map.getObjectBounds());
       checkBounds(screenres, map);
       updateAnimation();
       Animation::update(deltaTime);
}

void Player::draw(sf::RenderWindow &window)
{
       window.draw(sprite);
}

void Player::manageCollisions(const std::vector<sf::FloatRect>& objectBounds) {
    sf::FloatRect playerBounds = getSprite().getGlobalBounds();
    isGrounded = false;

    for (const auto& obstacle : objectBounds) {
        CollisionInfo collision = checkCollision(playerBounds, {obstacle});
        if (collision.collided) {
            switch (collision.side) {
                case CollisionSide::Left:
                    position.x = obstacle.left - playerBounds.width;
                    velocity.x = 0;
                    break;
                case CollisionSide::Right:
                    position.x = obstacle.left + obstacle.width;
                    velocity.x = 0;
                    break;
                case CollisionSide::Top:
                    position.y = obstacle.top - playerBounds.height;
                    velocity.y = 0;
                    isGrounded = true;
                    break;
                case CollisionSide::Bottom:
                    position.y = obstacle.top + obstacle.height;
                    velocity.y = 0;
                    break;
                default:
                    break;
            }
            setPosition(position);
        }
    }
}
void Player::checkBounds(const sf::Vector2u &screenres, Map &map)
{
       if (this->position.x > screenres.x)
       {
              this->position.x = 0;
              map.changePart(1, 0);
       }
       else if (this->position.x < 0)
       {
              this->position.x = screenres.x - this->sprite.getGlobalBounds().width;
              map.changePart(-1, 0);
       }
       else if (this->position.y < 0)
       {
              this->position.y = screenres.y - this->sprite.getGlobalBounds().height;
              map.changePart(0, -1);
       }
       else if (this->position.y > screenres.y)
       {
              this->position.y = 0;
              map.changePart(0, 1);
       }
}
// what am i doing idk i seem to be too incapable to code which is paradoxically is the only thing i know how to do (or not this is subjective) well that is unfortunate