#ifndef CUSTOM_MENU_STATE_H
#define CUSTOM_MENU_STATE_H

#include <SFML/Graphics/Text.hpp>

#include <state/state.h>
#include <gfx/starfield_handler.h>
#include <gfx/wave_text.h>
#include <controls/text_button.h>

class CustomMenuState : public State
{
public:

//    MenuOptionsControlsState(StateStack &stack, Context &context);

    CustomMenuState(StateStack& stack, Context context);

    //CustomMenuState(StateStack &stack, Context &context);

    virtual void			draw();
    virtual bool			update(sf::Time dt);
    virtual bool			handleEvent(const sf::Event& event);

private:

    sf::Text    TDescription;
    StarfieldHandler* mStarfield;
    WaveText* waveText;

    TextButtonPtr mOptions[3];

/*
    ButtonPtr bagButton;
    ButtonPtr mapButton;
    ButtonPtr backButton;

    std::vector<ButtonPtr> uiElements;
*/

    int currentSelection = 0;
    int mouseY;


    sf::Clock LastUpdate;

    //int cSizeSmall;

};

#endif //CUSTOM_MENU_STATE_H
