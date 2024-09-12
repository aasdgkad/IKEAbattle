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
    class Menu
{
    public:
    std::string getFileNameWithoutExtension(const std::string& path)
    {
        // Find the last occurrence of '/' or '\' to get the start of the filename
        size_t lastSlash = path.find_last_of("/\\");
        std::string fileName = (lastSlash == std::string::npos) ? path : path.substr(lastSlash + 1);

        // Find the last occurrence of '.' to remove the extension
        size_t lastDot = fileName.find_last_of(".");
        if (lastDot != std::string::npos)
        {
            fileName = fileName.substr(0, lastDot);
        }

        return fileName;
    }
        Menu(const std::vector<std::string>& entityPaths, const std::vector<std::string>& texturePaths, sf::RenderWindow& window);
        void draw();
        void selectNext();
        void selectPrevious();
        bool isOpen = false;
        int selectedIndex;
        bool isEntitySelected() const { return selectedIndex < entityTextures.size(); }

        std::vector<sf::Texture> entityTextures;
        std::vector<std::string> entityNames;
        std::vector<sf::Texture> textures;
        std::vector<std::string> textureNames;

        // Add this new function
        const std::string& getSelectedName() const 
        {
            if (isEntitySelected())
            {
                return entityNames[selectedIndex];
            }
            else
            {
                return textureNames[selectedIndex - entityTextures.size()];
            }
        }

    private:
        sf::RenderWindow& window;
        //std::string getFileNameWithoutExtension(const std::string& path);
    };


public:
    bool handleMenuClick(const sf::Vector2i& mousePosition);
    const std::string& getSelectedName() const;


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
    void changePart(int x, int y);              // It changes the part relative to the current one
    sf::FloatRect getPartBounds();
    Menu menu;
    void removeEntity(int x, int y);
    std::vector<sf::FloatRect> getEntityBounds();

    struct PlacedEntity
    {
        sf::Sprite sprite;
        std::string type;
        std::unique_ptr<Entity> entity;
    };
    const sf::Texture* getSelectedTexture() const;
    void removeEntity(int index);
    void drawEditorEntities(sf::RenderWindow &window, const Map::PlacedEntity *selectedEntity, bool &isOpen);
    const sf::Texture *getEntityTexture(const std::string &entityName) const;
    std::vector<Entity *> activeEntities;
    void resetEntities(sf::FloatRect &playerBounds);
    void spawnEntities(sf::FloatRect &playerBounds);

    void updateEntities(float deltaTime, const sf::Vector2u &windowSize);

    void drawEntities(sf::RenderWindow &window);

    std::vector<std::unique_ptr<PlacedEntity>> placedEntities;

    void addEntity(int x, int y, const std::string &entityType);
    sf::Clock inputClock;
    std::string inputBuffer;

private:
    static std::unordered_map<std::string, sf::Texture> entityTextures;          // used in the editor for the placed entities
    std::unordered_map<int, std::unordered_map<int, std::vector<Object *>>> obj; // It used to be a vector with pointers to the actual objects contained by the map
    int mx, my, np;
    sf::View view;
    sf::RenderWindow &wndref;
    void removeDeadEntities();

public:
    class PropertyEditor
    {
    public:
        static const float INPUT_BOX_WIDTH;
        static const float INPUT_BOX_HEIGHT;
        static const float LINE_SPACING;
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
        void wrapText(sf::Text &text, float maxWidth);
        float calculateRequiredHeight(const sf::Text &text, float maxWidth);
        void adjustLayout();
        void adjustBackgroundSize();
    };
};