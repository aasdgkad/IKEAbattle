#include "../hpp/libs.hpp"
std::unordered_map<std::string, sf::Texture> Map::entityTextures;

std::vector<sf::FloatRect> Map::getEntityBounds()
{
    std::vector<sf::FloatRect> bounds;
    for (const auto &entity : placedEntities)
    {
        bounds.push_back(entity->sprite.getGlobalBounds());
    }
    return bounds;
}
void Map::removeEntity(int x, int y)
{
    sf::Vector2f mousePos(x, y);

    // Remove from placedEntities
    auto it = std::find_if(placedEntities.begin(), placedEntities.end(),
                           [&mousePos](const std::unique_ptr<PlacedEntity> &entity)
                           {
                               return entity->sprite.getGlobalBounds().contains(mousePos);
                           });

    if (it != placedEntities.end())
    {
        std::cout << "\n"
                  << (*it)->sprite.getPosition().x
                  << " " << (*it)->sprite.getPosition().y
                  << " " << (*it)->type << " deleted";
        placedEntities.erase(it);
    }
}
const sf::Texture *Map::getEntityTexture(const std::string &entityName) const
{
    auto it = entityTextures.find(entityName);
    if (it != entityTextures.end())
    {
        return &(it->second);
    }
    return nullptr;
}
void Map::addEntity(int x, int y, const std::string &entityType)
{
    std::cout << "\n"
              << x << " " << y << " " << entityType;
    int wx = this->wndref.getSize().x;
    int wy = this->wndref.getSize().y;
    sf::Vector2f position(x, y);

    // Create the entity
    std::unique_ptr<Entity> newEntity(EntityFactory::createEntity(entityType, position, *gameOver));

    if (newEntity)
    {
        // Create sprite
        if (entityTextures.find(entityType) != entityTextures.end())
        {
            sf::Sprite sprite(entityTextures[entityType]);
            sprite.setPosition(position);

            // Add to placedEntities
            placedEntities.push_back(std::make_unique<PlacedEntity>(PlacedEntity{
                std::move(sprite),
                entityType,
                std::move(newEntity)}));
        }
    }
}
void Map::drawEditorEntities(sf::RenderWindow &window, const PlacedEntity *selectedEntity, bool &isOpen)
{
    for (const auto &entityPtr : placedEntities)
    {
        sf::Sprite entitySprite = entityPtr->sprite;

        // Check if this entity is the selected one and the property editor is open
        if (entityPtr.get() == selectedEntity && isOpen)
        {
            // Set the color to pink
            entitySprite.setColor(sf::Color(255, 92, 3, 255)); // Pink color
        }
        else
        {
            // Reset to default color
            entitySprite.setColor(sf::Color::White);
        }

        window.draw(entitySprite);
    }
}

Map::Object::Object(int x, int y, int w, int h, std::string tname) : rect(sf::Vector2f(w, h))
{
    assert(this->tex.loadFromFile(tname));
    this->tex.setRepeated(true);

    this->rect.setTexture(&this->tex);
    this->rect.setTextureRect(sf::IntRect(0, 0, w, h));
    this->rect.setPosition(x, y);

    // Extract filename without path and extension
    size_t lastSlash = tname.find_last_of("/\\");
    size_t lastDot = tname.find_last_of(".");
    if (lastSlash == std::string::npos)
    {
        lastSlash = 0;
    }
    else
    {
        lastSlash++;
    }
    if (lastDot == std::string::npos || lastDot < lastSlash)
    {
        lastDot = tname.length();
    }
    this->texid = tname.substr(lastSlash, lastDot - lastSlash);
}

void Map::Object::draw(sf::RenderWindow &window)
{
    window.draw(rect);
}

