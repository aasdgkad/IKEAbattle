#pragma once

class Collidable {
protected:
    sf::FloatRect bounds;

public:
    Collidable(sf::FloatRect bounds) : bounds(bounds) {}
    virtual ~Collidable() = default;

    bool checkCollision(const Map& map) const;
    void setBounds(sf::FloatRect newBounds) { bounds = newBounds; }
    sf::FloatRect getBounds() const { return bounds; }
};
