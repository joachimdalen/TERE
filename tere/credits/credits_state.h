#ifndef CREDITS_STATE_H
#define CREDITS_STATE_H

#include <SFML/Graphics/Text.hpp>

#include <state/state.h>
#include <gfx/starfield_handler.h>
#include <controls/text_button.h>
#include <gfx/cube.h>

class CreditsState : public State
{
public:

    CreditsState(StateStack& stack, Context context);

    virtual void			draw();
    virtual bool			update(sf::Time dt);
    virtual bool			handleEvent(const sf::Event& event);

private:

    StarfieldHandler* mStarfield;
    Cube* mCube;

    sf::Text    TScroller;

    float yPos;
    float scrollSpeed = 1.0f;

    sf::Clock Timer, fadeInTimer, fadeOutTimer;


    float exitTime = 1.f; // When to return to main menu, in seconds (after game over and sweep fade-out)
    float fadeTime = 1.f; // How long the fade should last for, in seconds


};

#endif //CREDITS_STATE_H
