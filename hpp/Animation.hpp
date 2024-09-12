#pragma once
class Animation : public Sprite {
public:
    Animation();
    virtual ~Animation() = default;

    // Load a spritesheet for animations
    void loadSpritesheet(const std::string& filename, int cellWidth, int cellHeight);

    // Add a new animation sequence
    void addAnimation(const std::string& name, int row, int frameCount);

    // Update the current animation
    void update(float deltaTime, Map &map, const sf::Vector2u& screenres) override;

    // Set the current animation
    void setAnimation(const std::string& name);

    // Set the time interval between frames
    void setFrameInterval(float interval);

    // Pause the current animation
    void pause();

    // Resume the current animation
    void resume();

    // Set the current frame of the animation
    void setCurrentFrame(int frame);

    // Check if the animation is paused
    bool isPaused() const;

protected:
    std::map<std::string, std::vector<sf::IntRect>> animations; // Map of animation sequences
    std::string currentAnimation;
    int currentFrame;
    float frameTime;
    float frameInterval;
    int cellWidth;
    int cellHeight;
    bool paused;

    // Update the texture rectangle for the current frame
    void updateTextureRect();
};