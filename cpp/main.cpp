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
void resetGame(Boss *&boss, Player *&player, PacMan *&twoPac, Idk *&wtf, Npc *&npc, Inventory &inventory, sf::RenderWindow &window, Map &map, std::unique_ptr<TextBox> &textBox)
{
    delete twoPac, boss, player, wtf, npc;
    boss = new Boss("../imgs/ikeaman.jpg", window.getSize());
    player = new Player(sf::Vector2f(500, 384));
    twoPac = new PacMan(sf::Vector2f(-107, 384));
    wtf = new Idk(sf::Vector2f(300, 300), 200);
    npc = new Npc(sf::Vector2f(1300, 300), textBox.get());

    // Reinitialize the inventory with the new player
    inventory.reset(player);
}

int main()
{
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(sf::VideoMode(1024, 768), "SFML Player", sf::Style::Default);
    window.setFramerateLimit(60);

    Boss *boss = new Boss("../imgs/ikeaman.jpg", window.getSize());
    std::vector<std::string> cutSceneImages = {
        //"../imgs/player.png", "../imgs/2sleepHours.jpg", "../imgs/fun.jpg", "../imgs/haha.jpg"
    };
    CutScene cutScene(cutSceneImages, window.getSize());
    Map map("../map.mib", window);
    std::unique_ptr<TextBox> textBox = std::make_unique<TextBox>("", 0.1f);
    Player *player = new Player(sf::Vector2f(500, 384));
    PacMan *twoPac = new PacMan(sf::Vector2f(-107, 384));
    Idk *wtf = new Idk(sf::Vector2f(300, 300), 200);
    Npc *npc = new Npc(sf::Vector2f(1300, 300), textBox.get());
    GameOverScreen gameOverScreen(window);
    Menu menu(window);
    Inventory inventory(map, player, window);

    GameState currentState = GameState::Menu;
    sf::Clock clock;

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
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && menu.isPlayButtonClicked())
            {
                resetGame(boss, player, twoPac, wtf, npc, inventory, window, map, textBox);
                currentState = GameState::CutScene;
                clock.restart();
            }
            menu.updateButtonColor();
            menu.draw();
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
                twoPac->update(deltaTime, map, window.getSize());
                wtf->update(deltaTime, map, window.getSize());
                npc->update(deltaTime, map, window.getSize(), player->getBounds());
                player->update(deltaTime, map, window.getSize());
                boss->update(deltaTime, map, window.getSize(),player->getBounds());
                textBox->update(deltaTime);
                inventory.update();

                map.draw();
                boss->draw(window);
                wtf->draw(window);
                npc->draw(window);
                twoPac->draw(window);
                player->draw(window);
                inventory.draw();
                textBox->draw(window);
            }
            break;
        case GameState::GameOver:
            if (gameOverScreen.handleEvent(event))
            {
                currentState = GameState::Menu;
            }
            gameOverScreen.draw();
            break;
        }

        window.display();
    }

    delete twoPac;
    return 0;
}