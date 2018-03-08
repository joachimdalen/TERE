#ifndef HIGHSCORE_DISPLAY_STATE_H
#define HIGHSCORE_DISPLAY_STATE_H

#include <SFML/Graphics/Text.hpp>

#include <state/state.h>
#include <gfx/starfield_handler.h>
#include <controls/text_button.h>
#include <gfx/wave_text.h>
#include "highscore.h"

class HighScoreDisplayState : public State
{
public:
    HighScoreDisplayState(StateStack& stack, Context context);

    virtual void			draw();
    virtual bool			update(sf::Time dt);
    virtual bool			handleEvent(const sf::Event& event);

private:

    StarfieldHandler* mStarfield;
    WaveText* waveText;
    HighScore* highScore;

    void updateMode(int mode);
    void selectHighScore();
    void deselectHighScore();
    void horizontalAction(int direction);

    sf::Text    TNames[9], TLevel[9], TScore[9], TDirection[2];
    sf::Text    TDebug;

    TextButtonPtr selection[2];

    std::string sMode[3] = { "Marathon", "Line Clear", "Sprint" };
    std::string debugText;

    int currentSelection = 0, modeSelected = 0;
    int mouseX, mouseY, hoverX;
};


#endif //HIGHSCORE_DISPLAY_STATE_H
