#pragma once

class Player{
    public:
    Player();
    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;
    ~Player();

    private:
};