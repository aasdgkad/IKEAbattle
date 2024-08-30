#include "../hpp/libs.hpp"
Map::EntityMenu::EntityMenu(const std::vector<std::string>& entityPaths, sf::RenderWindow& window)
    : selectedIndex(0), window(window)
{
    for (const auto& path : entityPaths)
    {
        sf::Texture texture;
        if (texture.loadFromFile(path))
        {
            entityTextures.push_back(texture);
            entityNames.push_back(getFileNameWithoutExtension(path));
        }
    }
}
std::string Map::EntityMenu::getFileNameWithoutExtension(const std::string& path)
{
    size_t lastSlash = path.find_last_of("/\\");
    size_t lastDot = path.find_last_of(".");
    if (lastSlash == std::string::npos) lastSlash = 0;
    else lastSlash++;
    if (lastDot == std::string::npos || lastDot < lastSlash) lastDot = path.length();
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

// New Map methods implementation
void Map::drawEntityMenu()
{
    entityMenu.draw();
}

bool Map::handleEntityMenuClick(const sf::Vector2i& mousePosition)
{
    if (!entityMenu.isOpen) return false;

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
const std::string& Map::getSelectedEntityName() const 
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
    for (const auto& entity : placedEntities)
    {
        bounds.push_back(entity.first.getGlobalBounds());
    }
    return bounds;
}
void Map::removeEntity(int x, int y)
{
    sf::Vector2f mousePos(x, y);
    
    // Remove from placedEntities
    for (auto it = placedEntities.begin(); it != placedEntities.end(); ++it)
    {
        if (it->first.getGlobalBounds().contains(mousePos))
        {
            placedEntities.erase(it);
            break;
        }
    }
    
    // Remove from entities map
    auto& entitiesInCurrentPart = entities[mx][my];
    for (auto it = entitiesInCurrentPart.begin(); it != entitiesInCurrentPart.end(); ++it)
    {
        if (it->second == mousePos)
        {
            entitiesInCurrentPart.erase(it);
            break;
        }
    }
}
const sf::Texture* Map::getEntityTexture(const std::string& entityName) const
{
    auto it = entityTextures.find(entityName);
    if (it != entityTextures.end()) {
        return &(it->second);
    }
    return nullptr;
}
void Map::addEntity(int x, int y, const std::string& entityType)
{
        int wx = this->wndref.getSize().x;
        int wy = this->wndref.getSize().y;
    entities[mx][my].push_back(std::make_pair(entityType, sf::Vector2f(x + wx*mx, y+ wy*my)));
    
    // Create and place the entity sprite
    if (entityTextures.find(entityType) == entityTextures.end()) {
        // Load the texture if it's not already loaded
        sf::Texture texture;
        if (texture.loadFromFile("../imgs/" + entityType + ".png")) {
            entityTextures[entityType] = texture;
        }
    }
    
    if (entityTextures.find(entityType) != entityTextures.end()) {
        sf::Sprite sprite(entityTextures[entityType]);
        sprite.setPosition(x + wx*mx, y+ wy*my);
        placedEntities.push_back(std::make_pair(sprite, entityType));
    }
}
void Map::drawEditorEntities(sf::RenderWindow& window)
{
    for (const auto& entity : placedEntities) {
        window.draw(entity.first);
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
Map::Map(sf::RenderWindow &wndref)
    : mx(0), my(0), np(1), wndref(wndref),
      textureMenu({"../imgs/wow.png", "../imgs/woow.png", "../imgs/wooow.png", "../imgs/woooow.png"}, wndref),
      entityMenu({"../imgs/player.png", "../imgs/pacman.png", "../imgs/capybaraa.png", "../imgs/arrow.png"}, wndref)
{
    this->view.setSize(wndref.getSize().x, wndref.getSize().y);
}

Map::Map(std::string fname, sf::RenderWindow &wndref)
    : mx(0), my(0), np(1), wndref(wndref),
      textureMenu({"../imgs/wow.png", "../imgs/woow.png", "../imgs/wooow.png", "../imgs/woooow.png"}, wndref),
      entityMenu({"../imgs/player.png", "../imgs/pacman.png", "../imgs/capybaraa.png", "../imgs/arrow.png"}, wndref)
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
        int entityMapSize;
        file.read((char *)&entityMapSize, sizeof(int));
        for (int i = 0; i < entityMapSize; ++i)
        {
            int cmx, cmy, entityCount;
            file.read((char *)&cmx, sizeof(int));
            file.read((char *)&cmy, sizeof(int));
            file.read((char *)&entityCount, sizeof(int));
            
            for (int j = 0; j < entityCount; ++j)
            {
                int nameLength;
                file.read((char *)&nameLength, sizeof(int));
                
                std::string entityName(nameLength, '\0');
                file.read(&entityName[0], nameLength);
                
                float x, y;
                file.read((char *)&x, sizeof(float));
                file.read((char *)&y, sizeof(float));
                
                addEntity(x, y, entityName);
            }
        }
        //spawnEntities();
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
    int entityMapSize = this->entities.size();
    file.write((char *)&entityMapSize, sizeof(int));
    for (const auto &x : this->entities)
    {
        for (const auto &y : x.second)
        {
            int entityCount = y.second.size();
            file.write((char *)&x.first, sizeof(int));
            file.write((char *)&y.first, sizeof(int));
            file.write((char *)&entityCount, sizeof(int));
            for (const auto &entity : y.second)
            {
                int nameLength = entity.first.length();
                file.write((char *)&nameLength, sizeof(int));
                file.write(entity.first.c_str(), nameLength);
                file.write((char *)&entity.second.x, sizeof(float));
                file.write((char *)&entity.second.y, sizeof(float));
            }
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
        // Remove from placedEntities
        auto removedEntity = placedEntities[index];
        placedEntities.erase(placedEntities.begin() + index);

        // Remove from entities map
        for (auto& mapPart : entities)
        {
            for (auto& row : mapPart.second)
            {
                auto it = std::find_if(row.second.begin(), row.second.end(),
                    [&removedEntity](const auto& pair) {
                        return pair.first == removedEntity.second &&
                               pair.second == removedEntity.first.getPosition();
                    });
                if (it != row.second.end())
                {
                    row.second.erase(it);
                    return; // Entity found and removed
                }
            }
        }
    }
}
void Map::resetEntities()
{
    activeEntities.clear();
    spawnEntities();
    std::cout<<activeEntities.size()<<"\n";
}
void Map::spawnEntities() {
    std::cout << "aaa\n";
    for (const auto placedEntity : placedEntities) {
        const sf::Vector2f position = placedEntity.first.getPosition();
        const std::string type = placedEntity.second;
        Entity* entity = EntityFactory::createEntity(type, position,nullptr);
        if (entity) {
            activeEntities.push_back(entity);
        }
    }
}
   void Map::updateEntities(float deltaTime, const sf::Vector2u& windowSize,sf::FloatRect playerBounds) {
        for (auto& entity : activeEntities) {
            entity->update(deltaTime, *this, windowSize,playerBounds);
        }
    }
    void Map::drawEntities(sf::RenderWindow& window) {
        for (auto& entity : activeEntities) {
            entity->draw(window);
        }
    }