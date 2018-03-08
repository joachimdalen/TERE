#ifndef MENU_OPTIONS_GENERAL_NEW_H
#define MENU_OPTIONS_GENERAL_NEW_H

#include <state/state.h>
#include <controls/text_button.h>
#include <controls/text_toggle.h>
#include <gfx/starfield_handler.h>
#include <gfx/wave_text.h>

class MenuOptionsGeneralState : public State
{
public:
    MenuOptionsGeneralState(StateStack &stack, Context &context);
    virtual void draw();

    virtual bool update(sf::Time dt);

    virtual bool handleEvent(const sf::Event &event);


protected:
    sf::Vector2f spacing;

    TextTogglePtr selectionToggle[10];
    TextButtonPtr selectionButton[2];

    int currentSelection = 0;
    int mouseX, mouseY, directionX;

    StarfieldHandler* mStarfield;
    WaveText* waveText;

    std::string sMessage = "";
    sf::Text    TMessage;

    sf::Clock timerMessage;

    bool bInvalidResolution = false;
};


#endif //MENU_OPTIONS_GENERAL_NEW_H
