#pragma once
class Animation;
class CollisionDetector;
class Map;
class Entity : public Animation, public CollisionDetector {
public:
    Entity();
    virtual void update(float deltaTime, Map& map, const sf::Vector2u& screenres) = 0; // Pure virtual function for updating the entity's state
    virtual void draw(sf::RenderWindow& window) = 0; // Pure virtual function for drawing the entity
    virtual std::vector<std::pair<std::string, std::string>> getEditableProperties() const{
        return {};
    }
    virtual void setProperty(const std::string& name, const std::string& value){
        
    }
    virtual ~Entity() = default;
    void setPosition(const sf::Vector2f &pos); // Set the entity's position
    void setPosition(float x, float y);
    sf::Vector2f getPosition() const; // Get the entity's current position
    void move(const sf::Vector2f& offset); // Move the entity by a given offset
    bool isOnScreen(sf::FloatRect screen);
    sf::FloatRect *playerBounds;

    bool shouldBeDead;
protected:
    sf::Vector2f position; // The entity's current position
    sf::Vector2f velocity; // The entity's current velocity
};