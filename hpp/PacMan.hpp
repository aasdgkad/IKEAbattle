#pragma once

class PacMan : public Entity {
public:
    PacMan(sf::Vector2f spawnPosition);
    ~PacMan(); // Destructor

    void update(float deltaTime, Map& map, const sf::Vector2u& screenres,sf::FloatRect playerBounds = sf::FloatRect()) override;
    void draw(sf::RenderWindow& window) override;
    bool shouldRemove();
private:
    bool hasAppearedOnScreen;
    float lifeTimer;
    const float lifeDuration;
    const float speed;

    void loadSprite();
};