Map::Map(sf::RenderWindow &wndref, bool &gameover)
    : mx(0), my(0), np(1), wndref(wndref), gameOver(&gameover),
      menu({"../imgs/player.png", "../imgs/pacman.png", "../imgs/capybaraa.png", "../imgs/arrow.png", "../imgs/pengu.png"},
           {"../imgs/wow.png", "../imgs/woow.png", "../imgs/wooow.png", "../imgs/woooow.png"},
           wndref)
{
    this->view.setSize(wndref.getSize().x, wndref.getSize().y);
}

Map::Map(std::string fname, sf::RenderWindow &wndref, bool &gameover)
    : mx(0), my(0), np(1), wndref(wndref), gameOver(&gameover),
      menu({"../imgs/player.png", "../imgs/pacman.png", "../imgs/capybaraa.png", "../imgs/arrow.png", "../imgs/pengu.png"},
           {"../imgs/wow.png", "../imgs/woow.png", "../imgs/wooow.png", "../imgs/woooow.png"},
           wndref)
{
    this->view.setSize(wndref.getSize().x, wndref.getSize().y);
    if (std::filesystem::exists(fname))
    {
        std::ifstream file(fname, std::ios::binary);
        int size;
        file.read((char *)&size, sizeof(int));
        this->np = size;

        // Load objects
        for (int j = 0; j < size; j++)
        {
            int cmx, cmy, ssize;
            file.read((char *)&cmx, sizeof(int));
            file.read((char *)&cmy, sizeof(int));
            file.read((char *)&ssize, sizeof(int));
            for (int i = 0; i < ssize; i++)
            {
                int cx, cy, cw, ch, nameLength;
                file.read((char *)&cx, sizeof(int));
                file.read((char *)&cy, sizeof(int));
                file.read((char *)&cw, sizeof(int));
                file.read((char *)&ch, sizeof(int));
                file.read((char *)&nameLength, sizeof(int));
                std::string texName(nameLength, '\0');
                file.read(&texName[0], nameLength);
                this->obj[cmx][cmy].push_back(new Object(cx, cy, cw, ch, "../imgs/" + texName + ".png"));
            }
        }

        // Load entities
        int entityCount;
        file.read((char *)&entityCount, sizeof(int));
        for (int i = 0; i < entityCount; ++i)
        {
            int nameLength;
            file.read((char *)&nameLength, sizeof(int));

            std::string entityType(nameLength, '\0');
            file.read(&entityType[0], nameLength);

            float x, y;
            file.read((char *)&x, sizeof(float));
            file.read((char *)&y, sizeof(float));

            // Create the entity
            std::unique_ptr<Entity> newEntity(EntityFactory::createEntity(entityType, sf::Vector2f(x, y), *gameOver));

            if (newEntity)
            {
                // Load entity properties
                int propertyCount;
                file.read((char *)&propertyCount, sizeof(int));
                for (int j = 0; j < propertyCount; ++j)
                {
                    // Load property name
                    file.read((char *)&nameLength, sizeof(int));
                    std::string propName(nameLength, '\0');
                    file.read(&propName[0], nameLength);

                    // Load property value
                    int valueLength;
                    file.read((char *)&valueLength, sizeof(int));
                    std::string propValue(valueLength, '\0');
                    file.read(&propValue[0], valueLength);

                    // Set the property
                    newEntity->setProperty(propName, propValue);
                }

                // Create sprite
                if (entityTextures.find(entityType) != entityTextures.end())
                {
                    sf::Sprite sprite(entityTextures[entityType]);
                    sprite.setPosition(x, y);

                    // Add to placedEntities
                    placedEntities.push_back(std::make_unique<PlacedEntity>(PlacedEntity{sprite, entityType, std::move(newEntity)}));
                }
            }
        }
    }
}
void Map::draw()
{
    for (int i = 0; i < this->obj[mx][my].size(); i++)
    {
        obj[mx][my][i]->draw(this->wndref);
    }
}
const std::string &Map::getSelectedName() const
{
    if (menu.isEntitySelected())
    {
        return menu.entityNames[menu.selectedIndex];
    }
    else
    {
        return menu.textureNames[menu.selectedIndex - menu.entityTextures.size()];
    }
}
void Map::addObject(int x, int y, int w, int h)
{
    if (menu.isEntitySelected())
    {
        // If an entity is selected, we don't add an object
        std::cout << "\nCannot add object when an entity is selected.";
        return;
    }

    const std::string &textureName = menu.getSelectedName();
    int wx = this->wndref.getSize().x, wy = this->wndref.getSize().y;
    std::cout << "\n"
              << x + wx * mx << " " << y + wy * my << " " << textureName;
    this->obj[mx][my].push_back(new Object(x + wx * mx, y + wy * my, w, h, "../imgs/" + textureName + ".png"));
}
void Map::removeObject(int index)
{
    std::cout << "\n"
              << this->obj[mx][my].at(index)->rect.getPosition().x << " " << this->obj[mx][my].at(index)->rect.getPosition().y << " " << this->obj[mx][my].at(index)->texid << " deleted";
    this->obj[mx][my].erase(this->obj[mx][my].begin() + index);
}

