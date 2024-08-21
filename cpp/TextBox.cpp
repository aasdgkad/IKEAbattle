#include "../hpp/libs.hpp"

TextBox::TextBox(const std::string& text, float letterDelay)
    : letterDelay(letterDelay), elapsedTime(0.0f), currentLetter(0), visible(false)
{
    background.setFillColor(sf::Color(255, 255, 255, 200)); // Semi-transparent white
    background.setOutlineColor(sf::Color::Black);
    background.setOutlineThickness(2.0f);

    font.loadFromFile("../fonts/ARIAL.TTF"); // Replace with the actual path to your font file
    visibleContent.setFont(font);
    visibleContent.setString(text);
    visibleContent.setFillColor(sf::Color::Black);
    visibleContent.setCharacterSize(18);
}

void TextBox::setString(const std::string& text)
{
    if(text.length()!=0){

    textContent=text;
    visibleContent.setString("");
    currentLetter = 0;
    elapsedTime = 0.0f;
    visible = true;
    }
    else{
        visible=false;
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
        background.setPosition(sf::Vector2f(windowr.getSize().x / 6, windowr.getSize().y - 115));
        visibleContent.setPosition(sf::Vector2f(windowr.getSize().x / 6+10.0f, windowr.getSize().y - 115+10.0f));
        background.setSize(sf::Vector2f( windowr.getSize().x / 6, 70.0f));
        windowr.draw(background);
        windowr.draw(visibleContent);
        // Restore the original view
        windowr.setView(originalView);
    }
}