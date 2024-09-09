#include "../hpp/libs.hpp"

TextBox::TextBox(const std::string& text, float letterDelay)
    : letterDelay(letterDelay), elapsedTime(0.0f), currentLetter(0), visible(false), hasCharacterImage(false)
{
    background.setFillColor(sf::Color(255, 255, 255, 200)); // Semi-transparent white
    background.setOutlineColor(sf::Color::Black);
    background.setOutlineThickness(2.0f);
    font.loadFromFile("../fonts/ARIAL.TTF"); // Replace with the actual path to your font file
    setCharacterImage("../imgs/hammer.jpg");
    visibleContent.setFont(font);
    visibleContent.setString(text);
    visibleContent.setFillColor(sf::Color::Black);
    visibleContent.setCharacterSize(18);
}

void TextBox::setString(const std::string& text)
{
    if(text.length() != 0) {
        textContent = text;
        visibleContent.setString("");
        currentLetter = 0;
        elapsedTime = 0.0f;
        visible = true;
    }
    else {
        visible = false;
    }
}

void TextBox::update(float deltaTime)
{
    if (visible)
    {
        elapsedTime += deltaTime;
        if (elapsedTime >= letterDelay)
        {
            elapsedTime = 0.0f;
            if (currentLetter < textContent.size())
            {
                visibleContent.setString(visibleContent.getString() + textContent[currentLetter]);
                currentLetter++;
            }
        }
    }
}

void TextBox::draw(sf::RenderWindow& windowr)
{
    if (visible)
    {
        // Save the current view
        sf::View originalView = windowr.getView();
        // Reset the view to the default (window coordinates)
        windowr.setView(windowr.getDefaultView());

        float imageWidth = hasCharacterImage ? 70.0f : 0.0f; // Adjust this value based on your image size
        float textBoxWidth = windowr.getSize().x / 3;

        background.setPosition(sf::Vector2f(windowr.getSize().x / 6 + imageWidth, windowr.getSize().y - 110));
        visibleContent.setPosition(sf::Vector2f(windowr.getSize().x / 6 + imageWidth + 10.0f, windowr.getSize().y - 110 + 10.0f));
        background.setSize(sf::Vector2f(textBoxWidth, 70.0f));

        if (hasCharacterImage)
        {
            characterSprite.setPosition(sf::Vector2f(windowr.getSize().x / 6, windowr.getSize().y - 110));
            windowr.draw(characterSprite);
        }

        windowr.draw(background);
        windowr.draw(visibleContent);

        // Restore the original view
        windowr.setView(originalView);
    }
}

void TextBox::setCharacterImage(const std::string& imagePath)
{
    if (characterTexture.loadFromFile(imagePath))
    {
        characterSprite.setTexture(characterTexture);
        
        // Scale the sprite to fit the height of the text box
         float scaley = 70.0f / characterSprite.getLocalBounds().height;
        float scalex = 70.0f / characterSprite.getLocalBounds().width;
        characterSprite.setScale(scalex, scaley);
        hasCharacterImage = true;
    }
    else
    {
        hasCharacterImage = false;
    }
}