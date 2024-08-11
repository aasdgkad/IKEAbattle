#pragma once
class Idk : public Entity {
public:
    Idk(sf::Vector2f position, float moveDistance);
    void update(float deltaTime, Map& map, const sf::Vector2u& screenres) override;
    void draw(sf::RenderWindow& window) override;

private:
    void loadSprite();
    void manageCollisions(const std::vector<sf::FloatRect>& objectBounds);

    sf::RectangleShape shape;
    float moveSpeed;
    float moveDistance;
    float initialX;
    float gravity;
    bool movingRight;
};