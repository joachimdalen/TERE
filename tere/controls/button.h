#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include <memory>
#include "string"

class Button {
public:
    Button(std::string text, sf::Font &font);
    void setPosition(sf::Vector2f position);
    void setSize(sf::Vector2f size);
    sf::Vector2f getPosition();
    void draw(sf::RenderWindow &window);
    bool clicked(sf::RenderWindow &window);
protected:
    bool hovered;
    bool mouseInsideRect(sf::Vector2i position);
    bool isHovering(sf::RenderWindow &window);
    sf::RectangleShape container;
    sf::Text containerText;
    std::string &text;
    sf::Color textColor;
    sf::Color backgroundColor;
    sf::Color borderColor;
    sf::Color hoverColor;
    sf::Font &font;
    float borderWidth;
    sf::Vector2f position;
    sf::Vector2f size;
};
typedef std::shared_ptr<Button> ButtonPtr;


#endif //BUTTON_H