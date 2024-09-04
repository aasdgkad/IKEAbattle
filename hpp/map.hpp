#pragma once
class Player;
class Entity;
class EntityFactory;
class TextBox;
class Map
{

private:
    class Object
    {
    public:
        Object(int x, int y, int w, int h, std::string tname);
        ~Object() = default;

        void draw(sf::RenderWindow &window); // A function that draws the object

        sf::RectangleShape rect; // Rectangle that holds a reference to the texture
        sf::Texture tex;         // The texture in quexture
        std::string texid;       // I know it looks redundant but I need it in order to transfer it to a file
    };

public:
    class EntityMenu
    {
    public:
        EntityMenu(const std::vector<std::string> &entityPaths, sf::RenderWindow &window);
        void draw();
        void selectNext();
        void selectPrevious();
        bool isOpen = false;
        int selectedIndex;

        std::vector<sf::Texture> entityTextures;
        std::vector<std::string> entityNames;

    private:
        sf::RenderWindow &window;
        std::string getFileNameWithoutExtension(const std::string &path);
    };
    class TextureMenu
    {
    public:
        TextureMenu(const std::vector<std::string> &texturePaths, sf::RenderWindow &window);
        void draw();
        void selectNext();
        void selectPrevious();
        bool isOpen = false;
        sf::Sprite selectedTexture;
        int selectedIndex;

        std::vector<sf::Texture> textures;
        std::vector<std::string> textureNames;

    private:
        std::string getFileNameWithoutExtension(const std::string &path)
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
        sf::RenderWindow &window;
    };

public:
    bool *gameOver;
    std::vector<sf::FloatRect> getObjectBounds();
    Map(sf::RenderWindow &wndref, bool &gameover);
    Map(std::string fname, sf::RenderWindow &wndref, bool &gameover); // used to load the map from a file
    Map(const Map &) = delete;                                        // Copy constructor is not needed
    Map &operator=(const Map &) = delete;                             // Copy assignment operator is not needed
    ~Map();

    void draw();                                // A function that draws the map
    void addObject(int x, int y, int w, int h); // A function that adds an object to the map
    void removeObject(int index);               // A function that removes an object from the current part
    void saveToFile(std::string fname);         // A function that saves the map to a file (used by the level editor)
    void changePart(int x, int y); // It changes the part relative to the current one
    sf::FloatRect getPartBounds();
    TextureMenu textureMenu;
    void drawTextureMenu();
    const std::string &getSelectedEntityName() const;
    bool handleTextureMenuClick(const sf::Vector2i &mousePosition);
    void toggleTextureMenu();
    void selectNextTexture();
    void selectPreviousTexture();
    int getSelectedTextureIndex() const;
    const sf::Texture *getSelectedTexture() const;
    bool isTextureMenuOpen() const;

    EntityMenu entityMenu;
    void drawEntityMenu();
    bool handleEntityMenuClick(const sf::Vector2i &mousePosition);
    void toggleEntityMenu();
    void selectNextEntity();
    void selectPreviousEntity();
    int getSelectedEntityIndex() const;
    bool isEntityMenuOpen() const;
    void removeEntity(int x, int y);
    std::vector<sf::FloatRect> getEntityBounds();

    struct PlacedEntity
    {
        sf::Sprite sprite;
        std::string type;
        std::unique_ptr<Entity> entity;
    };
    void removeEntity(int index);
    void drawEditorEntities(sf::RenderWindow &window,Map::PlacedEntity *selectedEntity,bool isOpen);
    const sf::Texture *getEntityTexture(const std::string &entityName) const;
    std::vector<Entity *> activeEntities;
    void resetEntities(sf::FloatRect &playerBounds);
    void spawnEntities(sf::FloatRect &playerBounds);

    void updateEntities(float deltaTime, const sf::Vector2u &windowSize);

    void drawEntities(sf::RenderWindow &window);

    std::vector<PlacedEntity> placedEntities;

    void addEntity(int x, int y, const std::string &entityType);

private:
    std::vector<std::map<std::string, std::string>> entityProperties;
    static std::unordered_map<std::string, sf::Texture> entityTextures;
    std::unordered_map<int, std::unordered_map<int, std::vector<Object *>>> obj; // It used to be a vector with pointers to the actual objects contained by the map
    std::unordered_map<int, std::unordered_map<int, std::vector<std::pair<std::string, sf::Vector2f>>>> entities;
    int mx, my, np;
    sf::View view;
    sf::RenderWindow &wndref;
    void removeDeadEntities();

public:
    class PropertyEditor
    {
    public:
        sf::RectangleShape background;
        std::vector<sf::Text> labels;
        std::vector<sf::RectangleShape> inputBoxes;
        std::vector<sf::Text> inputTexts;
        Map::PlacedEntity *selectedEntity = nullptr;
        int selectedInputBox;
        bool isOpen = false;
        sf::Font *font; // Store font pointer

        void setup(sf::Font &loadedFont);

        void updateForEntity(Map::PlacedEntity *entity, sf::Font &font);

        void draw(sf::RenderWindow &window);

        void handleInput(sf::Event &event, sf::RenderWindow &window);

        void applyChanges();
    };
};
