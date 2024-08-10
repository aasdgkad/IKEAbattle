#pragma once
class Animation;

class Player : public Entity {
public:
    Player(sf::Vector2f position);
    void update(float deltaTime, Map& map, const sf::Vector2u& screenres) override;
    void draw(sf::RenderWindow& window) override;

private:
    void handleInput();
    void updateAnimation();
    void manageCollisions(const std::vector<sf::FloatRect>& objectBounds);
    void checkBounds(const sf::Vector2u& screenres, Map& map);

    float gravity;
    float jumpForce;
    float moveSpeed;
    bool isGrounded;
    void loadAnimations();
};