void Map::saveToFile(std::string fname)
{
    if (std::filesystem::exists(fname))
    {
        std::filesystem::remove(fname);
    }
    std::ofstream file(fname, std::ios::binary);
    int size = this->np;
    file.write((char *)&size, sizeof(int));

    // Save objects
    for (const auto &x : this->obj)
    {
        for (const auto &y : x.second)
        {
            int ssize = y.second.size();
            file.write((char *)&x.first, sizeof(int));
            file.write((char *)&y.first, sizeof(int));
            file.write((char *)&ssize, sizeof(int));
            for (int i = 0; i < ssize; i++)
            {
                int cx = this->obj[x.first][y.first][i]->rect.getPosition().x;
                int cy = this->obj[x.first][y.first][i]->rect.getPosition().y;
                int cw = this->obj[x.first][y.first][i]->rect.getSize().x;
                int ch = this->obj[x.first][y.first][i]->rect.getSize().y;
                file.write((char *)&cx, sizeof(int));
                file.write((char *)&cy, sizeof(int));
                file.write((char *)&cw, sizeof(int));
                file.write((char *)&ch, sizeof(int));

                int nameLength = this->obj[x.first][y.first][i]->texid.length();
                file.write((char *)&nameLength, sizeof(int));
                file.write(this->obj[x.first][y.first][i]->texid.c_str(), nameLength);
            }
        }
    }

    // Save entities
    int entityCount = this->placedEntities.size();
    file.write((char *)&entityCount, sizeof(int));
    for (const auto &entity : this->placedEntities)
    {
        // Save entity type
        int nameLength = entity->type.length();
        file.write((char *)&nameLength, sizeof(int));
        file.write(entity->type.c_str(), nameLength);

        // Save entity position
        file.write((char *)&entity->sprite.getPosition().x, sizeof(float));
        file.write((char *)&entity->sprite.getPosition().y, sizeof(float));

        // Save entity properties
        auto properties = entity->entity->getEditableProperties();
        int propertyCount = properties.size();
        file.write((char *)&propertyCount, sizeof(int));
        for (const auto &prop : properties)
        {
            // Save property name
            nameLength = prop.first.length();
            file.write((char *)&nameLength, sizeof(int));
            file.write(prop.first.c_str(), nameLength);

            // Save property value
            int valueLength = prop.second.length();
            file.write((char *)&valueLength, sizeof(int));
            file.write(prop.second.c_str(), valueLength);
        }
    }
}

void Map::changePart(int x, int y)
{
    this->mx += x;
    this->my += y;
    if (this->obj[mx][my].size() == 0)
        np++;
    int wx = this->wndref.getSize().x, wy = this->wndref.getSize().y;
    this->view.setCenter(wx / 2 + wx * this->mx, wy / 2 + wy * this->my);
    this->wndref.setView(view);
}

