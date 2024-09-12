#pragma once
class Entity;
class Sprite : public Entity
{
public:
    virtual std::vector<std::pair<std::string, std::string>> getEditableProperties() const
    {
        return {};
    }
    virtual void setProperty(const std::string &name, const std::string &value)
    {
    }
    Sprite();
    void loadTexture(const std::string &filename);
    void update(float deltaTime, Map &map, const sf::Vector2u &screenres) override;
    void draw(sf::RenderWindow &window) override;
    void setPosition(const sf::Vector2f &pos);
    void setPosition(float x, float y);

protected:
    bool flipped;
    sf::Sprite sprite;
    sf::Texture texture;
};