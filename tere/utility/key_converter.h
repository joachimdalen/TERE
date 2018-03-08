#ifndef KEY_CONVERTER_H
#define KEY_CONVERTER_H

#include <SFML/Window/Keyboard.hpp>
#include <string>
std::string keyToString(sf::Keyboard::Key key);
sf::Keyboard::Key stringToKey(std::string key);
#endif //KEY_CONVERTER_H
