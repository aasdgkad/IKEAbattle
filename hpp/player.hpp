#pragma once
class Animation;
class Item;

class Player : public Entity {
    friend class Item;
public:
    Player(sf::Vector2f position,bool&gameover);
    void update(float deltaTime, Map& map, const sf::Vector2u& screenres, sf::FloatRect playerBounds = sf::FloatRect()) override;
    sf::FloatRect getBounds();
    void draw(sf::RenderWindow& window) override;

private:
bool *gameover;
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