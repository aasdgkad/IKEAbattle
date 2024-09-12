#include "../hpp/libs.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

sf::Vector2f getEntityOrigin(const sf::Sprite &sprite)
{
    sf::FloatRect bounds = sprite.getLocalBounds();
    return sf::Vector2f(bounds.width / 2, bounds.height / 2);
}

void updateEntityPreview(sf::Sprite &entityPreview, const sf::Texture *entityTexture)
{
    if (entityTexture)
    {
        entityPreview.setTexture(*entityTexture, true);
        entityPreview.setOrigin(getEntityOrigin(entityPreview));

        const float maxPreviewSize = 64.0f;
        sf::Vector2f scale(1.0f, 1.0f);
        if (entityPreview.getLocalBounds().width > maxPreviewSize ||
            entityPreview.getLocalBounds().height > maxPreviewSize)
        {
            scale.x = maxPreviewSize / entityPreview.getLocalBounds().width;
            scale.y = maxPreviewSize / entityPreview.getLocalBounds().height;
        }
        entityPreview.setScale(scale);

        entityPreview.setColor(sf::Color(255, 255, 255, 192)); // Semi-transparent
    }
}

int main()
{
    bool gameover;
    sf::RenderWindow window(sf::VideoMode(1024, 768), "Level Editor", sf::Style::Titlebar | sf::Style::Close);
    Map map("../map.mib", window, gameover);
    sf::View view(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y));
    sf::RectangleShape transrect;    // This is the transparent placeholder rectangle for textures
    sf::Sprite entityPreview;        // This is the preview sprite for entities
    sf::Vector2i fc(0, 0), sc(0, 0); // Stands for first click, second click
    bool lc = false;                 // Stands for left click, used to determine if the left click is currently being pressed
    bool placingTexture = false;      // True for texture, false for entity

    window.setView(view);

    transrect.setFillColor(sf::Color(255, 255, 255, 128));
    transrect.setTexture(map.getSelectedTexture());

    // Initialize entity preview
    const sf::Texture *initialEntityTexture = map.getEntityTexture(map.getSelectedName());
    updateEntityPreview(entityPreview, initialEntityTexture);

    Map::PropertyEditor propertyEditor;
    sf::Font font;
    if (!font.loadFromFile("../fonts/Arial.ttf"))
    {
        // Handle font loading error
        std::cerr << "Failed to load font!" << std::endl;
        return -1;
    }
    propertyEditor.setup(font);

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
                if (event.key.code == sf::Keyboard::E && !propertyEditor.isOpen)
                {
                    map.menu.isOpen = !map.menu.isOpen;
                }
            }
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    if (map.handleMenuClick(mousePos))
                    {
                        if (map.menu.isEntitySelected())
                        {
                            placingTexture = false;
                            transrect.setTexture(nullptr);
                            transrect.setFillColor(sf::Color::Transparent);
                            const sf::Texture *entityTexture = map.getSelectedTexture();
                            updateEntityPreview(entityPreview, entityTexture);
                        }
                        else
                        {
                            placingTexture = true;
                            transrect.setTexture(map.getSelectedTexture());
                            transrect.setFillColor(sf::Color(255, 255, 255, 128));
                        }
                    }
                    else if (!map.menu.isOpen && !propertyEditor.isOpen)
                    {
                        fc = mousePos;
                        lc = true;
                    }
                }
                else if (event.mouseButton.button == sf::Mouse::Right)
                {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

                    Map::PlacedEntity *clickedEntity = nullptr;
                    for (auto &placedEntity : map.placedEntities)
                    {
                        if (placedEntity->sprite.getGlobalBounds().contains(worldPos))
                        {
                            map.menu.isOpen = false;
                            clickedEntity = placedEntity.get();
                            break;
                        }
                    }

                    propertyEditor.updateForEntity(clickedEntity, font);
                }
            }
            else if (event.type == sf::Event::MouseButtonReleased && !map.menu.isOpen && !propertyEditor.isOpen)
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
                        sf::Vector2f entityPos = entityPreview.getPosition();
                        sf::Vector2f entitySize = sf::Vector2f(entityPreview.getGlobalBounds().width, entityPreview.getGlobalBounds().height);
                        map.addEntity(entityPos.x - entitySize.x / 2, entityPos.y - entitySize.y / 2, map.getSelectedName());
                    }
                    sc = sf::Vector2i(0, 0);
                    transrect.setSize(sf::Vector2f(0, 0));
                }
            }
            else if (event.type == sf::Event::MouseMoved && !propertyEditor.isOpen)
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);
                entityPreview.setPosition(worldPos);
            }

            propertyEditor.handleInput(event, window);
        }

        if (lc && placingTexture)
        {
            sc = sf::Mouse::getPosition(window);
            transrect.setSize(sf::Vector2f(std::abs(sc.x - fc.x), std::abs(sc.y - fc.y)));
            transrect.setPosition(sf::Vector2f(std::min(sc.x, fc.x) + map.getPartBounds().left,
                                               std::min(sc.y, fc.y) + map.getPartBounds().top));
        }
        if (!propertyEditor.isOpen)
        {

            // Handle keyboard input for map navigation and saving
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            {
                map.saveToFile("../map.mib");
                std::cout << "\nsaved map";
                while (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
                {
                } // Wait for key release
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                map.changePart(-1, 0);
                while (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                {
                } // Wait for key release
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                map.changePart(1, 0);
                while (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                {
                } // Wait for key release
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {
                map.changePart(0, -1);
                while (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                {
                } // Wait for key release
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            {
                map.changePart(0, 1);
                while (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                {
                } // Wait for key release
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
                        if (map.placedEntities[i]->sprite.getGlobalBounds().contains(worldPos))
                        {
                            map.removeEntity(i);
                            break;
                        }
                    }
                }
            }
        }

        // Apply property changes when Enter is pressed
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
        {
            propertyEditor.applyChanges();
        }

        // Drawing
        window.clear();
        map.draw();
        map.drawEditorEntities(window, propertyEditor.selectedEntity, propertyEditor.isOpen);
        window.draw(transrect);
        if (!placingTexture && !map.menu.isOpen && !propertyEditor.isOpen)
        {
            window.draw(entityPreview);
        }
        map.menu.draw();
        propertyEditor.draw(window);
        window.display();
    }
    return 0;
}