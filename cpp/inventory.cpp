#include "../hpp/libs.hpp"

Inventory::Inventory(Map &map, Player* player, sf::RenderWindow& window) : pgcount(1), selectedItem(-1), shouldDraw(false), fc(true), mapr(map), playerr(player), windowr(window) 
{
    this->font.loadFromFile("../fonts/font.ttf");
    this->bpTex.loadFromFile("../imgs/BigPanel.jpg");
    this->bpSprite.setTexture(this->bpTex);
    this->bpSprite.setPosition(mapr.getPartBounds().width / 2.0f - bpSprite.getGlobalBounds().width / 2.0f, mapr.getPartBounds().height / 2.0f - bpSprite.getGlobalBounds().height / 2.0f);
    this->cellTex.loadFromFile("../imgs/Cell.png");
    for (int i = 0; i < 40; i++)
    {
        int x = i % 5, y = i / 5;
        this->cellSprite[i].setTexture(this->cellTex);
        this->cellSprite[i].setPosition(bpSprite.getPosition().x + 40 + x * cellTex.getSize().x, bpSprite.getPosition().y + 40 + y * cellTex.getSize().y);
    }
    this->selectedSquare.setTexture(this->cellTex);
    this->selectedSquare.setPosition(bpSprite.getPosition().x + 0.75 * mapr.getPartBounds().width - cellTex.getSize().x / 2.0f, bpSprite.getPosition().y + 40);

    this->selectedItemS.setPosition(this->selectedSquare.getPosition());

    this->allItems.push_back(new HB());

    this->text.setFont(this->font);
    this->text.setCharacterSize(12);
    this->text.setPosition(bpSprite.getPosition().x + bpSprite.getGlobalBounds().width/2.0f, selectedSquare.getPosition().y + selectedSquare.getGlobalBounds().height + 40);
}

void Inventory::update()
{
    std::cout << playerr->getBounds().getPosition().x << " " << playerr->getBounds().getPosition().y << std::endl;
    // Take care of item behavior
    for(int i = 0; i < this->allItems.size(); i++){
        if(this->allItems[i]->update(playerr)){
            this->allItems[i]->sprite.setScale(cellSprite[0].getGlobalBounds().width/allItems[i]->sprite.getGlobalBounds().width, cellSprite[0].getGlobalBounds().height/allItems[i]->sprite.getGlobalBounds().height);
            this->ownedItems.push_back(i);
        }
    }

    // Check if e is pressed so you can determine wether the inventory should be drawn or not
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
    {
        if (fc)
        {
            shouldDraw = !shouldDraw;
            fc = false;
        }
    }
    else if (!fc)
    {
        fc = true;
    }

    // Check if the mouse clicked on one of the items so that you can place it in the selected slot
    if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
        auto mp = sf::Mouse::getPosition(windowr);
        for(int i = 0; i < ownedItems.size(); i++){
            if(allItems[ownedItems[i]]->sprite.getGlobalBounds().contains(mp.x, mp.y)){
                selectedItem = ownedItems[i];
                selectedItemS.setTexture(allItems[selectedItem]->texture);
                selectedItemS.setScale(allItems[selectedItem]->sprite.getScale());
                std::string displaytext = allItems[selectedItem]->name + "\n";
                int fds = (allItems[selectedItem]->speedb - 1.0f)*100;
                if(fds > 0){
                    displaytext += std::to_string(fds) + "% speed\n";
                }
                int fdj = (allItems[selectedItem]->jumpb - 1.0f)*100;
                if(fdj > 0){
                    displaytext += std::to_string(fdj) + "% jump\n";
                }
                int fdsi = (allItems[selectedItem]->sizet - 1.0f)*100;
                if(fdsi > 0){
                    displaytext += std::to_string(fdsi) + "% size\n";
                }
                displaytext += allItems[selectedItem]->description;
                text.setString(displaytext);
                break;
            }
        }
    }
}

void Inventory::draw()
{
    // Draw the items that are actually not related to the inventory at all
    for(int i = 0; i < allItems.size(); i++){
        if(!allItems[i]->owned){
            allItems[i]->draw(windowr);
        }
    }

    // Draw the inventory normally
    if (shouldDraw)
    {
        // Save the current view
        sf::View originalView = windowr.getView();

        // Reset the view to the default (window coordinates)
        windowr.setView(windowr.getDefaultView());

        // Draw the big panel and cells of the inventory
        windowr.draw(this->bpSprite);
        for (int i = 0; i < 40; i++)
        {
            windowr.draw(this->cellSprite[i]);
        }
        windowr.draw(selectedSquare);

        // Draw the owned items
        for(int i = 0; i < this->ownedItems.size(); i++){
            allItems[ownedItems[i]]->sprite.setPosition(cellSprite[i].getPosition());
            allItems[ownedItems[i]]->draw(windowr);
        }

        // Draw the selected item and details about it
        if(selectedItem != -1){
            windowr.draw(selectedItemS);
            windowr.draw(this->text);
        }

        // Restore the original view
        windowr.setView(originalView);
    }
}