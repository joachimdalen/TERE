#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <vector>
#include <stdlib.h>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <state/state.h>
#include <gfx/starfield_handler.h>
#include <controls/text_button.h>
#include "tile.h"
#include "tetromino.h"
#include "text.h"
#include "ai.h"
#include "sweep.h"
#include "input.h"
#include "custom_map.h"

class SoundPlayer;

class GameState : public State
{
public:
    GameState(StateStack& stack, Context context);

    virtual void		draw();
    virtual bool		update(sf::Time dt);
    virtual bool		handleEvent(const sf::Event& event);


    int nNumberPlayers = 2;
    int nMap = -1;  // For the custom maps (-1 means no map is used)
    int nGameType = 0;

    bool bAI[2] = { false, false };
    int nAI[2]; // The AI difficulty

    bool bMovingLeft[2] = { false, false };
    bool bMovingRight[2] = { false, false };
    bool bMovingDown[2] = { false, false };
    bool bRotatingCCW[2] = { false, false };
    bool bRotatingCW[2] = { false, false };
    bool bJoystick[2] = { false, false };
    bool bGameOver[2] = { false, false };
    bool bWinner[2] = { false, false };

    int bFadeIn = false;
    int bFadeOut = false;

    int nEntryDelay[2] = { 18, 18 };    // ARE (Entry Delay) initially set to 18 frames, (short break before new piece starts to drop)
    int nDASCounter[2] = { 0, 0 };      // Frame counter for Delayed Auto Shift (Movement left and right)

    int nAIDASCounter[3] = { 10, 12, 16 }; // AI has different DAS depending on difficulty

    int nSoftDropCounter[2] = { 0, 0 }; // Frame counter for soft drops (soft drop every second frame)
    int nSoftDropPoints[2] = { 0, 0 };  // If a player soft drops until piece is locked, award points for it

    sf::Keyboard::Key kInput[2][5]; // For passing the keyboard settings to input.cpp
    sf::Keyboard::Key kPause;

    int pauseTime = 0;
    bool bGamePaused = false;
    bool bPauseMusic = false;
    bool bPauseSound = false;
    bool bActiveQuit = false;
    bool bDemoMode = false;

    TextButtonPtr pauseButton[3];
    int currentSelection;

    sf::Clock timerSprint; // for Single Player Game D (Sprint)

    int mouseY;

private:

    // ============================================================================================
    //  Game Type Variables

    int nSelectedHeight = 2; // TODO Inject   Height 0-5 for Game B
    int nHeight[6] = { 0, 3, 5, 8, 10, 12 };  // Height of garbage lines for Game B (Height 0-5)
    int nLineClear[5] = { 10, 25, 50, 80 }; // Lines to clear in Game B

    AI* ai;
    CustomMap* custom;
    Sweep* sweep;
    Tile* tile;
    Tetromino* tetromino;
    Text* text;
    Input* input;
    StarfieldHandler* mStarfield;

    void setPieces(int i);
    void getNextPiece(int i);
    bool doesPieceFit(int player, int nTetromino, int nRotation, int nPosX, int nPosY);
    void endMatch(int player);
    void insertGarbage(int player, int lines);
    void filterHighScore();

    sf::Vector2f getSoundPosition(int player);

    std::string debugText, moreDebugText, joyDebugText;
    sf::Clock LastUpdate, timeOutTimer;

    bool bAIassigned[2] = { false, false }; // Orders given to the AI

    // ============================================================================================
    //  Trails Variables

    bool bTrails = true;
    int nTrailDelay = 1;
    int nTrailIntensity = 192;
    // A higher value means less intense, since it is a divisor (value 9 = 100%)
    // (value 192 is default)

    int nTrailDelayArray[4] = { 1, 2, 4, 8 };
    int nTrailIntensityArray[4] = { 192, 96, 48, 24 };

    // ============================================================================================

    float exitTime = 20.f; // When to return to main menu, in seconds (after game over and sweep fade-out)
    float fadeTime = 1.f; // How long the fade should last for, in seconds

    int nFieldWidth = 12;
    int nFieldHeight = 22; // Oversized playfield prevents L and J tetrominos from "hooking" on top of well

    int nGlobalFrameCounter = 0;

    // Gravity speed from level 0 up to and including level 29
    int nGravity[30] = {
            48, 43, 38, 33, 28, 23, 18, 13, 8, 6, 5,    // 0 through 10
            5, 5, 4, 4, 4, 3, 3, 3, 2, 2,               // 11 through 20
            2, 2, 2, 2, 2, 2, 2, 2, 1 };                // 21 through 29+

    unsigned char *tempField[2] = { nullptr, nullptr };
    unsigned char *pField[2] = { nullptr, nullptr };
    unsigned char *fxField[2] = { nullptr, nullptr }; // For trails after pieces move, etc.

    // Game Logic

    std::string sRandomBag = "0123456"; //  ALL PIECES

    std::vector<int> randomBag[2];
    std::vector<int> vLines[2];

    int nCurrentPiece[2];       // The player's current piece in play
    int nNextPiece[2];          // Player's next piece
    int nCurrentRotation[2];    // Which Tetromino to use, from tetromino.cpp
    int nCurrentX[2];
    int nCurrentY[2];

    int nPieceNumber[2] = { 0, 0 };

    int nScore[2] = { 0, 0 };
    int nLevel[2] = { 0, 0 };
    int nLines[2] = { 0, 0 };
    int nFinalTime = 0; // Final time in Game D, this goes to the high score check

    int nNextLevel[2];  // Counter that keeps track of how many lines remains until player reaches next level
    int nPunishment[2] = { 0, 0 }; // Punishment lines for player in Game C

    int nGravityCounter[2] = { 0, 0 };  // Counter for when the game should force the piece one line down
    int nLineRemovalStep[2] = { 0, 0 };
    int nLineRemovalFrame[2] = { 0, 0 };  // Keep draw (which works faster than update) from doing multiple operations during one single frame
    int nTrailRemovalFrame[2] = { 0, 0 };

    bool bTetrisFlash[2] = { false, false };
    bool bForceDown[2] = { false, false }; // Tell game that nGravity has caused piece to drop one row
    bool bRotateHold[2] = { true, true };  // Keep the game from spinning a Tetromino when holding down the rotate button

    std::string sMessage[2]; // for the fade-out messages in the next piece box

    SoundPlayer&			mSounds;
};

#endif //GAMESTATE_H
