#include "libs.hpp"

Map::Object::Object(int x, int y, int w, int h, std::string tname) : rect(sf::Vector2f(w, h))
{
    // In case the texture will not load then the program will throw an exception
    assert(this->tex.loadFromFile(tname));
    this->tex.setRepeated(true); // Enable texture repeating

    // Initializing the rectangle shape
    this->rect.setTexture(&this->tex);
    this->rect.setTextureRect(sf::IntRect(0, 0, w, h)); // Texture repeated over 400x300 area
    this->rect.setPosition(x, y);

    // Setting the texture ID by removing the suffix .png and turning what is left to a number
    tname.erase(tname.length()-4);
    this->texid = std::stoi(tname);
}

void Map::Object::draw(sf::RenderWindow &window)
{
    window.draw(rect);
}

Map::Map(std::string fname)
{
    std::ifstream file(fname, std::ios::binary);
    int size;
    file.read((char*)&size, sizeof(int));
    for(int i = 0; i < size; i++){
        int cx,cy,cw,ch,tid;
        file.read((char*)&cx, sizeof(int));
        file.read((char*)&cy, sizeof(int));
        file.read((char*)&cw, sizeof(int));
        file.read((char*)&ch, sizeof(int));
        file.read((char*)&tid, sizeof(int));

        this->obj.push_back(new Object(cx,cy,cw,ch, std::to_string(tid) + ".png"));
    }
}

void Map::draw(sf::RenderWindow &window)
{
    for (int i = 0; i < this->obj.size(); i++)
    {
        obj[i]->draw(window);
    }
}

void Map::addObject(int x, int y, int w, int h, std::string tname){
    this->obj.push_back(new Object(x,y,w,h,tname));
}

void Map::saveToFile(std::string fname){
    std::ofstream file(fname, std::ios::binary | std::ios::app);
    int size = this->obj.size();
    file.write((char*)&size, sizeof(int));
    for(int i = 0; i < this->obj.size(); i++){
        // The only reason I am doing this btw is because sf::RectangleShape returns floats when my object uses integers ;)
        int cx = this->obj[i]->rect.getPosition().x;
        int cy = this->obj[i]->rect.getPosition().y;
        int cw = this->obj[i]->rect.getSize().x;
        int ch = this->obj[i]->rect.getSize().y;
        file.write((char*)&cx, sizeof(int));
        file.write((char*)&cy, sizeof(int));
        file.write((char*)&cw, sizeof(int));
        file.write((char*)&ch, sizeof(int));
        file.write((char*)&this->obj[i]->texid, sizeof(unsigned int));
    }
}

Map::~Map()
{
    for(int i = 0; i < this->obj.size(); i++){
        delete this->obj[i];
    }
}

std::vector<sf::FloatRect> Map::getObjectBounds() const {
    std::vector<sf::FloatRect> bounds;
    for (const auto& object : obj) {
        bounds.push_back(object->rect.getGlobalBounds());
    }
    return bounds;
}