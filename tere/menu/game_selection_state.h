#ifndef GAME_SELECTION_MENU_H
#define GAME_SELECTION_MENU_H

#include <state/state_stack.h>
#include <controls/text_button.h>
#include <controls/text_toggle.h>
#include <gfx/starfield_handler.h>
#include <gfx/wave_text.h>

class GameSelectionState : public State
{
public:
    GameSelectionState(StateStack &stack, Context &context);
    virtual void draw();

    virtual bool update(sf::Time dt);

    virtual bool handleEvent(const sf::Event &event);

protected:
    void positionItems();
    void applyChanges(GameSettings &settings);
    void horizontalAction(int playMode, int gameMode, int direction);

    TextTogglePtr mItems[9];
    TextButtonPtr mOptions[2];

    int currentSelection = 0;


    // mItems[9]
    // 0 - Play Mode, 1 - Game Mode, 2 - Level, 3 - Height, 4 - Lines, 5 - VS CPU, 6 - CPU Lvl, 7 - Map, 8 - RandomBag

    // mOptions[2]
    // 0 - play, 1 - back


    // [ Single-player, Multiplayer][Marathon, Line Clear, Battle, Sprint][ ... items ... , -1, Number of items ]

    int optionOrder[2][4][11] = {
            {
                    { 0, 1, 2, 7, 8, -1, -1, -1, -1, -1, 5},
                    { 0, 1, 2, 3, 4, 7, 8, -1, -1, -1, 7 },
                    { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0 },
                    { 0, 1, 2, 7, 8, -1, -1, -1, -1, -1, 5 },
            },
            {
                    { 0, 1, 2, 5, 6, 7, 8, -1, -1, -1, 7 },
                    { 0, 1, 2, 3, 4, 5, 6, 7, 8, -1, 9 },
                    { 0, 1, 2, 5, 6, 7, 8, -1, -1, -1, 7 },
                    { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0 },
            }
    };

    int mouseX, mouseY, hoverY, directionX;

    sf::Text TMessage;
    std::string sMessage;

    StarfieldHandler* mStarfield;
    WaveText* waveText;

    sf::Vector2f yShift;
};

#endif //GAME_SELECTION_MENU_H
