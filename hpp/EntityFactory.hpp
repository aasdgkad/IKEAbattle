#pragma once
#include "libs.hpp"
class PacMan;
class Player;
class Boss;
class Npc;
class Idk;

class EntityFactory {
public:
    static Entity* createEntity(const std::string& type, const sf::Vector2f& position , bool& gameOver);
};