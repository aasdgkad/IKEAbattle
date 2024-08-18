#pragma once

class Attack{
    public:
    Attack(sf::Vector2f sp,bool &gameOver);

    virtual bool update(sf::FloatRect player) = 0;

    virtual void draw(sf::RenderWindow &window) = 0;
    bool *gameOver;
    sf::Vector2f pos;
    sf::FloatRect hitbox;
};

class HammerThrow : public Attack{
    public:
    HammerThrow(sf::Vector2f sp,bool &gameOver);

    bool update(sf::FloatRect player) override;

    void draw(sf::RenderWindow &window) override;

    private:
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Vector2f velocity;
    bool fc;
};

class Plank : public Attack{
    public:
    Plank(sf::Vector2f sp,bool &gameOver);

    bool update(sf::FloatRect player) override;

    void draw(sf::RenderWindow& window) override;

    private:
    sf::Texture texture;
    sf::Sprite sprite[2];
    sf::Vector2f velocity;
    sf::Clock timer;
    bool fc;
};

class LaserBeam : public Attack{
    public:
    LaserBeam(sf::Vector2f sp, float rotangle,bool &gameOver);

    bool update(sf::FloatRect player) override;

    void draw(sf::RenderWindow& window) override;

    private:
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Vector2f velocity;
    sf::Clock timer;
    bool fc;
};

class TableFall : public Attack{
    public:
    TableFall(sf::Vector2f sp,bool &gameOver);

    bool update(sf::FloatRect player) override;

    void draw(sf::RenderWindow& window) override;

    private:
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Vector2f velocity;
    sf::Clock timer;
    bool fc;
};