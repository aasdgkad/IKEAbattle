#include "libs.hpp"

Entity::Entity() : position(0, 0), velocity(0, 0) {}

void Entity::setPosition(const sf::Vector2f& pos) {
    position = pos;
    getSprite().setPosition(position);
}

sf::Vector2f Entity::getPosition() const {
    return position;
}

void Entity::move(const sf::Vector2f& offset) {
    position += offset;
    getSprite().setPosition(position);
}