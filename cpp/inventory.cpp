#include "../hpp/libs.hpp"

Inventory::Inventory(Map &map, Player *player, sf::RenderWindow &window) : pgcount(1), selectedItem(-1), shouldDraw(false), fc(true), mapr(map), playerr(player), windowr(window), movingItem(false), active{nullptr, nullptr, nullptr}
{
    loadPanel();
    loadItems();
}

void Inventory::loadPanel()
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

    this->text.setFont(this->font);
    this->text.setCharacterSize(12);
    this->text.setPosition(bpSprite.getPosition().x + bpSprite.getGlobalBounds().width / 2.0f, selectedSquare.getPosition().y + selectedSquare.getGlobalBounds().height + 40);

    for (int i = 0; i < 3; i++)
    {
        this->activeCellS[i].setTexture(this->cellTex);
        this->activeCellS[i].setPosition(cellSprite[35].getPosition().x + i * activeCellS->getGlobalBounds().width, cellSprite[39].getPosition().y + cellSprite[39].getGlobalBounds().height + 40);
    }
}

void Inventory::loadItems()
{
    this->allItems.push_back(new HB(790, 500));
    this->allItems.push_back(new RP(750, 500));
    this->allItems.push_back(new GB(830, 500));
    this->allItems.push_back(new CTP(890,500));

    for (int i = 0; i < allItems.size(); i++)
    {
        unownedItems.push_back(i);
    }
}

void Inventory::selectItem(int i)
{
    selectedItem = ownedItems[i];
    selectedItemS.setTexture(allItems[selectedItem]->texture, true);
    selectedItemS.setScale(allItems[selectedItem]->sprite.getScale());
    std::string displaytext = allItems[selectedItem]->name + "\n";
    int fds = std::round((allItems[selectedItem]->speedb - 1.0f) * 100);
    if (fds != 0)
    {
        displaytext += std::to_string(fds) + "% speed\n";
    }
    int fdj = std::round((allItems[selectedItem]->jumpb - 1.0f) * 100);
    if (fdj != 0)
    {
        displaytext += std::to_string(fdj) + "% jump\n";
    }
    int fdsi = std::round((allItems[selectedItem]->sizet - 1.0f) * 100);
    if (fdsi != 0)
    {
        displaytext += std::to_string(fdsi) + "% size\n";
    }
    displaytext += allItems[selectedItem]->description + "\n" + allItems[selectedItem]->customText();
    text.setString(displaytext);
}

int Inventory::getHoverCell()
{
    sf::IntRect zone(cellSprite[0].getPosition().x, cellSprite[0].getPosition().y, cellSprite[0].getGlobalBounds().width * 5, cellSprite[0].getGlobalBounds().height * 8);
    sf::Vector2i mp = sf::Mouse::getPosition(windowr);
    if (zone.contains(mp))
    {
        mp -= zone.getPosition();
        int hcx = float(mp.x) / cellSprite[0].getGlobalBounds().width;
        int hcy = float(mp.y) / cellSprite[0].getGlobalBounds().height;
        return hcy * 5 + hcx;
    }
    return -1;
}

int Inventory::getActiveHoverCell()
{
    sf::IntRect zone(activeCellS[0].getPosition().x, activeCellS[0].getPosition().y, activeCellS[0].getGlobalBounds().width * 3, activeCellS[0].getGlobalBounds().height);
    sf::Vector2i mp = sf::Mouse::getPosition(windowr);
    if (zone.contains(mp))
    {
        mp -= zone.getPosition();
        return int(float(mp.x) / activeCellS[0].getGlobalBounds().width);
    }
    return -1;
}

void Inventory::update()
{
    // Take care of item behavior
    for (int j = 0; j < this->unownedItems.size(); j++)
    {
        int i = unownedItems[j];
        if (this->allItems[i]->updateU(playerr))
        {
            this->allItems[i]->sprite.setPosition(cellSprite[ownedItems.size()].getPosition());
            this->allItems[i]->sprite.setScale(cellSprite[0].getGlobalBounds().width / allItems[i]->sprite.getGlobalBounds().width, cellSprite[0].getGlobalBounds().height / allItems[i]->sprite.getGlobalBounds().height);
            this->ownedItems.push_back(i);
            this->unownedItems.erase(unownedItems.begin() + j);
            break;
        }
    }
    for (int j = 0; j < this->ownedItems.size(); j++)
    {
        int i = ownedItems[j];
        this->allItems[i]->updateO(playerr);
    }

    // Check if e is pressed so you can determine wether the inventory should be drawn or not
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E) && !movingItem)
    {
        if (fc)
        {
            shouldDraw = !shouldDraw;
            if (!shouldDraw)
                selectedItem = -1;
            fc = false;
        }
    }
    else if (!fc)
    {
        fc = true;
    }

    if(shouldDraw){
        // Check if the mouse clicked on one of the items so that you can place it in the selected slot
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            int i = getHoverCell();
            if (i != -1 && i < ownedItems.size())
            {
                selectItem(i);
            }
        }

        // Check if the right mouse button is clicked so you can place it in the active items section
        if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
        {
            int i = getHoverCell();
            sf::Vector2i mp = sf::Mouse::getPosition(windowr);
            if (movingItem)
            {
                allItems[ownedItems[movedItem]]->sprite.setPosition(sf::Vector2f(mp) - cellSprite[movedItem].getGlobalBounds().getSize() / 2.0f);
            }
            else if (i != -1 && i <= ownedItems.size())
            {
                movedItem = i;
                movingItem = true;
            }
        }
        else if (movingItem)
        {
            int ahovcell = getActiveHoverCell();
            if(ahovcell != -1 && dynamic_cast<Item::Active*>(allItems[ownedItems[movedItem]])){
                allItems[ownedItems[movedItem]]->sprite.setPosition(activeCellS[ahovcell].getPosition());
                active[ahovcell] = dynamic_cast<Item::Active*>(allItems[ownedItems[movedItem]]);
            }
            else{
                allItems[ownedItems[movedItem]]->sprite.setPosition(cellSprite[movedItem].getPosition());
            }
            movingItem = false;
        }
    }

    // Check for active item behaviour
    if(active[0] != nullptr && sf::Keyboard::isKeyPressed(sf::Keyboard::Z)){
        active[0]->activate();
    }
    if(active[1] != nullptr && sf::Keyboard::isKeyPressed(sf::Keyboard::X)){
        active[1]->activate();
    }
    if(active[2] != nullptr && sf::Keyboard::isKeyPressed(sf::Keyboard::C)){
        active[2]->activate();
    }
}

void Inventory::draw()
{
    // Draw the items that are actually not related to the inventory at all
    for (int i = 0; i < unownedItems.size(); i++)
    {
        allItems[unownedItems[i]]->draw(windowr);
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
        for (int i = 0; i < 3; i++)
        {
            windowr.draw(this->activeCellS[i]);
        }

        // Draw the owned items
        for (int i = 0; i < this->ownedItems.size(); i++)
        {
            allItems[ownedItems[i]]->draw(windowr);
        }

        // Draw the selected item and details about it
        if (selectedItem != -1)
        {
            windowr.draw(selectedItemS);
            windowr.draw(this->text);
        }

        // Restore the original view
        windowr.setView(originalView);
    }
}
void Inventory::reset(Player *player)
{
    this->playerr = player;
    // Reset other members as needed
    this->pgcount = 1;
    this->selectedItem = -1;
    this->shouldDraw = false;
    this->fc = true;
    this->ownedItems.clear();
    this->unownedItems.clear();
    for (int i = 0; i < allItems.size(); i++)
    {
        unownedItems.push_back(i);
    }
}