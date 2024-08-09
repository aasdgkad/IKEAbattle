// Player.cpp
#include "Player.hpp"

Player::Player(sf::Vector2f position) : position(position), currentState(State::Idle), currentFrame(0), frameTime(0), frameInterval(0.1f), isFacingRight(true)
{
       loadTextures();
       sprite.setTexture(texture);
       sprite.setTextureRect(idleFrames[0]);
       sprite.setPosition(position);

       gravity = 980.0f;
       jumpForce = -400.0f;
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

void Player::update(float deltaTime)
{
       handleInput();

       // Apply gravity
       velocity.y += gravity * deltaTime;

       // Update position
       position += velocity * deltaTime;

       // Simple collision with window bottom
       if (position.y > 768 - 64)
       { // Assuming window height is 768
              position.y = 768 - 64;
              velocity.y = 0;
              isGrounded = true;
              if (currentState == State::Jumping)
              {
                     currentState = State::Idle;
              }
       }
       else
       {
              isGrounded = false;
       }

       sprite.setPosition(position);
       updateAnimation(deltaTime);
}

void Player::handleInput()
{
       if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
       {
              velocity.x = -moveSpeed;
              isFacingRight = false;
              if (isGrounded)
              {
                     currentState = State::Running;
              }
       }
       else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
       {
              velocity.x = moveSpeed;
              isFacingRight = true;
              if (isGrounded)
              {
                     currentState = State::Running;
              }
       }
       else
       {
              velocity.x = 0;
              if (currentState != State::Jumping)
              {
                     currentState = State::Idle;
              }
       }

       if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && isGrounded)
       {
              velocity.y = jumpForce;
              currentState = State::Jumping;
              isGrounded = false;
       }
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