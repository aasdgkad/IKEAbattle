// CutScene.cpp
#include "../hpp/libs.hpp"
#include <algorithm>

CutScene::CutScene(const std::vector<std::string>& imagePaths, const sf::Vector2u& windowSize)
    : currentFrame(0), fadeTime(0), frameDuration(0.0f), pauseDuration(0.0f), 
      isFinished(false), isPaused(false), cutSceneFinished(false), windowSize(windowSize) {
    loadTextures(imagePaths);
    if (!textures.empty()) {
        updateSpriteForCurrentFrame();
        currentSprite.setColor(sf::Color(255, 255, 255, 0)); // Start fully transparent
    }
}

void CutScene::loadTextures(const std::vector<std::string>& imagePaths) {
    for (const auto& path : imagePaths) {
        sf::Texture texture;
        if (texture.loadFromFile(path)) {
            textures.push_back(texture);
        }
    }
}

void CutScene::updateSpriteForCurrentFrame() {
    if (currentFrame < textures.size()) {
        currentSprite.setTexture(textures[currentFrame], true);  // true resets the texture rect
        
        sf::Vector2u textureSize = textures[currentFrame].getSize();
        float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
        float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
        float scale = std::min(scaleX, scaleY);

        currentSprite.setScale(scale, scale);

        sf::FloatRect bounds = currentSprite.getLocalBounds();
        currentSprite.setOrigin(bounds.width / 2, bounds.height / 2);
        currentSprite.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);
    }
}

bool CutScene::update(float deltaTime) {
    if (isFinished) return true;

    fadeTime += deltaTime;

    if (isPaused) {
        if (fadeTime >= pauseDuration) {
            isPaused = false;
            fadeTime = 0;
            currentFrame++;
            if (currentFrame >= textures.size()) {
                isFinished = true;
                return true;
            }
            updateSpriteForCurrentFrame();
            currentSprite.setColor(sf::Color(255, 255, 255, 0)); // Start fully transparent
        }
    } else {
        float halfDuration = frameDuration / 2;
        if (fadeTime < halfDuration) {
            // Fading in
            float alpha = (fadeTime / halfDuration) * 255;
            currentSprite.setColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(alpha)));
        } else if (fadeTime < frameDuration) {
            // Fading out
            float alpha = (1 - (fadeTime - halfDuration) / halfDuration) * 255;
            currentSprite.setColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(alpha)));
        } else {
            // Frame finished, start pause
            isPaused = true;
            fadeTime = 0;
        }
    }

    return false;
}

void CutScene::draw(sf::RenderWindow& window) {
    window.draw(currentSprite);
}