sf::FloatRect Map::getPartBounds()
{
    sf::FloatRect bounds;
    int wx = this->wndref.getSize().x, wy = this->wndref.getSize().y;
    bounds.left = this->mx * wx;
    bounds.width = wx;
    bounds.top = this->my * wy;
    bounds.height = wy;
    return bounds;
}

Map::~Map()
{
    for (const auto &x : this->obj)
    {
        for (const auto &y : x.second)
        {
            for (int i = 0; i < y.second.size(); i++)
            {
                delete y.second[i];
            }
        }
    }
}

std::vector<sf::FloatRect> Map::getObjectBounds()
{
    std::vector<sf::FloatRect> bounds;
    for (int i = 0; i < obj[mx][my].size(); i++)
    {
        bounds.push_back(this->obj[mx][my][i]->rect.getGlobalBounds());
    }
    return bounds;
}

void Map::removeEntity(int index)
{
    if (index >= 0 && index < placedEntities.size())
    {
        std::cout << "\n"
                  << placedEntities[index]->sprite.getPosition().x
                  << " " << placedEntities[index]->sprite.getPosition().y
                  << " " << placedEntities[index]->type << " deleted";
        placedEntities.erase(placedEntities.begin() + index);
    }
}
void Map::resetEntities(sf::FloatRect &playerBounds)
{
    activeEntities.clear();
    spawnEntities(playerBounds);
}

void Map::spawnEntities(sf::FloatRect &playerBounds)
{
    for (const auto &placedEntity : placedEntities)
    {
        const sf::Vector2f position = placedEntity->sprite.getPosition();
        const std::string &type = placedEntity->type;

        // Create a new instance of the entity
        std::unique_ptr<Entity> entity(EntityFactory::createEntity(type, position, *gameOver));

        if (entity)
        {
            entity->playerBounds = &playerBounds;

            // Copy the properties from the placed entity to the new instance
            auto properties = placedEntity->entity->getEditableProperties();
            for (const auto &prop : properties)
            {
                entity->setProperty(prop.first, prop.second);
            }
            activeEntities.push_back(entity.release()); // Transfer ownership to activeEntities
        }
    }
}
void Map::updateEntities(float deltaTime, const sf::Vector2u &windowSize)
{
    for (auto &entity : activeEntities)
    {
        entity->update(deltaTime, *this, windowSize);
    }
    removeDeadEntities();
}
void Map::drawEntities(sf::RenderWindow &window)
{
    for (auto &entity : activeEntities)
    {
        entity->draw(window);
    }
}

