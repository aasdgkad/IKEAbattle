#include "../hpp/libs.hpp"

Entity::Entity() : position(0, 0), velocity(0, 0) {}

void Entity::setPosition(const sf::Vector2f& pos) {
    position = pos;
    getSprite().setPosition(position);
}
void Entity::setPosition(float x, float y) {
    position.x = x;
    position.y = y;
    getSprite().setPosition(position);
}

sf::Vector2f Entity::getPosition() const {
    return position;
}

void Entity::move(const sf::Vector2f& offset) {
    position += offset;
    getSprite().setPosition(position);
}
bool Entity::isOnScreen(sf::FloatRect screen){
    return screen.contains(position);
}