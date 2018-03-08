#ifndef CUSTOM_BAG_STATE_H
#define CUSTOM_BAG_STATE_H

#include <SFML/Graphics/Text.hpp>

#include <state/state.h>
#include <game/tile.h>
#include <game/tetromino.h>
#include <gfx/starfield_handler.h>
#include <gfx/wave_text.h>
#include <controls/text_button.h>

class CustomBagState : public State
{
public:
    CustomBagState(StateStack& stack, Context context);

    virtual void			draw();
    virtual bool			update(sf::Time dt);
    virtual bool			handleEvent(const sf::Event& event);

private:

    void checkTetromino();
    void saveCustomBag();

    sf::Text    TDescription, TDebug;

    TextButtonPtr mOptions[3];

    Tile* tile;
    Tetromino* tetromino;
    StarfieldHandler* mStarfield;
    WaveText* waveText;

    int currentSelection = 0;
    int cursorTetromino = 0;
    bool selectedTetrominos[7] = { false, false, false, false, false, false, false };

    std::string sRandomBag;

    int centerPadding;
    int mouseY, mouseX, mouseSelected;
};

#endif //CUSTOM_BAG_STATE_H
