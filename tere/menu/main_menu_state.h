#ifndef MENU_NEW_H
#define MENU_NEW_H

#include <SFML/Graphics/Text.hpp>

#include <state/state.h>
#include <controls/text_button.h>
#include <controls/text_toggle.h>
#include <gfx/starfield_handler.h>
#include <game/tile.h>
#include <game/tetromino.h>

class MainMenuState : public State
{

public:
    MainMenuState(StateStack &stack, const Context &context);

    virtual void draw();
    virtual bool update(sf::Time dt);
    virtual bool handleEvent(const sf::Event &event);

protected:

    TextButtonPtr selection[7];

    int currentSelection = 0;
    int nDemoTime = 30; // Time in seconds before the demo starts

    StarfieldHandler* mStarfield;
    Tile* tile;
    Tetromino* tetromino;

    int centerPadding, mouseY;

    double nXShift = 0.0;

    std::string debugText;
    sf::Text    TDebug;

    sf::Clock Timer, demoTimer;
};
#endif //MENU_NEW_H
