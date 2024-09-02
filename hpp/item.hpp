#pragma once
class Inventory;

class Item{
    friend class Inventory;
    public:
    class Active{
        public:
        virtual void activate() = 0;
    };

    public:
    Item(float x, float y, float sizet, float speedb, float jumpb, std::string description, std::string name, std::string fname);
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
    void setStasis(Player* player, bool stasis);
};

//stands for Horus's Brogans
class HB : public Item{
    public:
    HB(float x, float y);
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
    RP(float x, float y);
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
    GB(float x, float y);
    ~GB() = default;

    void updateO(Player *player) override;//the update if you own the item
    bool updateU(Player *player) override;//the update if you do not own it

    private:
    int jc;
    bool isGroundedP;
    float jpowerg;
};

//stands for Chronos Time Piece
class CTP : public Item, public Item::Active{
    public:
    CTP(float x, float y);
    ~CTP() = default;

    void updateO(Player *player) override;//the update if you own the item
    bool updateU(Player *player) override;//the update if you do not own it
    void activate() override;//the functions that gets called when the button related to your active ability gets pressed

    private:
    sf::Clock timer;
    bool activated, fc;
};