#include "../hpp/libs.hpp"
class Player;
class Map;

// Define an enum for game states
enum class GameState
{
    Menu,
    CutScene,
    Playing,
    GameOver
};
void resetGame(Boss *&boss, Player *&player, PacMan *&twoPac, Idk *&wtf, sf::RenderWindow &window)
{
    delete twoPac, boss, player, wtf;
    boss = new Boss("../imgs/ikeaman.jpg", window.getSize());
    player = new Player(sf::Vector2f(500, 384));
    twoPac = new PacMan(sf::Vector2f(-107, 384));
    wtf = new Idk(sf::Vector2f(300, 300), 200);
}
int main()
{
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(sf::VideoMode(1024, 768), "SFML Player", sf::Style::Default);
    window.setFramerateLimit(60);
    Menu menu(window.getSize());
    GameOverScreen gameOverScreen;

    Boss *boss = new Boss("../imgs/ikeaman.jpg", window.getSize());
    std::vector<std::string> cutSceneImages = {
        //"../imgs/player.png", "../imgs/2sleepHours.jpg", "../imgs/fun.jpg", "../imgs/haha.jpg"
    };
    CutScene cutScene(cutSceneImages, window.getSize());
    Map map("../map.mib", window);
    Player *player = new Player(sf::Vector2f(500, 384));
    PacMan *twoPac = new PacMan(sf::Vector2f(-107, 384));
    Idk *wtf = new Idk(sf::Vector2f(300, 300), 200);

    GameState currentState = GameState::Menu;
    sf::Clock clock;
    sf::Clock bossMovementClock;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        float deltaTime = clock.restart().asSeconds();

        window.clear(sf::Color::Black);

        switch (currentState)
        {
        case GameState::Menu:
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && menu.isPlayButtonClicked(sf::Mouse::getPosition(window)))
            {
                resetGame(boss, player, twoPac, wtf, window);
                currentState = GameState::CutScene;
                clock.restart();
                bossMovementClock.restart();
            }
            menu.updateButtonColor(sf::Mouse::getPosition(window));
            menu.draw(window);
            break;
        case GameState::CutScene:
            if (cutScene.update(deltaTime))
            {
                currentState = GameState::Playing;
            }
            cutScene.draw(window);
            break;
        case GameState::Playing:
            if (boss->gameOver == true)
            {
                currentState = GameState::GameOver;
                boss->gameOver = false;
            }
            else
            {
                if (bossMovementClock.getElapsedTime().asSeconds() >= 3.0f)
                {
                    float minX = 0;
                    float maxX = window.getSize().x;
                    boss->moveRandomlyOnXAxis(minX, maxX);
                    bossMovementClock.restart();
                }
                twoPac->update(deltaTime, map, window.getSize());
                boss->update(deltaTime, map, window.getSize(), player->getBounds());
                wtf->update(deltaTime, map, window.getSize());
                player->update(deltaTime, map, window.getSize());

                map.draw();
                boss->draw(window);
                wtf->draw(window);
                twoPac->draw(window);
                player->draw(window);
            }
            break;
        case GameState::GameOver:
            if (gameOverScreen.handleEvent(event, sf::Mouse::getPosition(window)))
            {
                currentState = GameState::Menu;
            }
            gameOverScreen.draw(window);
            break;
        }

        window.display();
    }

    delete twoPac;
    return 0;
}