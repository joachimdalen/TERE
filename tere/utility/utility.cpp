#include "utility.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <cmath>
#include <SFML/Window/VideoMode.hpp>
#include <iostream>
#include <SFML/System/Clock.hpp>

void centerOrigin(sf::Sprite &sprite) {
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
}


void leftOrigin(sf::Text &text) // Left bound, top bound
{
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(0, bounds.top);
}


void centerOrigin(sf::Text &text)
{
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
}

void centerOriginHorizontal(sf::Text &text) // Centered horizontally, top bound
{
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(std::floor(bounds.left + bounds.width / 2.f), bounds.top );
}

void rightOrigin(sf::Text &text) // Right bound, top bound
{
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(bounds.width, bounds.top);
}

void rightOriginFixed(sf::Text &text, int cSize) // Right bound, max height even for strings with low height
{
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(bounds.width, std::floor(bounds.top - (cSize - bounds.height) / 2));
}


std::string thousandDelimiter(int num) {
    std::string old_str, new_str, temp, div;
    int len, rest, base, i, neg = 0;

    old_str = "" + toString(num);
    len = old_str.length();

    if (old_str.at(0) == '-') {
        neg = 1;
        old_str.erase(0, 1);
        len--;
    }

    base = len / 3;
    rest = len % 3;

    div = ",";

    if (base) {
        new_str = "";

        if (rest) {
            new_str = old_str.substr(0, rest) + div;
            old_str.erase(0, rest);
        }

        for (i = 0; i < base; i++) {
            temp = old_str.substr(i * 3, 3);
            new_str = new_str + temp;

            if (i < (base - 1))
                new_str = new_str + div;

        }
    } else {
        new_str = old_str;
    }

    if (neg) {
        new_str = "-" + new_str;
    }

    return new_str;
}


// Counter, or countdown as HH:MM:SS.CS
//std::string timerString(int seconds, int milliseconds, int countdown) {
std::string timerString(int milliseconds, int countdown)
{
    std::string s = "";
    int seconds = milliseconds / 1000;
    int hours = 0, minutes = 0, centiseconds = 0;

    if (countdown != 0)
    {
        seconds = countdown - seconds - (milliseconds - seconds * 1000 > 0 ? 1 : 0);
        milliseconds = countdown * 1000 - milliseconds;
    }

    if (seconds < 0) //  Cannot have negative time
    {
        seconds = 0;
        milliseconds = 0;
    }

    if (seconds >= 3600) // Hours (Only printed if there are any full hours)
    {
        hours = seconds / 3600;
        seconds -= 3600 * hours;
        milliseconds -= 3600 * hours * 1000;
        s += toString(hours) + ":";
    }

    if (seconds >= 60) // Minutes (Minutes, seconds and centiseconds are always printed)
    {
        minutes = seconds / 60;
        seconds -= 60 * minutes;
        milliseconds -= 60 * minutes * 1000;

        if (minutes < 10) s += "0";
        s += toString(minutes) + ":";
    } else s += "00:";

    if (seconds < 10) s += "0";
    s += toString(seconds) + ".";

    milliseconds -= seconds * 1000;
    centiseconds = milliseconds / 10;

    if (centiseconds < 10) s += "0";
    s += toString(centiseconds);

    return s;
}


void centerOrigin(sf::RectangleShape &rectangle) {
    sf::FloatRect bounds = rectangle.getLocalBounds();
    rectangle.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));

}


bool mouseOver(sf::Vector2i position, const sf::Text &container) {
    sf::FloatRect bounds = container.getLocalBounds();
    return position.x >= (container.getPosition().x - (bounds.width / 2))
           && position.x <= ((container.getPosition().x - (bounds.width / 2)) + bounds.width) &&
           position.y >= (container.getPosition().y - (bounds.height / 2))
           && position.y <= ((container.getPosition().y - (bounds.height / 2)) + bounds.height);
}
bool textToBool(std::string text) {
    return text == "On" || text == "TRUE" ? true : false;
}
std::string boolToText(bool value) {
    return value ? "On" : "Off";
}
std::vector<std::string> split(const std::string &s, char delim) {
    std::stringstream ss(s);
    std::string item;
    std::vector<std::string> tokens;
    while (getline(ss, item, delim)) {
        tokens.push_back(item);
    }
    return tokens;
}
std::string gameModeToString(GameSettings::GameMode mode) {
    switch (mode) {
        case GameSettings::GameMode::Marathon:
            return "Marathon";
        case GameSettings::GameMode::LineClear:
            return "Line Clear";
        case GameSettings::GameMode::Battle:
            return "Battle";
        case GameSettings::GameMode::Sprint:
            return "Spring";
        default:
            return "Unknown";
    }
}
std::string playModeToString(GameSettings::Mode mode) {
    switch (mode) {
        case GameSettings::Mode::SinglePlayer:
            return "Singleplayer";
        case GameSettings::Mode::MultiPlayer:
            return "Multiplayer";
        default:
            return "Unknown";
    }
}
