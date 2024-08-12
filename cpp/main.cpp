
#include "../hpp/libs.hpp"
class Player;
class Map;
int main() {
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(sf::VideoMode(1024, 768), "SFML Player", sf::Style::Default);
    Boss boss("../imgs/ikeaman.jpg", window.getSize());
    window.setFramerateLimit(60);

    // Create the cut scene
    std::vector<std::string> cutSceneImages = {
        "../imgs/player.png", "../imgs/2sleepHours.jpg", "../imgs/fun.jpg", "../imgs/haha.jpg"
    };
    CutScene cutScene(cutSceneImages, window.getSize());
    Map map("../map.mib", window);
    Player player(sf::Vector2f(500, 384));
    sf::Clock clock;
    PacMan* twoPac = new PacMan(sf::Vector2f(-107, 384));
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
            twoPac->update(deltaTime, map,window.getSize());
             boss.update(deltaTime, map, window.getSize(),player.getBounds());
             wtf.update(deltaTime, map,window.getSize());
            player.update(deltaTime, map,window.getSize());
            window.clear(sf::Color::Black);
            map.draw();
            boss.draw(window);
            wtf.draw(window);
            twoPac->draw(window);
            player.draw(window);

        }

        window.display();
    }

    return 0;
}