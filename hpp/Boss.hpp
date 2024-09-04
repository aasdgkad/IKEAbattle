#pragma once

class Boss : public Entity  {
public:
    Boss(const std::string& imagePath, sf::Vector2u windowSize,bool &gameOver);
    void update(float deltaTime, Map& map, const sf::Vector2u& screenres) override;
    void draw(sf::RenderWindow& window);
    void resetTimers();
    ~Boss();

private:
    bool *gameover;
    sf::Texture texture;
    sf::Texture eyetexture;
    sf::Sprite sprite;
    sf::Sprite eyesprite;
    sf::Vector2f velocity;
    float targetWidth;
    sf::Vector2u windowSize;
    std::vector<Attack*> attacks;
    sf::Clock atimer;
    sf::Clock ptimer;
    sf::Clock ltimer;
    sf::Clock ttimer;

    void loadAndScaleImage(const std::string& imagePath);
    void setInitialPosition();
};