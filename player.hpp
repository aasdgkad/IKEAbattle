// Player.hpp
#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
class Map;
class Player
{
public:
    Player(sf::Vector2f position);
    void update(float deltaTime, Map &map, const sf::Vector2u& screenres);
    void draw(sf::RenderWindow &window);
    void handleInput();
    void updateState();

private:
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Vector2f velocity;
    sf::Vector2f position;

    std::vector<sf::IntRect> runningFrames;
    std::vector<sf::IntRect> idleFrames;
    std::vector<sf::IntRect> jumpingFrames;

    enum class State
    {
        Idle,
        Running,
        Jumping
    };
    State currentState;

    float gravity;
    float jumpForce;
    float moveSpeed;
    bool isGrounded;
    bool isFacingRight;
    int currentFrame;
    float frameTime;
    float frameInterval;
    void checkCollisions(const std::vector<sf::FloatRect> &objectBounds);
    void checkBounds(const sf::Vector2u& screenres, Map& map);
    void updateAnimation(float deltaTime);
    void loadTextures();
};