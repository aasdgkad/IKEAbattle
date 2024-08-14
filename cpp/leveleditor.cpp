#include "../hpp/libs.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1024, 768), "Level Editor", sf::Style::Titlebar | sf::Style::Close);
    Map map("../map.mib", window);

    sf::View view(sf::FloatRect(0,0,window.getSize().x,window.getSize().y));
    sf::Texture rectex;
    sf::RectangleShape transrect;// This is the transparent placeholder rectangle 
    sf::Vector2i fc(0,0), sc(0,0), ap(0,0), mp(0,0);// Stands for first click, second click, anchor point, magical potion ;)
    bool lc = 0;// Stands for right click, left click, used to determine if the left or right click are currently being pressed
    

    window.setView(view);
    rectex.loadFromFile("../imgs/0.png");
    rectex.setRepeated(true);
    transrect.setFillColor(sf::Color(255, 255, 255, 128));
    transrect.setTexture(&rectex);


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
            // If this is the first time clicking then lock in left click and get the position of one corner of the rectangle
            if(!lc){
                fc = sf::Mouse::getPosition(window);
                lc = 1;
            }
        }
        else if(lc){
            // When we release the mouse we create the object based on the placeholder rectangle and then add it to the map
            map.addObject(std::min(sc.x, fc.x), std::min(sc.y, fc.y), std::abs(sc.x-fc.x), std::abs(sc.y-fc.y), "../imgs/0.png");
            fc = sf::Vector2i(0,0);
            sc = sf::Vector2i(0,0);
            transrect.setSize(sf::Vector2f(0,0));
            lc = 0;
        }

        if(lc){
            // When we are holding the left click draw the placeholder rectangle and update its second corner
            sc = sf::Mouse::getPosition(window);
            transrect.setSize(sf::Vector2f(std::abs(sc.x-fc.x), std::abs(sc.y-fc.y)));
            transrect.setPosition(sf::Vector2f(std::min(sc.x, fc.x) + map.getPartBounds().left,std::min(sc.y, fc.y) + map.getPartBounds().top));
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
            map.saveToFile("../map.mib");
            while(sf::Keyboard::isKeyPressed(sf::Keyboard::S));
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
            map.changePart(-1,0);
            while(sf::Keyboard::isKeyPressed(sf::Keyboard::Left));
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
            map.changePart(1,0);
            while(sf::Keyboard::isKeyPressed(sf::Keyboard::Right));
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
            map.changePart(0,-1);
            while(sf::Keyboard::isKeyPressed(sf::Keyboard::Up));
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
            map.changePart(0,1);
            while(sf::Keyboard::isKeyPressed(sf::Keyboard::Down));
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
            auto mp = sf::Mouse::getPosition(window);
            auto list = map.getObjectBounds();
            for(int i = 0; i < list.size(); i++){
                if(list[i].contains(mp.x, mp.y)){
                    map.removeObject(i);
                    break;
                }
            }
        }

        window.clear();
        window.draw(transrect);
        map.draw();
        window.display();
    }

    return 0;
}