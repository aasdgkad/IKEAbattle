#include "../hpp/libs.hpp"
Entity* EntityFactory::createEntity(const std::string& type, const sf::Vector2f& position, bool* gameOver) {
        std::cout << type << " ";
    if (type == "pacman") {
        return new PacMan(position, *gameOver);
    } else if (type == "arrow") {
        return new Idk(position, 200);
    }else if (type == "capybaraa") {
            return new Npc(position);}
    return nullptr;
    }