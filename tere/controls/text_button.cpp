#include <utility/utility.h>
#include "text_button.h"

TextButton::TextButton(std::string text, sf::Font &font) {
    originalText = text;
    optionText.setString(text);
    optionText.setFont(font);
    optionText.setCharacterSize(GameSettings::cSize);
    optionText.setFillColor(sf::Color(127, 127, 127));
}

void TextButton::draw(sf::RenderWindow &window) {

    centerOriginHorizontal(optionText);
    //centerOrigin(optionText);
    window.draw(optionText);
}


void TextButton::updateString(std::string newText)
{
    originalText = newText;

    optionText.setString(newText);
    centerOriginHorizontal(optionText);
}


void TextButton::setSelect() {
    //optionText.setString(">> " + originalText + " <<");
    optionText.setFillColor(sf::Color::White);
    seleted = true;
}

void TextButton::setUnSelect() {
    optionText.setString(originalText);
    optionText.setFillColor(sf::Color(127, 127, 127));
    seleted = false;
}

void TextButton::setPosition(sf::Vector2f position) {

    centerOriginHorizontal(optionText);
    //centerOrigin(optionText);
    optionText.setPosition(position);
}

sf::Vector2f TextButton::getPosition() {
    return optionText.getPosition();
}

bool TextButton::isSelected() {
    return seleted;
}

void TextButton::setCharacterSize(unsigned int size) {
    optionText.setCharacterSize(size);
}
