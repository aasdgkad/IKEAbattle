#pragma once

class Npc : public Entity {
public:
    Npc(sf::Vector2f position, TextBox* textBox);
    void update(float deltaTime, Map& map, const sf::Vector2u& screenres, sf::FloatRect playerBounds = sf::FloatRect()) override;
    void draw(sf::RenderWindow& window) override;

private:
    void loadSprite();
    void manageCollisions(const std::vector<sf::FloatRect>& objectBounds);
    void checkCollisionWithPlayer(const sf::FloatRect& playerBounds);
    float gravity;
    TextBox* textBox;
    bool isColliding;
};