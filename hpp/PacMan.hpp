#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.hpp"
#include "Map.hpp"

class PacMan : public Animation, public CollisionDetector {
public:
    PacMan(sf::Vector2f spawnPosition, bool &gameOver);
    void update(float deltaTime, Map& map, const sf::Vector2u& screenres) override;
    void draw(sf::RenderWindow& window) override;
    bool shouldRemove();
    std::vector<std::pair<std::string, std::string>> getEditableProperties() const override;
    void setProperty(const std::string& name, const std::string& value) override;
private:
    bool hasAppearedOnScreen;
    float lifeTimer;
    const float lifeDuration;
    bool *gameOver;
    std::string customString;
    float customFloat;
    void loadSprite();
    std::string name;
    float speed;
    float degrees;
    void updateDirection();
};