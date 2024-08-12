#pragma once
class Entity : public Animation, public CollisionDetector {
public:
    Entity();
    virtual void update(float deltaTime, Map& map, const sf::Vector2u& screenres, sf::FloatRect playerBounds = sf::FloatRect()) = 0; // Pure virtual function for updating the entity's state
    virtual void draw(sf::RenderWindow& window) = 0; // Pure virtual function for drawing the entity
    void setPosition(const sf::Vector2f& pos); // Set the entity's position
    sf::Vector2f getPosition() const; // Get the entity's current position
    void move(const sf::Vector2f& offset); // Move the entity by a given offset
    bool isOnScreen(sf::FloatRect screen);
protected:
    sf::Vector2f position; // The entity's current position
    sf::Vector2f velocity; // The entity's current velocity
};