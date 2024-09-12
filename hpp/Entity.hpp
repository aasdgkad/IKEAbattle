#pragma once
class Animation;
class CollisionDetector;
class Map;
class Entity {
public:
    Entity();
    virtual ~Entity() = default;
    
    virtual void update(float deltaTime, Map &map, const sf::Vector2u &screenrese) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual std::vector<std::pair<std::string, std::string>> getEditableProperties() const{
        return {};
    }
    virtual void setProperty(const std::string& name, const std::string& value){
        
    }
    
    sf::Vector2f getPosition() const;
    void move(const sf::Vector2f& offset);
    bool isOnScreen(sf::FloatRect screen);
    sf::FloatRect *playerBounds;

    bool shouldBeDead;

protected:
    sf::Vector2f position;
    sf::Vector2f velocity;
};