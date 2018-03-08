#ifndef PLAYERKEYMAP_H
#define PLAYERKEYMAP_H

#include <SFML/Window.hpp>
#include <memory>

class PlayerKeyMap {
public:
    sf::Keyboard::Key Left;
    sf::Keyboard::Key Right;
    sf::Keyboard::Key Down;
    sf::Keyboard::Key RotateCounterClockwise;
    sf::Keyboard::Key RotateClockwise;
    sf::Keyboard::Key PauseGame;
};
typedef std::shared_ptr<PlayerKeyMap> PlayerKeyMapPtr;
#endif //PLAYERKEYMAP_H
