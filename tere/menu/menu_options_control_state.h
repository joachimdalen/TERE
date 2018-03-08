#ifndef MENU_CONTROLS_NEW_H
#define MENU_CONTROLS_NEW_H

#include <state/state.h>
#include <controls/text_key_mapper.h>
#include <controls/text_button.h>
#include <gfx/starfield_handler.h>
#include <gfx/wave_text.h>

class MenuOptionsControlsState : public State
{

public:
    MenuOptionsControlsState(StateStack &stack, Context &context);
    virtual void draw();
    virtual bool update(sf::Time dt);
    virtual bool handleEvent(const sf::Event &event);

protected:
    sf::Texture controllerTextures[6];
    sf::Sprite controllerSprite[6];

    sf::Text subHeader;
    bool keyMapInProgress;
    int currentSelection = 0;
    std::vector<TextKeyMapperPtr> mappableControls;

    TextKeyMapperPtr mKeys[11];
    TextButtonPtr mOptions[2];

    int mouseY;
    std::string sMessage = "Press Return to remap the key for the selected action.";

    StarfieldHandler* mStarfield;
    WaveText* waveText;

    sf::Clock timerMessage;
};

#endif //MENU_CONTROLS_NEW_H
