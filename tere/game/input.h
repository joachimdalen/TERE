#ifndef INPUT_H
#define INPUT_H

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Context;
class GameState;

class Input {

public:

    Input();

    virtual ~Input();
    // Virtual destructor: Important!  If not freeing Text* pointers
    // Could leak mem if child classes allocate any

    bool handleEvent(GameState &GameState, const sf::Event& event, sf::RenderWindow &window, int cSize);

    bool joystickUtilization = false;
};

#endif //INPUT_H