void Map::removeDeadEntities()
{
    activeEntities.erase(
        std::remove_if(activeEntities.begin(), activeEntities.end(),
                       [](Entity *entity)
                       {
                           if (entity->shouldBeDead)
                           {
                               delete entity;
                               return true;
                           }
                           return false;
                       }),
        activeEntities.end());
}
void Map::PropertyEditor::applyChanges()
{
    if (!selectedEntity)
    {
        return;
    }
    std::cout << "\nsaved some changed things";
    auto properties = selectedEntity->entity->getEditableProperties();
    for (size_t i = 0; i < properties.size() && i < inputTexts.size(); ++i)
    {
        selectedEntity->entity->setProperty(properties[i].first, inputTexts[i].getString());
    }
}
void Map::PropertyEditor::handleInput(sf::Event &event, sf::RenderWindow &window)
{
    if (!isOpen || !selectedEntity)
        return;

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        for (size_t i = 0; i < inputBoxes.size(); ++i)
        {
            if (inputBoxes[i].getGlobalBounds().contains(mousePos.x, mousePos.y))
            {
                selectedInputBox = i;
                return;
            }
        }
        selectedInputBox = -1;
    }
    else if (event.type == sf::Event::TextEntered && selectedInputBox != -1)
    {
        if (event.text.unicode < 128)
        {
            std::string currentText = inputTexts[selectedInputBox].getString();
            if (event.text.unicode == '\b')
            {
                if (!currentText.empty())
                    currentText.pop_back();
            }
            else
            {
                currentText += static_cast<char>(event.text.unicode);
            }
            inputTexts[selectedInputBox].setString(currentText);

            // Recalculate the height of the input box and adjust layout
            float requiredHeight = calculateRequiredHeight(inputTexts[selectedInputBox], 180);
            inputBoxes[selectedInputBox].setSize(sf::Vector2f(180, requiredHeight));
            adjustLayout();
        }
    }
}
void Map::PropertyEditor::setup(sf::Font &loadedFont)
{
    font = &loadedFont; // Store the font
    background.setFillColor(sf::Color(200, 200, 200));
    background.setSize(sf::Vector2f(200, 400));
    background.setPosition(824, 0);
    selectedInputBox = -1;
}
void Map::PropertyEditor::updateForEntity(Map::PlacedEntity *entity, sf::Font &font)
{
    selectedEntity = entity;
    labels.clear();
    inputBoxes.clear();
    inputTexts.clear();
    selectedInputBox = -1;
    // std::cout<<"ffffff";
    if (!entity)
        return;

    auto properties = entity->entity->getEditableProperties();
    float yOffset = 10;
    for (const auto &prop : properties)
    {
        sf::Text label(prop.first + ":", font, 14);
        label.setPosition(834, yOffset);
        labels.push_back(label);
        std::cout << (std::string)label.getString();

        sf::Text inputText(prop.second, font, 14);
        inputText.setPosition(838, yOffset + 22);
        inputText.setFillColor(sf::Color::Black);

        // Calculate the required height for the input box
        float requiredHeight = calculateRequiredHeight(inputText, 180);

        sf::RectangleShape inputBox(sf::Vector2f(180, requiredHeight));
        inputBox.setFillColor(sf::Color::White);
        inputBox.setOutlineColor(sf::Color::Black);
        inputBox.setOutlineThickness(1);
        inputBox.setPosition(834, yOffset + 20);

        inputBoxes.push_back(inputBox);
        inputTexts.push_back(inputText);

        yOffset += 30 + requiredHeight;
    }

    isOpen = true;
    adjustBackgroundSize();
}

void Map::PropertyEditor::draw(sf::RenderWindow &window)
{
    if (!selectedEntity)
    {
        isOpen = false;
        return;
    }
    isOpen = true;
    window.draw(background);
    for (const auto &label : labels)
        window.draw(label);
    for (const auto &box : inputBoxes)
        window.draw(box);
    for (auto &text : inputTexts)
    {
        wrapText(text, 170); // Reduced width to account for padding
        window.draw(text);
    }

    if (selectedInputBox >= 0 && selectedInputBox < inputBoxes.size())
    {
        sf::RectangleShape highlight = inputBoxes[selectedInputBox];
        highlight.setFillColor(sf::Color::Transparent);
        highlight.setOutlineColor(sf::Color::Red);
        highlight.setOutlineThickness(2);
        window.draw(highlight);
    }
}
void Map::PropertyEditor::wrapText(sf::Text &text, float maxWidth)
{
    const std::string &words = text.getString();
    std::string wrappedText;
    std::string line;
    sf::Text testText = text;

    for (char c : words)
    {
        if (c == '\n')
        {
            wrappedText += line + '\n';
            line.clear();
        }
        else
        {
            testText.setString(line + c);
            if (testText.getLocalBounds().width > maxWidth && !line.empty())
            {
                wrappedText += line + '\n';
                line = c;
            }
            else
            {
                line += c;
            }
        }
    }
    wrappedText += line;
    text.setString(wrappedText);
}
float Map::PropertyEditor::calculateRequiredHeight(const sf::Text &text, float maxWidth)
{
    sf::Text tempText = text;
    wrapText(tempText, maxWidth);
    return tempText.getLocalBounds().height + 20; // Increased padding
}
void Map::PropertyEditor::adjustLayout()
{
    float yOffset = 10;
    for (size_t i = 0; i < inputBoxes.size(); ++i)
    {
        labels[i].setPosition(834, yOffset);
        inputBoxes[i].setPosition(834, yOffset + 20);
        inputTexts[i].setPosition(838, yOffset + 22);

        yOffset += 30 + inputBoxes[i].getSize().y;
    }
    adjustBackgroundSize();
}
void Map::PropertyEditor::adjustBackgroundSize()
{
    if (inputBoxes.empty())
        return;

    float lastBoxBottom = inputBoxes.back().getPosition().y + inputBoxes.back().getSize().y;
    background.setSize(sf::Vector2f(200, lastBoxBottom + 20)); // Add some padding at the bottom
}
Map::Menu::Menu(const std::vector<std::string> &entityPaths, const std::vector<std::string> &texturePaths, sf::RenderWindow &window)
    : selectedIndex(0), window(window)
{
    for (const auto &path : entityPaths)
    {
        sf::Texture texture;
        if (texture.loadFromFile(path))
        {
            entityTextures.push_back(texture);
            entityNames.push_back(getFileNameWithoutExtension(path));
            if (Map::entityTextures.find(getFileNameWithoutExtension(path)) == Map::entityTextures.end())
            {
                Map::entityTextures[getFileNameWithoutExtension(path)] = texture;
            }
        }
    }

    for (const auto &path : texturePaths)
    {
        sf::Texture tex;
        if (tex.loadFromFile(path))
        {
            textures.push_back(std::move(tex));
            textureNames.push_back(getFileNameWithoutExtension(path));
        }
    }
}

