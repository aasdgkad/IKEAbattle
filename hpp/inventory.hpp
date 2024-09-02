#pragma once

class Inventory{
    public:
    Inventory(Map& map, Player* player, sf::RenderWindow& window);
    ~Inventory() = default;

    void update();
    void draw();
    void reset(Player* player);

    private:
    void loadPanel();
    void loadItems();
    void selectItem(int i);
    int getHoverCell();
    int getActiveHoverCell();

    private:
    sf::Font font;
    sf::Text text;
    sf::Texture bpTex, cellTex;
    sf::Sprite bpSprite, cellSprite[40], selectedSquare, selectedItemS, activeCellS[3];
    std::vector<Item*> allItems;
    Item::Active* active[3];
    std::vector<int> ownedItems, unownedItems;
    int selectedItem, pgcount, movedItem;
    bool shouldDraw, fc, movingItem;
    Map &mapr;
    Player *playerr;
    sf::RenderWindow &windowr;
};