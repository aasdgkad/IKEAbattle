#pragma once
class Inventory;

class Item{
    friend class Inventory;
    public:
    Item(float sizet, float speedb, float jumpb, std::string description, std::string name, std::string fname);
    ~Item() = default;

    virtual bool update(Player *player) = 0;
    void draw(sf::RenderWindow &window);

    protected:
    float sizet, speedb, jumpb;
    std::string description, name;
    bool owned;
    sf::Texture texture;
    sf::Sprite sprite;

    protected:
    void applyStats(Player* player);
};

//stands for Horus's Brogans
class HB : public Item{
    public:
    HB();
    ~HB();

    bool update(Player *player) override;
};