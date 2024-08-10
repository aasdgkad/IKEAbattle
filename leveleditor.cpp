#include "libs.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1024, 768), "Level Editor", sf::Style::Titlebar | sf::Style::Close);
    Map map;

    sf::View view(sf::FloatRect(0,0,window.getSize().x,window.getSize().y));
    sf::Texture rectex;
    sf::RectangleShape transrect;// This is the transparent placeholder rectangle 
    sf::Vector2i fc(0,0), sc(0,0), ap(0,0), mp(0,0);// Stands for first click, second click, anchor point, magical potion ;)
    int xoff = 0, yoff = 0;// The x and y offsets that are used by the right click movement
    float wscale = 1.0f;// The wheel scale factor
    bool rc = 0, lc = 0;// Stands for right click, left click, used to determine if the left or right click are currently being pressed
    

    window.setView(view);
    rectex.loadFromFile("0.png");
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
            // Check for mouse wheel scroll event
            if (event.type == sf::Event::MouseWheelScrolled) {
                // Determine the type of wheel (usually vertical)
                if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
                    // Adjusting the scale
                    wscale -= event.mouseWheelScroll.delta/50.0f;
                    if(wscale > 1.0f){
                        wscale = 1.0f;
                    }
                }
            }
        }

        if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
            // If this is the first time clicking then lock in left click and get the position of one corner of the rectangle
            if(!lc && !rc){
                fc = sf::Mouse::getPosition(window);
                fc.x = window.mapPixelToCoords(fc).x;
                fc.y = window.mapPixelToCoords(fc).y;
                lc = 1;
            }
        }
        else if(lc){
            // When we release the mouse we create the object based on the placeholder rectangle and then add it to the map
            map.addObject(std::min(sc.x, fc.x), std::min(sc.y, fc.y), std::abs(sc.x-fc.x), std::abs(sc.y-fc.y), "0.png");
            fc = sf::Vector2i(0,0);
            sc = sf::Vector2i(0,0);
            transrect.setSize(sf::Vector2f(0,0));
            lc = 0;
        }

        if(lc){
            // When we are holding the left click draw the placeholder rectangle and update its second corner
            sc = sf::Mouse::getPosition(window);
            sc.x = window.mapPixelToCoords(sc).x;
            sc.y = window.mapPixelToCoords(sc).y;
            transrect.setSize(sf::Vector2f(std::abs(sc.x-fc.x), std::abs(sc.y-fc.y)));
            transrect.setPosition(sf::Vector2f(std::min(sc.x, fc.x),std::min(sc.y, fc.y)));
        }



        if(sf::Mouse::isButtonPressed(sf::Mouse::Right)){
            // If this is the first time clicking then lock in right click and get the position of the anchor click
            if(!lc && !rc){
                ap = sf::Mouse::getPosition();
                rc = 1;
            }
            // While the right click is being pressed adjust the view accordingly
            else if(rc){
                mp = sf::Mouse::getPosition();
                view.setCenter(window.getSize().x/2 + xoff + ap.x - mp.x, window.getSize().y/2 + yoff + ap.y - mp.y);
                window.setView(view);
            }
        }
        else if(rc){
            // After we stopped clicking the right mouse button we update the offset to match the offset created by the last right mouse click
            xoff += ap.x-mp.x;
            yoff += ap.y-mp.y;
            rc = 0;
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
            map.saveToFile("map.mib");
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

        // Scaling the view
        view.setSize(window.getSize().x*wscale, window.getSize().y*wscale);
        window.setView(view);

        window.clear();
        window.draw(transrect);
        map.draw(window);
        window.display();
    }

    return 0;
}