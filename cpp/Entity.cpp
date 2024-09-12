#include "../hpp/libs.hpp"

Entity::Entity() : position(0,0), velocity(0, 0),shouldBeDead(false) {}

sf::Vector2f Entity::getPosition() const {
    return position;
}

void Entity::move(const sf::Vector2f& offset) {
    position += offset;
}

bool Entity::isOnScreen(sf::FloatRect screen) {
    return screen.contains(position);
}