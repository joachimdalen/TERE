#ifndef TEXT_BUTTON_H
#define TEXT_BUTTON_H


#include <SFML/Graphics/Text.hpp>
#include <memory>
#include <SFML/Graphics/RenderWindow.hpp>

class TextButton {
public:
    TextButton(std::string text, sf::Font &font);
    void draw(sf::RenderWindow &window);
    void updateString(std::string newText);
    void setSelect();
    void setUnSelect();
    void setPosition(sf::Vector2f position);
    sf::Vector2f getPosition();
    bool isSelected();
    void setCharacterSize(unsigned int size);
protected:
    sf::Text optionText;
    std::string originalText;
    bool  seleted = false;
};

typedef std::shared_ptr<TextButton> TextButtonPtr;
#endif //TERE_TEXT_BUTTON_H
