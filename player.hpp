// Player.hpp
#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
class Map;
class Player {
public:
    Player(sf::Vector2f position);
    void update(float deltaTime, const Map& map);
    void draw(sf::RenderWindow& window);
    void handleInput();
void updateState();
    sf::FloatRect getBounds() const;
    void handleCollision(const sf::FloatRect& objectBounds);

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
void checkCollisions(const std::vector<sf::FloatRect>& objectBounds);
    void updateAnimation(float deltaTime);
    void loadTextures();
};