#pragma once

class Map;

class Penguin : public Entity {
public:
    Penguin(sf::Vector2f position);
    void update(float deltaTime, Map& map, const sf::Vector2u& screenres) override;
    void draw(sf::RenderWindow& window) override;
    std::vector<std::pair<std::string, std::string>> getEditableProperties() const override;
    void setProperty(const std::string& name, const std::string& value) override;

private:
    void loadSprite();
    void manageCollisions(const std::vector<sf::FloatRect>& objectBounds);
    float gravity;
    bool isColliding;
    bool isMovingRight;
    float speed;
};