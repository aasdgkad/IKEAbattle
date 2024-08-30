#pragma once
class Inventory;

class Item{
    friend class Inventory;
    public:
    Item(float sizet, float speedb, float jumpb, std::string description, std::string name, std::string fname);
    ~Item() = default;

    virtual void updateO(Player *player) = 0;//the update if you own the item
    virtual bool updateU(Player *player) = 0;//the update if you do not own it
    void draw(sf::RenderWindow &window);

    //for custom writing in the inventory
    virtual std::string customText();

    protected:
    float sizet, speedb, jumpb;
    std::string description, name;
    sf::Texture texture;
    sf::Sprite sprite;

    protected:
    void applyStats(Player* player);
    void addStats(Player *player, float speed, float jump);
    void setOneHitInv(Player* player, bool val);
    bool getOneHitInv(Player* player);
    bool isGrounded(Player* player);
    float getJumpForce(Player* player);
};

//stands for Horus's Brogans
class HB : public Item{
    public:
    HB();
    ~HB() = default;

    void updateO(Player *player) override;
    bool updateU(Player *player) override;

    private:
    sf::Clock ctimer;
    sf::Clock btimer;
    bool pinv, fc;
};

//stands for Runner's Pact
class RP : public Item{
    public:
    RP();
    ~RP() = default;

    void updateO(Player *player) override;//the update if you own the item
    bool updateU(Player *player) override;//the update if you do not own it

    //for custom text writing
    std::string customText() override;

    private:
    float px, xdis, accsp;
};

//stands for Ground Breaker
class GB: public Item{
    public:
    GB();
    ~GB() = default;

    void updateO(Player *player) override;//the update if you own the item
    bool updateU(Player *player) override;//the update if you do not own it

    private:
    int jc;
    bool isGroundedP;
    float jpowerg;
};