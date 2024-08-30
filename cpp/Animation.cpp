#include "../hpp/libs.hpp"

Animation::Animation()
    : currentFrame(0), frameTime(0), frameInterval(0.1f), isFacingRight(true),paused(false)
{
}
void Animation::loadSpritesheet(const std::string& filename, int cellWidth, int cellHeight)
{
    spritesheet.loadFromFile(filename);
    if (!spritesheet.loadFromFile(filename)) {
            // Handle error
            std::cerr << "Failed to load texture: " << filename << std::endl;
        }
    sprite.setTexture(spritesheet);
    this->cellWidth = cellWidth;
    this->cellHeight = cellHeight;
}

void Animation::addAnimation(const std::string& name, int row, int frameCount)
{
    std::vector<sf::IntRect> frames;
    for (int i = 0; i < frameCount; ++i)
    {
        frames.push_back(sf::IntRect(i * cellWidth, row * cellHeight, cellWidth, cellHeight));
    }
    animations[name] = frames;
}

void Animation::update(float deltaTime)
{
    if (animations.empty() || currentAnimation.empty() || paused) return;

    frameTime += deltaTime;
    if (frameTime >= frameInterval)
    {
        frameTime = 0;
        currentFrame = (currentFrame + 1) % animations[currentAnimation].size();
        updateTextureRect();
    }
}

void Animation::setAnimation(const std::string& name)
{
    if (currentAnimation != name && animations.find(name) != animations.end())
    {
        currentAnimation = name;
        currentFrame = 0;
        updateTextureRect();
    }
}

void Animation::setFrameInterval(float interval)
{
    frameInterval = interval;
}

sf::Sprite& Animation::getSprite()
{
    return sprite;
}

void Animation::updateTextureRect()
{
    sf::IntRect rect = animations[currentAnimation][currentFrame];
    if (!isFacingRight)
    {
        rect.left = rect.left + rect.width;
        rect.width = -rect.width;
    }
    sprite.setTextureRect(rect);
}
void Animation::pause()
{
    paused = true;
}

void Animation::resume()
{
    paused = false;
}
void Animation::setCurrentFrame(int frame)
{
    if (frame >= 0 && frame < animations[currentAnimation].size())
    {
        currentFrame = frame;
        updateTextureRect();
    }
}

bool Animation::isPaused() const
{
    return paused;
}