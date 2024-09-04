#include "../hpp/libs.hpp"
std::unordered_map<std::string, sf::Texture> Map::entityTextures;
Map::EntityMenu::EntityMenu(const std::vector<std::string> &entityPaths, sf::RenderWindow &window)
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
}
std::string Map::EntityMenu::getFileNameWithoutExtension(const std::string &path)
{
    size_t lastSlash = path.find_last_of("/\\");
    size_t lastDot = path.find_last_of(".");
    if (lastSlash == std::string::npos)
        lastSlash = 0;
    else
        lastSlash++;
    if (lastDot == std::string::npos || lastDot < lastSlash)
        lastDot = path.length();
    return path.substr(lastSlash, lastDot - lastSlash);
}
void Map::EntityMenu::draw()
{
    if (isOpen)
    {
        sf::View originalView = window.getView();
        window.setView(window.getDefaultView());

        for (size_t i = 0; i < entityTextures.size(); i++)
        {
            sf::Sprite sprite(entityTextures[i]);
            sprite.setPosition(50.0f + i * 100.0f, 150.0f);
            sprite.setScale(64.0f / entityTextures[i].getSize().x,
                            64.0f / entityTextures[i].getSize().y);
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

void Map::EntityMenu::selectNext()
{
    selectedIndex = (selectedIndex + 1) % entityNames.size();
}

void Map::EntityMenu::selectPrevious()
{
    selectedIndex = (selectedIndex - 1 + entityNames.size()) % entityNames.size();
}

void Map::drawEntityMenu()
{
    entityMenu.draw();
}

bool Map::handleEntityMenuClick(const sf::Vector2i &mousePosition)
{
    if (!entityMenu.isOpen)
        return false;

    for (size_t i = 0; i < entityMenu.entityTextures.size(); ++i)
    {
        sf::FloatRect spriteBounds(50.0f + i * 100.0f, 150.0f, 64, 64);
        if (spriteBounds.contains(mousePosition.x, mousePosition.y))
        {
            entityMenu.selectedIndex = i;
            return true;
        }
    }
    return false;
}

void Map::toggleEntityMenu()
{
    entityMenu.isOpen = !entityMenu.isOpen;
}
const std::string &Map::getSelectedEntityName() const
{
    return entityMenu.entityNames[entityMenu.selectedIndex];
}
void Map::selectNextEntity()
{
    entityMenu.selectNext();
}

void Map::selectPreviousEntity()
{
    entityMenu.selectPrevious();
}

int Map::getSelectedEntityIndex() const
{
    return entityMenu.selectedIndex;
}

bool Map::isEntityMenuOpen() const
{
    return entityMenu.isOpen;
}
std::vector<sf::FloatRect> Map::getEntityBounds()
{
    std::vector<sf::FloatRect> bounds;
    for (const auto &entity : placedEntities)
    {
        bounds.push_back(entity.sprite.getGlobalBounds());
    }
    return bounds;
}
void Map::removeEntity(int x, int y)
{
    sf::Vector2f mousePos(x, y);

    // Remove from placedEntities
    for (auto it = placedEntities.begin(); it != placedEntities.end(); ++it)
    {
        if (it->sprite.getGlobalBounds().contains(mousePos))
        {
            placedEntities.erase(it);
            break;
        }
    }

    // Remove from entities map
    auto &entitiesInCurrentPart = entities[mx][my];
    for (auto it = entitiesInCurrentPart.begin(); it != entitiesInCurrentPart.end(); ++it)
    {
        if (it->second == mousePos)
        {
            entitiesInCurrentPart.erase(it);
            break;
        }
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
    int wx = this->wndref.getSize().x;
    int wy = this->wndref.getSize().y;
    sf::Vector2f position(x + wx * mx, y + wy * my);

    // Create the entity
    std::unique_ptr<Entity> newEntity(EntityFactory::createEntity(entityType, position, *gameOver));

    if (newEntity)
    {
        // Add to entities map (if you still need this)
        entities[mx][my].push_back(std::make_pair(entityType, position));

        // Create sprite
        if (entityTextures.find(entityType) != entityTextures.end())
        {
            sf::Sprite sprite(entityTextures[entityType]);
            sprite.setPosition(position);

            // Add to placedEntities
            placedEntities.push_back(PlacedEntity{sprite, entityType, std::move(newEntity)});
        }
    }
    std::cout << entityTextures.size() << " ";
}
void Map::drawEditorEntities(sf::RenderWindow &window,Map::PlacedEntity *selectedEntity,bool isOpen)
{
    for (const auto &entity : placedEntities)
    {
        sf::Sprite entitySprite = entity.sprite;
        
        // Check if this entity is the selected one and the property editor is open
        if (&entity == selectedEntity&& isOpen)
        {
            // Set the color to pink
            entitySprite.setColor(sf::Color(255, 92, 3, 255));  // Pink color
        }
        else
        {
            // Reset to default color
            entitySprite.setColor(sf::Color::White);
        }
        
        window.draw(entitySprite);
    }
}
Map::TextureMenu::TextureMenu(const std::vector<std::string> &texturePaths, sf::RenderWindow &window)
    : selectedIndex(0), window(window)
{
    for (const auto &path : texturePaths)
    {
        sf::Texture tex;
        if (tex.loadFromFile(path))
        {
            textures.push_back(std::move(tex));
            textureNames.push_back(getFileNameWithoutExtension(path));
        }
    }
    if (!textures.empty())
    {
        selectedTexture.setTexture(textures[selectedIndex]);
    }
    // selectedTexture.setColor(sf::Color::Magenta);
    // selectedTexture.setOutlineThickness(4.0f);
}

void Map::TextureMenu::draw()
{
    if (isOpen)
    {
        // Save the current view
        sf::View originalView = window.getView();

        // Reset the view to the default (window coordinates)
        window.setView(window.getDefaultView());
        for (size_t i = 0; i < textures.size(); i++)
        {
            sf::Sprite sprite(textures[i]);
            sprite.setPosition(50.0f + i * 100.0f, 50.0f);
            sprite.setScale(64.0f / sprite.getTexture()->getSize().x, 64.0f / sprite.getTexture()->getSize().y);
            window.draw(sprite);

            if (i == selectedIndex)
            {
                sf::RectangleShape highlight;
                highlight.setSize(sf::Vector2f(64, 64));
                highlight.setPosition(sprite.getPosition());
                highlight.setFillColor(sf::Color::Transparent);
                highlight.setOutlineColor(sf::Color::Magenta);
                highlight.setOutlineThickness(4.0f);
                window.draw(highlight);
            }
        }
        // Restore the original view
        window.setView(originalView);
    }
}
void Map::TextureMenu::selectNext()
{
    selectedIndex = (selectedIndex + 1) % textures.size();
    selectedTexture.setTexture(textures[selectedIndex]);
}

void Map::TextureMenu::selectPrevious()
{
    selectedIndex = (selectedIndex - 1 + textures.size()) % textures.size();
    selectedTexture.setTexture(textures[selectedIndex]);
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
        lastSlash = 0;
    else
        lastSlash++;
    if (lastDot == std::string::npos || lastDot < lastSlash)
        lastDot = tname.length();
    this->texid = tname.substr(lastSlash, lastDot - lastSlash);
}
const sf::Texture *Map::getSelectedTexture() const
{
    return textureMenu.selectedTexture.getTexture();
}
bool Map::handleTextureMenuClick(const sf::Vector2i &mousePosition)
{
    if (!textureMenu.isOpen)
        return false;

    for (size_t i = 0; i < textureMenu.textures.size(); ++i)
    {
        sf::FloatRect textureBounds(50.0f + i * 100.0f, 50.0f, 64.0f, 64.0f); // Adjust size as needed
        if (textureBounds.contains(mousePosition.x, mousePosition.y))
        {
            textureMenu.selectedIndex = i;
            textureMenu.selectedTexture.setTexture(textureMenu.textures[i]);
            return true;
        }
    }
    return false;
}
bool Map::isTextureMenuOpen() const
{
    return textureMenu.isOpen;
}
void Map::Object::draw(sf::RenderWindow &window)
{
    window.draw(rect);
}
void Map::drawTextureMenu()
{
    textureMenu.draw();
}

void Map::toggleTextureMenu()
{
    textureMenu.isOpen = !textureMenu.isOpen;
}

void Map::selectNextTexture()
{
    textureMenu.selectNext();
}

void Map::selectPreviousTexture()
{
    textureMenu.selectPrevious();
}

int Map::getSelectedTextureIndex() const
{
    return textureMenu.selectedIndex;
}
Map::Map(sf::RenderWindow &wndref, bool &gameover)
    : mx(0), my(0), np(1), wndref(wndref), gameOver(&gameover),
      textureMenu({"../imgs/wow.png", "../imgs/woow.png", "../imgs/wooow.png", "../imgs/woooow.png"}, wndref),
      entityMenu({"../imgs/player.png", "../imgs/pacman.png", "../imgs/capybaraa.png", "../imgs/arrow.png"}, wndref)
{
    this->view.setSize(wndref.getSize().x, wndref.getSize().y);
}

Map::Map(std::string fname, sf::RenderWindow &wndref, bool &gameover)
    : mx(0), my(0), np(1), wndref(wndref), gameOver(&gameover),
      textureMenu({"../imgs/wow.png", "../imgs/woow.png", "../imgs/wooow.png", "../imgs/woooow.png"}, wndref),
      entityMenu({"../imgs/player.png", "../imgs/pacman.png", "../imgs/arrow.png", "../imgs/capybaraa.png"}, wndref)
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
                    placedEntities.push_back(PlacedEntity{sprite, entityType, std::move(newEntity)});
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

void Map::addObject(int x, int y, int w, int h)
{
    std::string textureName = textureMenu.textureNames[textureMenu.selectedIndex];
    int wx = this->wndref.getSize().x, wy = this->wndref.getSize().y;
    this->obj[mx][my].push_back(new Object(x + wx * mx, y + wy * my, w, h, "../imgs/" + textureName + ".png"));
}
void Map::removeObject(int index)
{
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
        int nameLength = entity.type.length();
        file.write((char *)&nameLength, sizeof(int));
        file.write(entity.type.c_str(), nameLength);

        // Save entity position
        file.write((char *)&entity.sprite.getPosition().x, sizeof(float));
        file.write((char *)&entity.sprite.getPosition().y, sizeof(float));

        // Save entity properties
        auto properties = entity.entity->getEditableProperties();
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
        placedEntities.erase(placedEntities.begin() + index);
    }
}
void Map::resetEntities(sf::FloatRect &playerBounds)
{
    activeEntities.clear();
    spawnEntities(playerBounds);
    std::cout << activeEntities.size() << "\n";
}

void Map::spawnEntities(sf::FloatRect &playerBounds)
{
    std::cout << "aaa\n";
    for (const auto &placedEntity : placedEntities)
    {
        const sf::Vector2f position = placedEntity.sprite.getPosition();
        const std::string &type = placedEntity.type;

        // Create a new instance of the entity
        std::unique_ptr<Entity> entity(EntityFactory::createEntity(type, position, *gameOver));

        if (entity)
        {
            entity->playerBounds = &playerBounds;

            // Copy the properties from the placed entity to the new instance
            auto properties = placedEntity.entity->getEditableProperties();
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
        return;
    auto properties = selectedEntity->entity->getEditableProperties();
    for (size_t i = 0; i < properties.size() && i < inputTexts.size(); ++i)
    {
        selectedEntity->entity->setProperty(properties[i].first, inputTexts[i].getString());
    }
    std::cout << "Changes applied to entity properties." << std::endl;
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
                // Handle backspace
                if (!currentText.empty())
                    currentText.pop_back();
            }
            else
            {
                // Append the character
                currentText += static_cast<char>(event.text.unicode);
            }
            inputTexts[selectedInputBox].setString(currentText);

            // Debug: Print to console
            std::cout << "Input received. Current text: " << currentText << std::endl;
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

    if (!entity)
        return;

    auto properties = entity->entity->getEditableProperties();
    float yOffset = 10;
    for (const auto &prop : properties)
    {
        sf::Text label(prop.first + ":", font, 14);
        label.setPosition(834, yOffset);
        labels.push_back(label);

        sf::RectangleShape inputBox(sf::Vector2f(180, 20));
        inputBox.setFillColor(sf::Color::White);
        inputBox.setOutlineColor(sf::Color::Black);
        inputBox.setOutlineThickness(1);
        inputBox.setPosition(834, yOffset + 20);
        inputBoxes.push_back(inputBox);

        sf::Text inputText(prop.second, font, 14);
        inputText.setPosition(838, yOffset + 22);
        inputText.setFillColor(sf::Color::Black); // Ensure text color is visible
        inputTexts.push_back(inputText);

        yOffset += 50;
    }

    isOpen = true; // Ensure property editor is open when updating for an entity
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
    for (const auto &text : inputTexts)
        window.draw(text);

    // Debug: Draw selected input box indicator
    if (selectedInputBox >= 0 && selectedInputBox < inputBoxes.size())
    {
        sf::RectangleShape highlight = inputBoxes[selectedInputBox];
        highlight.setFillColor(sf::Color::Transparent);
        highlight.setOutlineColor(sf::Color::Red);
        highlight.setOutlineThickness(2);
        window.draw(highlight);
    }

    // Debug: Draw text showing which box is selected and its content
    sf::Text debugText("Selected: " + std::to_string(selectedInputBox), *font, 12);
    debugText.setPosition(834, 380);
    debugText.setFillColor(sf::Color::Red);
    window.draw(debugText);

    if (selectedInputBox >= 0 && selectedInputBox < inputTexts.size())
    {
        sf::Text contentText("Content: " + inputTexts[selectedInputBox].getString(), *font, 12);
        contentText.setPosition(834, 395);
        contentText.setFillColor(sf::Color::Red);
        window.draw(contentText);
    }
}