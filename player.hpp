// Player.hpp
#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Player {
public:
    Player(sf::Vector2f position);
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    void handleInput();

private:
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Vector2f velocity;
    sf::Vector2f position;

    std::vector<sf::IntRect> runningFrames;
    std::vector<sf::IntRect> idleFrames;
    std::vector<sf::IntRect> jumpingFrames;

    enum class State { Idle, Running, Jumping };
    State currentState;

    float gravity;
    float jumpForce;
    float moveSpeed;
    bool isGrounded;
    bool isFacingRight;
    int currentFrame;
    float frameTime;
    float frameInterval;

    void updateAnimation(float deltaTime);
    void loadTextures();
};