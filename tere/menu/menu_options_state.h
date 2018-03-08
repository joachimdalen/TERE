#ifndef MENU_OPTIONS_NEW_H
#define MENU_OPTIONS_NEW_H

#include <SFML/Graphics/Text.hpp>

#include <state/state.h>
#include <controls/text_button.h>
#include <controls/text_toggle.h>
#include <gfx/starfield_handler.h>
#include <gfx/wave_text.h>

class MenuOptionState : public State{
public:
    MenuOptionState(StateStack &stack, const Context &context);
    virtual void draw();

    virtual bool update(sf::Time dt);

    virtual bool handleEvent(const sf::Event &event);

protected:

    //sf::Text header;


    TextButtonPtr selection[3];

/*
    TextButtonPtr general;
    TextButtonPtr controls;
    TextButtonPtr back;
*/

    int currentSelection = 0;
    int mouseY;

    StarfieldHandler* mStarfield;
    WaveText* waveText;

};


#endif //TERE_MENU_OPTIONS_NEW_H
