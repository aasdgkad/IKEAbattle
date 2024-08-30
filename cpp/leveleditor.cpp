#include "../hpp/libs.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1024, 768), "Level Editor", sf::Style::Titlebar | sf::Style::Close);
    Map map("../map.mib", window);
    sf::View view(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y));
    sf::RectangleShape transrect;    // This is the transparent placeholder rectangle
    sf::Vector2i fc(0, 0), sc(0, 0); // Stands for first click, second click
    bool lc = false;                 // Stands for left click, used to determine if the left click is currently being pressed
    bool placingTexture = true;      // True for texture, false for entity

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
                    map.toggleEntityMenu();
                }
                else if (event.key.code == sf::Keyboard::Tab)
                {
                    placingTexture = !placingTexture;
                    if (placingTexture)
                    {
                        transrect.setTexture(map.getSelectedTexture());
                        transrect.setFillColor(sf::Color(255, 255, 255, 128));
                    }
                    else
                    {
                        transrect.setTexture(nullptr);
                        transrect.setFillColor(sf::Color::Blue);
                    }
                }
            }
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    if (map.handleTextureMenuClick(mousePos))
                    {
                        placingTexture = true;
                        transrect.setTexture(map.getSelectedTexture());
                        transrect.setFillColor(sf::Color(255, 255, 255, 128));
                    }
                    else if (map.handleEntityMenuClick(mousePos))
                    {
                        placingTexture = false;
                        transrect.setTexture(nullptr);
                        transrect.setFillColor(sf::Color::Blue);
                    }
                    else if (!map.isTextureMenuOpen() && !map.isEntityMenuOpen())
                    {
                        fc = mousePos;
                        lc = true;
                        if (!placingTexture)
                        {
                            // Immediately show the entity sprite
                            const sf::Texture *entityTexture = map.getEntityTexture(map.getSelectedEntityName());
                            if (entityTexture)
                            {
                                transrect.setTexture(entityTexture);
                                transrect.setSize(sf::Vector2f(entityTexture->getSize()));
                            }
                        }
                    }
                }
            }
            else if (event.type == sf::Event::MouseButtonReleased&&!map.isTextureMenuOpen() && !map.isEntityMenuOpen())
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    lc = false;
                    if (placingTexture)
                    {
                        map.addObject(std::min(sc.x, fc.x), std::min(sc.y, fc.y),
                                      std::abs(sc.x - fc.x), std::abs(sc.y - fc.y));
                    }
                    else
                    {
                        map.addEntity(std::min(sc.x, fc.x), std::min(sc.y, fc.y), map.getSelectedEntityName());
                    }
                    sc = sf::Vector2i(0, 0);
                    transrect.setSize(sf::Vector2f(0, 0));
                }
            }
        }

        if (lc)
        {
            sc = sf::Mouse::getPosition(window);
            if (placingTexture)
            {
                transrect.setSize(sf::Vector2f(std::abs(sc.x - fc.x), std::abs(sc.y - fc.y)));
                transrect.setPosition(sf::Vector2f(std::min(sc.x, fc.x) + map.getPartBounds().left,
                                                   std::min(sc.y, fc.y) + map.getPartBounds().top));
            }
            else
            {
                transrect.setSize(sf::Vector2f(32, 32)); // Use a fixed size for entity placement
                transrect.setPosition(sf::Vector2f(fc.x + map.getPartBounds().left,
                                                   fc.y + map.getPartBounds().top));
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            map.saveToFile("../map.mib");
            while (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
                ; // Wait for key release
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            map.changePart(-1, 0);
            while (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                ; // Wait for key release
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            map.changePart(1, 0);
            while (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                ; // Wait for key release
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            map.changePart(0, -1);
            while (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                ; // Wait for key release
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            map.changePart(0, 1);
            while (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                ; // Wait for key release
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

            bool deleted = false;

            // Check if an object is clicked
            std::vector<sf::FloatRect> objBounds = map.getObjectBounds();
            for (int i = 0; i < objBounds.size(); i++)
            {
                if (objBounds[i].contains(worldPos))
                {
                    map.removeObject(i);
                    deleted = true;
                    break;
                }
            }

            // If no object was deleted, check for entities
            if (!deleted)
            {
                // Check for entity deletion
                for (int i = map.placedEntities.size() - 1; i >= 0; --i)
                {
                    if (map.placedEntities[i].first.getGlobalBounds().contains(worldPos))
                    {
                        map.removeEntity(i);
                        break;
                    }
                }
            }
        }

        window.clear();
        map.draw();
        map.drawEditorEntities(window);
        map.drawTextureMenu();
        map.drawEntityMenu();
        window.draw(transrect);
        window.display();
    }
    return 0;
}