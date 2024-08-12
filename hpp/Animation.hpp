#pragma once
class Animation
{
public:
    Animation();
    void loadSpritesheet(const std::string& filename, int cellWidth, int cellHeight); // Load a spritesheet for animations
    void addAnimation(const std::string& name, int row, int frameCount); // Add a new animation sequence
    void update(float deltaTime); // Update the current animation
    void setAnimation(const std::string& name); // Set the current animation
    void setFrameInterval(float interval); // Set the time interval between frames
    sf::Sprite& getSprite(); // Get the current sprite
    void pause(); // Pause the current animation
    void resume(); // Resume the current animation
    void setCurrentFrame(int frame); // Set the current frame of the animation
    bool isPaused() const; // Check if the animation is paused
protected:
    sf::Sprite sprite;
    sf::Texture spritesheet;
    std::map<std::string, std::vector<sf::IntRect>> animations; // Map of animation sequences
    std::string currentAnimation;
    int currentFrame;
    float frameTime;
    float frameInterval;
    int cellWidth;
    int cellHeight;
    bool isFacingRight;
    bool paused;
    void updateTextureRect(); // Update the texture rectangle for the current frame
};