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
    tname.erase(tname.length() - 4);
    this->texid = std::stoi(tname);
}

void Map::Object::draw(sf::RenderWindow &window)
{
    window.draw(rect);
}

Map::Map(sf::RenderWindow& wndref) : mx(0), my(0), np(1), wndref(wndref)
{
    this->view.setSize(wndref.getSize().x, wndref.getSize().y);
}

Map::Map(std::string fname, sf::RenderWindow& wndref) : mx(0), my(0), np(1), wndref(wndref)
{
    this->view.setSize(wndref.getSize().x, wndref.getSize().y);
    if (std::filesystem::exists(fname)) {
        std::ifstream file(fname, std::ios::binary);
        int size;
        file.read((char *)&size, sizeof(int));
        this->np = size;
        for (int j = 0; j < size; j++)
        {
            int cmx, cmy, ssize;
            file.read((char *)&cmx, sizeof(int));
            file.read((char *)&cmy, sizeof(int));
            file.read((char *)&ssize, sizeof(int));
            for (int i = 0; i < ssize; i++)
            {
                int cx, cy, cw, ch, tid;
                file.read((char *)&cx, sizeof(int));
                file.read((char *)&cy, sizeof(int));
                file.read((char *)&cw, sizeof(int));
                file.read((char *)&ch, sizeof(int));
                file.read((char *)&tid, sizeof(int));

                this->obj[cmx][cmy].push_back(new Object(cx, cy, cw, ch, std::to_string(tid) + ".png"));
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

void Map::addObject(int x, int y, int w, int h, std::string tname)
{
    int wx = this->wndref.getSize().x, wy = this->wndref.getSize().y;
    this->obj[mx][my].push_back(new Object(x + wx*mx, y + wy*my, w, h, tname));
    std::cout << "object added to " << x + wx*mx << " " << y + wy*my << std::endl;
}

void Map::removeObject(int index){
    this->obj[mx][my].erase(this->obj[mx][my].begin()+index);
}

void Map::saveToFile(std::string fname)
{
    if (std::filesystem::exists(fname)) {
        // Remove the file
        std::filesystem::remove(fname);
    }
    std::ofstream file(fname, std::ios::binary | std::ios::app);
    int size = this->np;
    file.write((char *)&size, sizeof(int));
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
                // The only reason I am doing this btw is because sf::RectangleShape returns floats when my object uses integers ;)
                int cx = this->obj[x.first][y.first][i]->rect.getPosition().x;
                int cy = this->obj[x.first][y.first][i]->rect.getPosition().y;
                int cw = this->obj[x.first][y.first][i]->rect.getSize().x;
                int ch = this->obj[x.first][y.first][i]->rect.getSize().y;
                file.write((char *)&cx, sizeof(int));
                file.write((char *)&cy, sizeof(int));
                file.write((char *)&cw, sizeof(int));
                file.write((char *)&ch, sizeof(int));
                file.write((char *)&this->obj[x.first][y.first][i]->texid, sizeof(unsigned int));
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
    this->view.setCenter(wx/2 + wx*this->mx, wy/2 + wy*this->my);
    this->wndref.setView(view);
    std::cout << "changed to " << mx << " " << my << " center " << view.getCenter().x << " " << view.getCenter().y << std::endl;
}

sf::FloatRect Map::getPartBounds(){
    sf::FloatRect bounds;
    int wx = this->wndref.getSize().x, wy = this->wndref.getSize().y;
    bounds.left = this->mx * wx;
    bounds.width = wx;
    bounds.top = this->my*wy;
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