#include "libs.hpp"
#include <iostream>

Player::Player(sf::Vector2f position) : position(position), currentState(State::Idle), currentFrame(0), frameTime(0), frameInterval(0.1f), isFacingRight(true)
{
       loadTextures();
       sprite.setTexture(texture);
       sprite.setTextureRect(idleFrames[0]);
       sprite.setPosition(position);

       gravity = 980.0f;
       jumpForce = -500.0f;
       moveSpeed = 200.0f;
       velocity = sf::Vector2f(0, 0);
}

void Player::loadTextures()
{
       texture.loadFromFile("./imgs/player.png");

       // Set up running frames
       for (int i = 0; i < 4; ++i)
       {
              idleFrames.push_back(sf::IntRect(i * 64, 0, 64, 64));
       }

       // Set up idle frames
       for (int i = 0; i < 6; ++i)
       {
              runningFrames.push_back(sf::IntRect(i * 64, 64, 64, 64));
       }

       // Set up jumping frames
       for (int i = 0; i < 3; ++i)
       {
              jumpingFrames.push_back(sf::IntRect(i * 64, 128, 64, 64));
       }
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

void Player::updateState()
{
       if (!isGrounded)
       {
              currentState = State::Jumping;
       }
       else if (std::abs(velocity.x) > 0)
       {
              currentState = State::Running;
       }
       else
       {
              currentState = State::Idle;
       }
}

void Player::update(float deltaTime, Map &map, const sf::Vector2u& screenres)
{
       handleInput();

       // Apply gravity
       velocity.y += gravity * deltaTime;

       // Update position
       position += velocity * deltaTime;
       sprite.setPosition(position);
       // Check collisions with map objects
       checkCollisions(map.getObjectBounds());

       //Check for part changes
       checkBounds(screenres, map);    

       updateState();

       sprite.setPosition(position);
       updateAnimation(deltaTime);
}
void Player::updateAnimation(float deltaTime)
{
       frameTime += deltaTime;
       if (frameTime >= frameInterval)
       {
              frameTime = 0;

              std::vector<sf::IntRect> *currentAnimation;
              switch (currentState)
              {
              case State::Running:
                     currentAnimation = &runningFrames;
                     currentFrame++;
                     if (currentFrame >= currentAnimation->size())
                     {
                            currentFrame = 0;
                     }
                     break;
              case State::Jumping:
                     currentAnimation = &jumpingFrames;
                     // Jumping animation logic
                     if (velocity.y < 0) // Rising
                     {
                            currentFrame = 0;
                     }
                     else if (velocity.y >= -200 && velocity.y < 200) // Near apex
                     {
                            currentFrame = 1;
                     }
                     else // Falling
                     {
                            currentFrame = 2;
                     }
                     break;
              default:
                     currentAnimation = &idleFrames;
                     currentFrame++;
                     if (currentFrame >= currentAnimation->size())
                     {
                            currentFrame = 0;
                     }
              }

              sf::IntRect currentRect = (*currentAnimation)[currentFrame];
              if (!isFacingRight)
              {
                     currentRect.left = currentRect.left + currentRect.width;
                     currentRect.width = -currentRect.width;
              }
              sprite.setTextureRect(currentRect);
       }
}

void Player::draw(sf::RenderWindow &window)
{
       window.draw(sprite);
}

void Player::checkCollisions(const std::vector<sf::FloatRect> &objectBounds)
{
       sf::FloatRect playerBounds = sprite.getGlobalBounds();
       isGrounded = false;
       for (const auto &objBounds : objectBounds)
       {
              sf::FloatRect intersection;
              if (playerBounds.intersects(objBounds, intersection))
              {
                     // Determine collision side
                     if (intersection.width < intersection.height)
                     {
                            // Horizontal collision
                            if (playerBounds.left < objBounds.left)
                            {
                                   position.x = objBounds.left - playerBounds.width;
                            }
                            else
                            {
                                   position.x = objBounds.left + objBounds.width;
                            }
                            velocity.x = 0;
                     }
                     else
                     {
                            // Vertical collision
                            if (playerBounds.top < objBounds.top)
                            {
                                   if(velocity.y>0){
                                          position.y = objBounds.top - playerBounds.height;
                                   velocity.y = 0;
                                   isGrounded = true;
                                   }
                            }
                            else
                            {
                                   position.y = objBounds.top + objBounds.height;
                                   velocity.y /= -2;
                            }
                     }
                     sprite.setPosition(position);
                     playerBounds = sprite.getGlobalBounds(); // Update bounds for next iteration
              }
       }
}

void Player::checkBounds(const sf::Vector2u& screenres, Map& map){
    if(this->position.x > screenres.x){
        this->position.x = 0;
        map.changePart(1,0);
    }
    else if(this->position.x < 0){
        this->position.x = screenres.x - this->sprite.getGlobalBounds().width;
        map.changePart(-1,0);
    }
    else if(this->position.y < 0){
        this->position.y = screenres.y - this->sprite.getGlobalBounds().height;
        map.changePart(0,-1);
    }
    else if(this->position.y > screenres.y){
        this->position.y = 0;
        map.changePart(0,1);
    }
}
//what am i doing idk i seem to be too incapable to code which is paradoxically is the only thing i know how to do (or not this is subjective) well that is unfortunate