#include "../hpp/libs.hpp"
#include "../hpp/penguin.hpp"

Penguin::Penguin(sf::Vector2f position)
    : Animation(), CollisionDetector(), gravity(980.0f), isColliding(false), 
    speed(200.0f) {
    setPosition(position);
    loadSprite();
}

void Penguin::loadSprite() {
    loadSpritesheet("../imgs/penguinSpin/Spin Attack.png", 18, 16);
    addAnimation("spin", 0, 7);
    setAnimation("spin");
}

void Penguin::update(float deltaTime, Map &map, const sf::Vector2u& screenres) {
    if (isOnScreen(map.getPartBounds())) {
        velocity.y += gravity * deltaTime;
        position.y += velocity.y * deltaTime;
        
        velocity.x = speed * deltaTime;
        position.x += velocity.x;

        setPosition(position);
        manageCollisions(map.getObjectBounds());
        Animation::update(deltaTime,map,screenres);
    }
}

void Penguin::draw(sf::RenderWindow& window) {
    Animation::draw(window);
}

void Penguin::manageCollisions(const std::vector<sf::FloatRect>& objectBounds) {
    sf::FloatRect penguinBounds = sprite.getGlobalBounds();
    for (const auto& obstacle : objectBounds) {
        CollisionInfo collision = checkCollision(penguinBounds, {obstacle});
        if (collision.collided) {
            switch (collision.side) {
                case CollisionSide::Bottom:
                    position.y = obstacle.top - penguinBounds.height;
                    velocity.y = 0;
                    break;
                case CollisionSide::Left:
                case CollisionSide::Right:
                    speed = -speed;
                    break;
                case CollisionSide::Top:
                    position.y = obstacle.top + obstacle.height;
                    velocity.y = 0;
                    break;
                default:
                    break;
            }
            setPosition(position);
        }
    }
}