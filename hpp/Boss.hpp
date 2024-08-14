#pragma once

class Boss : public Entity  {
public:
    Boss(const std::string& imagePath, sf::Vector2u windowSize);
    void update(float deltaTime, Map& map, const sf::Vector2u& screenres, sf::FloatRect playerBounds) override;
    void draw(sf::RenderWindow& window);
    ~Boss();

private:
    sf::Texture texture;
    sf::Texture eyetexture;
    sf::Sprite sprite;
    sf::Sprite eyesprite;
    sf::Vector2f velocity;
    float bounceSpeed;
    float targetWidth;
    sf::Vector2u windowSize;
    float spawnTimer;
    std::vector<std::unique_ptr<Idk>> idkObjects;
    std::vector<Attack*> attacks;
    sf::Clock atimer;
    sf::Clock ptimer;
    sf::Clock ltimer;
    sf::Clock ttimer;

    void loadAndScaleImage(const std::string& imagePath);
    void setInitialPosition();
    void spawnIdk();
};