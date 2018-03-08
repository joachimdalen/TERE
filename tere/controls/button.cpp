#include <utility/utility.h>
#include <cmath>
#include <iostream>
#include "button.h"

Button::Button(std::string text, sf::Font &font) : text(text), font(font) {
    this->textColor = sf::Color(127, 127, 127);
    this->borderWidth = 3;
    this->borderColor = sf::Color(127, 127, 127);
    this->hoverColor = sf::Color::Blue;
    this->backgroundColor = sf::Color::Black;
    this->hovered = false;
    containerText.setFont(this->font);
    containerText.setString(this->text);
    containerText.setFillColor(this->textColor);
    // container.setSize(sf::Vector2f(containerText.getLocalBounds().width + 6.f,
    //        containerText.getLocalBounds().height + (containerText.getLocalBounds().height / 2)));
    container.setFillColor(this->backgroundColor);
    container.setOutlineThickness(this->borderWidth);
    container.setOutlineColor(this->borderColor);

}
void Button::draw(sf::RenderWindow &window) {
    if (this->isHovering(window)) {
        if (!hovered) {
            containerText.setFillColor(sf::Color::White);
            //  container.setOutlineColor(hoverColor);
            hovered = true;
        }
    } else {
        if (hovered) {
            containerText.setFillColor(textColor);
            //    container.setOutlineColor(borderColor);
            hovered = false;
        }
    }
    centerOrigin(containerText);
    centerOrigin(container);
    window.draw(container);
    window.draw(containerText);
}
void Button::setPosition(sf::Vector2f position) {
    containerText.setPosition(position);
    container.setPosition(position);
    this->position = position;
}
void Button::setSize(sf::Vector2f size) {
    container.setSize(size);
    this->size = size;
}
sf::Vector2f Button::getPosition() {
    centerOrigin(containerText);
    return containerText.getPosition();
}
bool Button::isHovering(sf::RenderWindow &window) {
    sf::Vector2i position = sf::Mouse::getPosition(window);
    return mouseOver(position, this->containerText);
}
bool Button::mouseInsideRect(sf::Vector2i position) {
    /*  return position.x >= this->container.getPosition().x
             && position.x <= (this->container.getPosition().x + this->container.getLocalBounds().width) &&
             position.y >= this->container.getPosition().y
             && position.y <= (this->container.getPosition().y + this->container.getLocalBounds().height);*/
    return mouseOver(position, this->containerText);
}
bool Button::clicked(sf::RenderWindow &window) {
    sf::Vector2i position = sf::Mouse::getPosition(window);
    return mouseOver(position, this->containerText);
}