#ifndef TEXT_KEY_MAPPER_H
#define TEXT_KEY_MAPPER_H

#include <memory>

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

class TextKeyMapper
{
public:
    TextKeyMapper(std::string text, sf::Font &font, sf::Keyboard::Key initail);
    void draw(sf::RenderWindow &window);
    void setSelect();
    void setUnSelect();
    void setPosition(sf::Vector2f position, sf::RenderWindow &window);
    sf::Vector2f getPosition();
    sf::Keyboard::Key getValue();
    std::string getValueString();
    void setKey(sf::Keyboard::Key key);
    void setCharacterSize(unsigned int size);

protected:
    sf::Text optionText, optionKeyText;
    sf::Keyboard::Key value;

    int cSize;

};
typedef std::shared_ptr<TextKeyMapper> TextKeyMapperPtr;

#endif //TERE_TEXT_KEY_MAPPER_H
