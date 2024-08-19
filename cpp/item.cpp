#include "../hpp/libs.hpp"

Item::Item(float sizet, float speedb, float jumpb, std::string description, std::string name, std::string fname) : 
    owned(0),
    sizet(sizet),
    speedb(speedb),
    jumpb(jumpb),
    name(name),
    description(description)
{
    this->texture.loadFromFile(fname);
    this->sprite.setTexture(this->texture);
}

void Item::applyStats(Player* player){
    player->jumpForce *= this->jumpb;
    player->moveSpeed *= this->speedb;
    player->sprite.scale(sizet, sizet);
}

void Item::draw(sf::RenderWindow &window){
    window.draw(this->sprite);
}

HB::HB() : 
    Item(1, 1.2, 1.2, "Avoid one instance of damage once every 15 seconds, except when it doesnt", "Horus's Brogans", "../imgs/HorusBrogans.png")
{
    this->sprite.setPosition(600,250);
}

bool HB::update(Player* player){
    if(owned){

    }
    else{
        if(player->getBounds().intersects(this->sprite.getGlobalBounds())){
            owned = true;
            applyStats(player);
            return true;
        }
    }
    return false;
}
