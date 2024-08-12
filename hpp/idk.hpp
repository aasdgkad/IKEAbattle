#pragma once
class Idk : public Entity {
public:
    Idk(sf::Vector2f position, float moveDistance);
    void update(float deltaTime, Map& map, const sf::Vector2u& screenres,sf::FloatRect playerBounds = sf::FloatRect()) override;
    void draw(sf::RenderWindow& window) override;
    bool isOutOfBounds(const sf::Vector2u& windowSize) const;

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