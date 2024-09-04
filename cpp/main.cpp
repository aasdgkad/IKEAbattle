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

void resetGame(Map &map,Player *&player, Inventory &inventory, sf::RenderWindow &window, bool &gameover)
{
    player = new Player(sf::Vector2f(500, 500),gameover);
    map.resetEntities(player->place);
    inventory.reset(player);
    gameover = false;
}

int main()
{
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(sf::VideoMode(1024, 768), "SFML Player", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);

    std::vector<std::string> cutSceneImages = {
        
    };
    bool gameOver = false;
    Player *player = new Player(sf::Vector2f(500, 500),gameOver);
    CutScene cutScene(cutSceneImages, window.getSize());
    Map map("../map.mib", window,gameOver);
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
                resetGame(map,player, inventory, window, gameOver);
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
            if (gameOver)
            {
                currentState = GameState::GameOver;
                gameOver = false;
            }
            else
            {
                map.updateEntities(deltaTime, window.getSize());
                player->update(deltaTime, map, window.getSize());
                inventory.update();

                map.draw();
                map.drawEntities(window);
                player->draw(window);
                inventory.draw();
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

    return 0;
}