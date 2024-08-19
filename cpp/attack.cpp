#include "../hpp/libs.hpp"

Attack::Attack(sf::Vector2f sp,bool &gameOver) : pos(sp),gameOver(&gameOver) {
    
}

HammerThrow::HammerThrow(sf::Vector2f sp,bool &gameOver) : Attack(sp,gameOver), fc(true){
    this->texture.loadFromFile("../imgs/hammer.jpg");
    this->sprite.setTexture(this->texture);
    this->sprite.setPosition(this->pos);
    this->sprite.setScale(32.0f/this->texture.getSize().x, 32.0f/this->texture.getSize().y);
    this->sprite.move(0,-this->sprite.getGlobalBounds().height);
}

bool HammerThrow::update(sf::FloatRect player){
    //hammer throw should be changed to throw multiple hammers in different directions at the same time
    return false;
}

void HammerThrow::draw(sf::RenderWindow &window){
    window.draw(this->sprite);
}

Plank::Plank(sf::Vector2f sp,bool &gameOver) : Attack(sp,gameOver), fc(true){
    this->texture.loadFromFile("../imgs/plank.png");
    this->sprite[0].setTexture(this->texture);
    this->sprite[0].setScale(20.0f/this->texture.getSize().x, 100.0f/this->texture.getSize().y);
    this->sprite[1].setTexture(this->texture);
    this->sprite[1].setScale(20.0f/this->texture.getSize().x, 100.0f/this->texture.getSize().y);
}

bool Plank::update(sf::FloatRect player){
    float deltatime = this->timer.getElapsedTime().asSeconds();
    if(deltatime < 0.9f){
        if(fc){
            pos = player.getPosition();
            fc = false;
        }
        sprite[0].setPosition(sf::Vector2f(pos.x - 5*player.width, pos.y-20));
        sprite[1].setPosition(sf::Vector2f(pos.x + 5*player.width, pos.y-20));
    }
    else{
        sprite[0].setPosition(sf::Vector2f(pos.x - 5*player.width + 500*(deltatime-0.9f), pos.y - 20));
        sprite[1].setPosition(sf::Vector2f(pos.x + 5*player.width - 500*(deltatime-0.9f), pos.y - 20));

        if(sprite[0].getPosition().x >= pos.x)
            return true;
    }
    if(this->sprite[0].getGlobalBounds().intersects(player) || this->sprite[1].getGlobalBounds().intersects(player))
        (*gameOver)=true;
    return false;
}

void Plank::draw(sf::RenderWindow &window){
    window.draw(this->sprite[0]);
    window.draw(this->sprite[1]);
}

LaserBeam::LaserBeam(sf::Vector2f sp, float rotangle,bool &gameOver) : Attack(sp,gameOver), fc(true){
    this->texture.loadFromFile("../imgs/laser.png");
    this->sprite.setTexture(this->texture);
    this->sprite.setPosition(this->pos);
    this->sprite.rotate(90 + rotangle);
}

bool LaserBeam::update(sf::FloatRect player){
    if(fc){
        this->velocity = player.getPosition() + player.getSize()/2.0f - this->pos;
        this->velocity /= sqrt(this->velocity.x*this->velocity.x + this->velocity.y*this->velocity.y);
        this->velocity *= 20.0f;
        fc = false;
    }
    else{
        this->pos += this->velocity;
        this->sprite.setPosition(this->pos);
    }
    if(player.intersects(this->sprite.getGlobalBounds()))
        (*gameOver)=true;
    return false;
}

void LaserBeam::draw(sf::RenderWindow &window){
    window.draw(this->sprite);
}

TableFall::TableFall(sf::Vector2f sp,bool &gameOver) : Attack(sp,gameOver){
    this->texture.loadFromFile("../imgs/table.png");
    this->sprite.setTexture(this->texture);
    this->sprite.setOrigin(this->texture.getSize().x/2.0f, this->texture.getSize().y);
    this->sprite.setScale(60.0f/this->texture.getSize().x, 30.0f/this->texture.getSize().y);
    this->sprite.setPosition(this->pos);
}

bool TableFall::update(sf::FloatRect player){
    this->velocity.y += 0.5f;
    this->sprite.move(0,velocity.y);
    if(this->sprite.getGlobalBounds().intersects(player))
        (*gameOver)=true;
    return false;
}

void TableFall::draw(sf::RenderWindow &window){
    window.draw(this->sprite);
}