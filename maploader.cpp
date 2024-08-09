#include "libs.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Level Editor", sf::Style::Titlebar | sf::Style::Close);
    Map map("map.mib");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            
        }

        

        window.clear();
        map.draw(window);
        window.display();
    }

    return 0;
}