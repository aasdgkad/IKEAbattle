#include "../hpp/libs.hpp"

CollisionDetector::CollisionInfo CollisionDetector::checkCollision(const sf::FloatRect& bounds, const std::vector<sf::FloatRect>& obstacles) {
    for (const auto& obstacle : obstacles) {
        sf::FloatRect intersection;
        if (bounds.intersects(obstacle, intersection)) {
            CollisionSide side = getCollisionSide(bounds, obstacle, intersection);
            return {true, side, intersection};
        }
    }
    return {false, CollisionSide::None, sf::FloatRect()};
}

CollisionDetector::CollisionSide CollisionDetector::getCollisionSide(const sf::FloatRect& object, const sf::FloatRect& obstacle, const sf::FloatRect& intersection) {
    float leftOverlap = intersection.left + intersection.width - obstacle.left;
    float rightOverlap = obstacle.left + obstacle.width - intersection.left;
    float topOverlap = intersection.top + intersection.height - obstacle.top;
    float bottomOverlap = obstacle.top + obstacle.height - intersection.top;

    float minOverlap = std::min({leftOverlap, rightOverlap, topOverlap, bottomOverlap});

    if (minOverlap == leftOverlap) return CollisionSide::Left;
    if (minOverlap == rightOverlap) return CollisionSide::Right;
    if (minOverlap == topOverlap) return CollisionSide::Bottom;
    if (minOverlap == bottomOverlap) return CollisionSide::Top;
    return CollisionSide::None;
}