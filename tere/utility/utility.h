#ifndef UTILITY_H
#define UTILITY_H

#include <sstream>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <settings/game_settings.h>

namespace sf
{
    class Sprite;
    class Text;
}

// Since std::to_string doesn't work on MinGW we have to implement
// our own to support all platforms.
template <typename T>
std::string toString(const T& value)
{
    std::stringstream stream;
    stream << value;
    return stream.str();
}

void centerOrigin(sf::Sprite& sprite);
void leftOrigin(sf::Text& text);
void centerOrigin(sf::Text& text);
void centerOriginHorizontal(sf::Text& text);
void centerOrigin(sf::RectangleShape& rectangle);
void rightOrigin(sf::Text& text);
void rightOriginFixed(sf::Text &text, int cSize);
std::string thousandDelimiter(int num);
//std::string timerString(int seconds, int milliseconds, int countdown);
std::string timerString(int milliseconds, int countdown);
bool mouseOver(sf::Vector2i position, sf::RectangleShape container);
bool mouseOver(sf::Vector2i position, const sf::Text &container);
bool textToBool(std::string text);
std::string boolToText(bool value);
std::vector<std::string> split(const std::string &s, char delim);
std::string gameModeToString(GameSettings::GameMode mode);
std::string playModeToString(GameSettings::Mode mode);
#endif //UTILITY_H
