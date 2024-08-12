#pragma once

class CollisionDetector {
public:
    // Enum to represent the side of collision
    enum class CollisionSide {
        None,
        Top,
        Bottom,
        Left,
        Right
    };
    // Struct to hold collision information
    struct CollisionInfo {
        bool collided;
        CollisionSide side;
        sf::FloatRect intersection;
    };
    // Check for collision between an object and obstacles
    CollisionInfo checkCollision(const sf::FloatRect& bounds, const std::vector<sf::FloatRect>& obstacles);
protected:
    // Determine the side of collision
    CollisionSide getCollisionSide(const sf::FloatRect& object, const sf::FloatRect& obstacle, const sf::FloatRect& intersection);
};