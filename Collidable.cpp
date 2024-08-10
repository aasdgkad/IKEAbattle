#include "libs.hpp"
bool Collidable::checkCollision(const Map& map) const {
    for (const auto& objBounds : map.getObjectBounds()) {
        if (bounds.intersects(objBounds)) {
            return true;
        }
    }
    return false;
}