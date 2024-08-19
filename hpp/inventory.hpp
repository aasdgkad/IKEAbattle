#pragma once

class Inventory{
    public:
    Inventory(Map& map, Player* player, sf::RenderWindow& window);
    ~Inventory() = default;

    void update();
    void draw();
    void reset(Player* player);
    private:
    sf::Font font;
    sf::Text text;
    sf::Texture bpTex, cellTex;
    sf::Sprite bpSprite, cellSprite[40], selectedSquare, selectedItemS;
    std::vector<Item*> allItems;
    std::vector<int> ownedItems;
    int selectedItem, pgcount;
    bool shouldDraw, fc;
    Map &mapr;
    Player *playerr;
    sf::RenderWindow &windowr;
};