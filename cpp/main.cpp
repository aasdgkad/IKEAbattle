#include "../hpp/libs.hpp"
class Player;
class Map;
int main()
{
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(sf::VideoMode(1024, 768), "SFML Player", sf::Style::Default);
    Boss boss("../imgs/ikeaman.jpg", window.getSize());
    window.setFramerateLimit(60);

    Menu menu(window.getSize());

    std::vector<std::string> cutSceneImages = {
        "../imgs/player.png", "../imgs/2sleepHours.jpg", "../imgs/fun.jpg", "../imgs/haha.jpg"};
    CutScene cutScene(cutSceneImages, window.getSize());
    Map map("../map.mib", window);
    Player player(sf::Vector2f(500, 384));
    PacMan *twoPac = new PacMan(sf::Vector2f(-107, 384));

    bool gameStarted = false;
    sf::Clock clock; // Move clock declaration here
    bool firstime = true;// Please delete this later and kys

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                if (!gameStarted && menu.isPlayButtonClicked(sf::Mouse::getPosition(window)))
                {
                    gameStarted = true;
                    clock.restart(); // Reset the clock when starting the game
                }
            }
        }

        window.clear(sf::Color::Black);

        if (!gameStarted)
        {
            menu.updateButtonColor(sf::Mouse::getPosition(window));
        menu.draw(window);

        }
        else
        {
            float deltaTime = clock.restart().asSeconds(); // Calculate delta time here

            if (!cutScene.cutSceneFinished) {
                cutScene.cutSceneFinished = cutScene.update(deltaTime);
                cutScene.draw(window);
            }
            else
            {
                if(firstime){
                    boss.resetTimers();
                    firstime = false;
                }
                twoPac->update(deltaTime, map, window.getSize());
                boss.update(deltaTime, map, window.getSize(), player.getBounds());
                player.update(deltaTime, map, window.getSize());
                map.draw();
                boss.draw(window);
                twoPac->draw(window);
                player.draw(window);
            }
        }

        window.display();
    }

    return 0;
}