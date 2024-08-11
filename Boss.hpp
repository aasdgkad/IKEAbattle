// Boss.hpp
#pragma once

class Boss {
public:
    Boss(const std::string& imagePath, sf::Vector2u windowSize);
    void update(float deltaTime, Map& map, sf::FloatRect player);
    void draw(sf::RenderWindow& window);
    ~Boss();

private:
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Vector2f velocity;
    float bounceSpeed;
    float targetWidth;
    sf::Vector2u windowSize;
    float spawnTimer;
    std::vector<std::unique_ptr<Idk>> idkObjects;
    std::vector<Attack*> attacks;
    sf::Clock atimer;

    void loadAndScaleImage(const std::string& imagePath);
    void setInitialPosition();
    void spawnIdk();
};
