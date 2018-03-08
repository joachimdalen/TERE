#include <utility/utility.h>
#include <utility/key_converter.h>
#include "text_key_mapper.h"

TextKeyMapper::TextKeyMapper(std::string text, sf::Font &font, sf::Keyboard::Key initail)
{
    optionText.setString(text);
    optionText.setFont(font);
    optionText.setCharacterSize(GameSettings::cSize);
    optionText.setFillColor(sf::Color(127, 127, 127));

    optionKeyText.setString("O");
    optionKeyText.setFont(font);
    optionKeyText.setCharacterSize(GameSettings::cSize);
    optionKeyText.setFillColor(sf::Color(127, 127, 127));

    cSize = optionKeyText.getGlobalBounds().height;

    this->setKey(initail);
}


void TextKeyMapper::draw(sf::RenderWindow &window)
{
    window.draw(optionText);
    window.draw(optionKeyText);
}


void TextKeyMapper::setSelect()
{
    optionText.setFillColor(sf::Color::White);
    optionKeyText.setFillColor(sf::Color::White);
}


void TextKeyMapper::setUnSelect()
{
    optionText.setFillColor(sf::Color(127, 127, 127));
    optionKeyText.setFillColor(sf::Color(127, 127, 127));
}


void TextKeyMapper::setPosition(sf::Vector2f position, sf::RenderWindow &window)
{
    leftOrigin(optionText);
    optionText.setPosition(position);

    rightOrigin(optionKeyText);
    optionKeyText.setPosition( window.getSize().x - position.x, position.y );
}


sf::Vector2f TextKeyMapper::getPosition()
{
    return optionText.getPosition();
}


sf::Keyboard::Key TextKeyMapper::getValue()
{
    return value;
}


std::string TextKeyMapper::getValueString()
{
    return keyToString(value);
}


void TextKeyMapper::setKey(sf::Keyboard::Key key)
{
    value = key;
    this->optionKeyText.setString(keyToString(value));
    rightOriginFixed(optionKeyText, cSize);
}


void TextKeyMapper::setCharacterSize(unsigned int size)
{
    optionText.setCharacterSize(size);
}
