#pragma once
class Map; // Forward declaration
class Npc : public Entity {
public:
    Npc(sf::Vector2f position);
    void update(float deltaTime, Map& map, const sf::Vector2u& screenres, sf::FloatRect playerBounds = sf::FloatRect()) override;
    void draw(sf::RenderWindow& window) override;
    std::unique_ptr<TextBox> textBox;

private:
    void loadSprite();
    void manageCollisions(const std::vector<sf::FloatRect>& objectBounds);
    void checkCollisionWithPlayer(const sf::FloatRect& playerBounds);
    float gravity;
    sf::RenderWindow* window;
    bool isColliding;
};