#ifndef CUSTOM_MAP_STATE_H
#define CUSTOM_MAP_STATE_H

#include <SFML/Graphics/Text.hpp>

#include <state/state.h>
#include <game/tile.h>
#include <game/tetromino.h>
#include <gfx/starfield_handler.h>
#include <gfx/wave_text.h>
#include <fstream>

class SoundPlayer;

class CustomMapState : public State
{

public:
    CustomMapState(StateStack& stack, Context context);

    virtual void			draw();
    virtual bool			update(sf::Time dt);
    virtual bool			handleEvent(const sf::Event& event);

private:

    void clearField();
    void createSaveFile(bool clear);
    void loadFile();

    bool fexists(const std::string& filename);

    sf::Text    TDescription, TDebug, TNumber[4], TOption[4], TMessage;

    Tile* tile;
    Tetromino* tetromino;
    StarfieldHandler* mStarfield;
    WaveText* waveText;

    sf::Clock LastUpdate, timerMessage;

    std::string debugText = "Blank";


    int nMap[800]; // for loading the saved maps into memory ... create a new save file if this one is invalid, or does not exist

    int nField[20][10];

    int mCol[4][2] = {
            { 0, 1 },
            { 2, 3 },
            { 4, 5 },
            { 6, 7 },
    };

    int cSizeTiny, cSizeSmall, padding = 0;
    int nSelectedColor = 0, mouseX, mouseY, hoverX, hoverY;

    bool bLeftMouseButton = false;
    bool bMapSelected[4] = { true, false, false, false };
    int nMapSelected = 0;

    std::ofstream saveToFile;
    std::ifstream loadFromFile;

    SoundPlayer&			mSounds;

};

#endif //CUSTOM_MAP_STATE_H
