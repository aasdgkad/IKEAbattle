#include "../hpp/libs.hpp"

Animation::Animation() 
    : Sprite(), currentFrame(0), frameTime(0), frameInterval(0.1f), 
      cellWidth(0), cellHeight(0),paused(false){}

void Animation::loadSpritesheet(const std::string& filename, int cellWidth, int cellHeight) {
    loadTexture(filename);
    this->cellWidth = cellWidth;
    this->cellHeight = cellHeight;
}

void Animation::addAnimation(const std::string& name, int row, int frameCount) {
    std::vector<sf::IntRect> frames;
    for (int i = 0; i < frameCount; ++i) {
        frames.push_back(sf::IntRect(i * cellWidth, row * cellHeight, cellWidth, cellHeight));
    }
    animations[name] = frames;
}

void Animation::update(float deltaTime, Map &map, const sf::Vector2u &screenres) {
    //Sprite::update(deltaTime,map,screenres);  // Call parent update method

    if (isPaused()|| animations.empty() || currentAnimation.empty()) return;

    frameTime += deltaTime;
    if (frameTime >= frameInterval) {
        frameTime = 0;
        currentFrame = (currentFrame + 1) % animations[currentAnimation].size();
    }
        updateTextureRect();
}

void Animation::setAnimation(const std::string& name) {
    if (currentAnimation != name && animations.find(name) != animations.end()) {
        currentAnimation = name;
        currentFrame = 0;
        sprite.setTextureRect(animations[currentAnimation][currentFrame]);
    }
}

void Animation::setFrameInterval(float interval) {
    frameInterval = interval;
}
void Animation::setCurrentFrame(int frame)
{
    if (frame >= 0 && frame < animations[currentAnimation].size())
    {
        currentFrame = frame;
        updateTextureRect();
    }
}
void Animation::pause()
{
    paused = true;
}

void Animation::resume()
{
    paused = false;
}
bool Animation::isPaused() const{
    return paused;
}
void Animation::updateTextureRect()
{
    sf::IntRect rect = animations[currentAnimation][currentFrame];
    if (!flipped)
    {
        rect.left = rect.left + rect.width;
        rect.width = -rect.width;
    }
    sprite.setTextureRect(rect);
}