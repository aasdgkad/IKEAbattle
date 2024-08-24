#include "../hpp/libs.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1024, 768), "Level Editor", sf::Style::Titlebar | sf::Style::Close);
    Map map("../map.mib", window);
    sf::View view(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y));
    sf::RectangleShape transrect; // This is the transparent placeholder rectangle 
    sf::Vector2i fc(0, 0), sc(0, 0), ap(0, 0), mp(0, 0); // Stands for first click, second click, anchor point, magical potion ;)
    bool lc = 0; // Stands for right click, left click, used to determine if the left or right click are currently being pressed
    
    window.setView(view);
    
    transrect.setFillColor(sf::Color(255, 255, 255, 128));
    transrect.setTexture(map.getSelectedTexture());

    while (window.isOpen())
    {
        window.setFramerateLimit(60);
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::E)
                {
                    map.toggleTextureMenu();
                }
                else if (event.key.code == sf::Keyboard::Up)
                {
                    transrect.setTexture(map.getSelectedTexture());
                }
                else if (event.key.code == sf::Keyboard::Down)
                {
                    transrect.setTexture(map.getSelectedTexture());
                }
            }
             else if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    if (map.handleTextureMenuClick(mousePos))
                    {
                        transrect.setTexture(map.getSelectedTexture());
                    }
                    else if (!map.isTextureMenuOpen())
                    {
                        // Existing left-click handling for placing objects
                        fc = mousePos;
                        lc = 1;
                    }
                }
            }
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !map.isTextureMenuOpen())
        {
            if (!lc)
            {
                fc = sf::Mouse::getPosition(window);
                lc = 1;
            }
        }
        else if (lc)
        {
            map.addObject(std::min(sc.x, fc.x), std::min(sc.y, fc.y), std::abs(sc.x - fc.x), std::abs(sc.y - fc.y));
            sc = sf::Vector2i(0, 0);
            transrect.setSize(sf::Vector2f(0, 0));
            lc = 0;
        }
        if (lc)
        {
            sc = sf::Mouse::getPosition(window);
            transrect.setSize(sf::Vector2f(std::abs(sc.x - fc.x), std::abs(sc.y - fc.y)));
            transrect.setPosition(sf::Vector2f(std::min(sc.x, fc.x) + map.getPartBounds().left, std::min(sc.y, fc.y) + map.getPartBounds().top));
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            map.saveToFile("../map.mib");
            while (sf::Keyboard::isKeyPressed(sf::Keyboard::S));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            map.changePart(-1, 0);
            while (sf::Keyboard::isKeyPressed(sf::Keyboard::Left));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            map.changePart(1, 0);
            while (sf::Keyboard::isKeyPressed(sf::Keyboard::Right));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            map.changePart(0, -1);
            while (sf::Keyboard::isKeyPressed(sf::Keyboard::Up));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            map.changePart(0, 1);
            while (sf::Keyboard::isKeyPressed(sf::Keyboard::Down));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            auto mp = sf::Mouse::getPosition(window);
            mp.x = mp.x + map.getPartBounds().getPosition().x;
            mp.y = mp.y + map.getPartBounds().getPosition().y;
            auto list = map.getObjectBounds();
            for (int i = 0; i < list.size(); i++)
            {
                if (list[i].contains(mp.x, mp.y))
                {
                    map.removeObject(i);
                    break;
                }
            }
        }

        window.clear();
        map.draw();
        map.drawTextureMenu();
        window.draw(transrect);
        window.display();
    }
    return 0;
}