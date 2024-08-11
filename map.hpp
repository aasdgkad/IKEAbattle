#pragma once
class Player;

class Map{
    private:
    class Object{
        public:
        Object(int x, int y, int w, int h, std::string tname);
        ~Object() = default;

        void draw(sf::RenderWindow &window);// A function that draws the object

        sf::RectangleShape rect;// Rectangle that holds a reference to the texture
        sf::Texture tex;// The texture in quexture
        unsigned int texid;// I know it looks redundant but I need it in order to transfer it to a file       
    };

    public:
    std::vector<sf::FloatRect> getObjectBounds();
    Map(sf::RenderWindow& wndref);
    Map(std::string fname, sf::RenderWindow& wndref);//used to load the map from a file
    Map(const Map&) = delete;// Copy constructor is not needed
    Map& operator=(const Map&) = delete;// Copy assignment operator is not needed
    ~Map();

    void draw();// A function that draws the map
    void addObject(int x, int y, int w, int h, std::string tname);// A function that adds an object to the map
    void removeObject(int index);// A function that removes an object from the current part
    void saveToFile(std::string fname);// A function that saves the map to a file (used by the level editor)
    void changePart(int x, int y);// It changes the part relative to the current one
    sf::FloatRect getPartBounds();

    private:
    std::unordered_map<int, std::unordered_map<int, std::vector<Object*>>> obj;// It used to be a vector with pointers to the actual objects contained by the map
    int mx,my,np;
    sf::View view;
    sf::RenderWindow& wndref;
    //std::vector<Object*> obj;// A vector with pointers to the actual objects contained by the map

};