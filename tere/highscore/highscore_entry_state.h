#ifndef HIGHSCORE_ENTRY_STATE_H
#define HIGHSCORE_ENTRY_STATE_H

#include <state/state.h>
#include <gfx/starfield_handler.h>
#include <gfx/wave_text.h>
#include <controls/text_button.h>
#include "highscore.h"

class HighScoreEntryState : public State
{
public:
    HighScoreEntryState(StateStack& stack, Context context);

    virtual void			draw();
    virtual bool			update(sf::Time dt);
    virtual bool			handleEvent(const sf::Event& event);

private:

    StarfieldHandler* mStarfield;
    WaveText* waveText;
    HighScore* highScore;

    sf::Text    TDescription[3], TNames[9], TLevel[9], TScore[9];
    sf::Text    TDebug;

    TextButtonPtr selection[2];

    int nGameMode, nScore, nLevel, nPosition;
    std::string sNameEntry = "";

    int nCursorToggle = 500; // Time it takes for cursor to toggle on/off in milliseconds
    bool bCursor = true; // Is the cursor on/off ?
    sf::Clock cursorTime;

    bool bNameEntryMode = true;
};


#endif //HIGHSCORE_ENTRY_STATE_H