void Map::Menu::draw()
{
    if (isOpen)
    {
        sf::View originalView = window.getView();
        window.setView(window.getDefaultView());

        size_t totalItems = entityTextures.size() + textures.size();
        for (size_t i = 0; i < totalItems; i++)
        {
            sf::Sprite sprite;
            if (i < entityTextures.size())
            {
                sprite.setTexture(entityTextures[i]);
            }
            else
            {
                sprite.setTexture(textures[i - entityTextures.size()]);
            }
            sprite.setPosition(50.0f + i * 100.0f, 50.0f);
            sprite.setScale(64.0f / sprite.getTexture()->getSize().x, 64.0f / sprite.getTexture()->getSize().y);
            window.draw(sprite);

            if (i == selectedIndex)
            {
                sf::RectangleShape highlight;
                highlight.setSize(sf::Vector2f(64, 64));
                highlight.setPosition(sprite.getPosition());
                highlight.setFillColor(sf::Color::Transparent);
                highlight.setOutlineColor(sf::Color::Yellow);
                highlight.setOutlineThickness(2.0f);
                window.draw(highlight);
            }
        }

        window.setView(originalView);
    }
}

void Map::Menu::selectNext()
{
    selectedIndex = (selectedIndex + 1) % (entityTextures.size() + textures.size());
}

void Map::Menu::selectPrevious()
{
    selectedIndex = (selectedIndex - 1 + entityTextures.size() + textures.size()) % (entityTextures.size() + textures.size());
}

bool Map::handleMenuClick(const sf::Vector2i &mousePosition)
{
    if (!menu.isOpen)
        return false;

    for (size_t i = 0; i < menu.entityTextures.size() + menu.textures.size(); ++i)
    {
        sf::FloatRect itemBounds(50.0f + i * 100.0f, 50.0f, 64.0f, 64.0f);
        if (itemBounds.contains(mousePosition.x, mousePosition.y))
        {
            menu.selectedIndex = i;
            return true;
        }
    }
    return false;
}

const sf::Texture *Map::getSelectedTexture() const
{
    if (menu.isEntitySelected())
    {
        return &menu.entityTextures[menu.selectedIndex];
    }
    else
    {
        return &menu.textures[menu.selectedIndex - menu.entityTextures.size()];
    }
}
