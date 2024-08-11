#include "libs.hpp"

Attack::Attack(sf::Vector2f sp) : pos(sp) {

}

HammerThrow::HammerThrow(sf::Vector2f sp) : Attack(sp), fc(true){
    this->texture.loadFromFile("./imgs/hammer.jpg");
    this->sprite.setTexture(this->texture);
    this->sprite.setPosition(this->pos);
    this->sprite.setScale(32.0f/this->texture.getSize().x, 32.0f/this->texture.getSize().y);
}

void HammerThrow::update(sf::FloatRect player){
    if(fc){
        this->velocity = player.getPosition() + player.getSize()/2.0f - this->pos;
        this->velocity /= sqrt(this->velocity.x*this->velocity.x + this->velocity.y*this->velocity.y);
        this->velocity *= 10.0f;
        fc = false;
    }
    else{
        this->pos += this->velocity;
        this->sprite.rotate(5);
        this->sprite.setPosition(this->pos);
    }
    if(player.intersects(this->sprite.getGlobalBounds()))
        exit(0);
}

void HammerThrow::draw(sf::RenderWindow &window){
    window.draw(this->sprite);
}