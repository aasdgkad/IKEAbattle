
#include "libs.hpp"
class Player;
class Map;
int main() {
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(sf::VideoMode(1024, 768), "SFML Player", sf::Style::Default);
    Boss boss("./imgs/oliverTree.png", window.getSize());
    window.setFramerateLimit(60);

    // Create the cut scene
    std::vector<std::string> cutSceneImages = {
        "./imgs/player.png", "./imgs/2sleepHours.jpg", "./imgs/fun.jpg", "./imgs/haha.jpg"
    };
    CutScene cutScene(cutSceneImages, window.getSize());
    Map map("map.mib");
    Player player(sf::Vector2f(500, 384));
    sf::Clock clock;
Idk wtf(sf::Vector2f(300, 300), 200);
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        float deltaTime = clock.restart().asSeconds();

        /*if (!cutScene.cutSceneFinished) {
            cutScene.cutSceneFinished = cutScene.update(deltaTime);
            window.clear(sf::Color::Black);
            cutScene.draw(window);
        } 
        else*/
         {
             boss.update(deltaTime, map);
             wtf.update(deltaTime, map,window.getSize());
            player.update(deltaTime, map,window.getSize());
            window.clear(sf::Color::White);
            map.draw(window);
            boss.draw(window);
            wtf.draw(window);
            player.draw(window);

        }

        window.display();
    }

    return 0;
}