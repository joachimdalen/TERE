#include <iostream>
#include <cmath>
#include <thread>
#include <iomanip>
#include <random>
#include <algorithm>

#include <SFML/Window/Joystick.hpp>

#include "game_state.h"
#include <resource/resource_holder.h>
#include <utility/utility.h>
#include "sound/music_player.h"
#include "sound/sound_player.h"


GameState::GameState(StateStack& stack, Context context) : State(stack, context)
        , mSounds(*context.sounds)
{
    srand(time(NULL));

    sf::Font& font = context.fonts->get(Fonts::Prime);
    sf::RenderWindow &window = *getContext().window;

    if(context.settings->bDemoMode) // Demo mode when idling in the main menu for 30 seconds
        bDemoMode = true;

    nNumberPlayers = context.settings->players;
    nGameType = context.settings->gameType;

    nMap = context.settings->map; // Are we playing with a preset/custom map?

    switch(context.settings->randomBagIndex)
    {
        case 0 : sRandomBag = "0123456"; break;
        case 1 : sRandomBag = "56"; break;
        case 2 : sRandomBag = "0134"; break;
        case 3 : sRandomBag = context.settings->randomBag; break;
    }

    if(sRandomBag.length() == 1)
        sRandomBag += sRandomBag;

    if(!bDemoMode)
        bAI[1] = context.settings->vsCpu;
    else
    {
        // Demo Mode
        bAI[0] = bAI[1] = true; // Two AI players
        nNumberPlayers = 2;
        nGameType = 0; // Marathon
        nMap = -1; // No map
        sRandomBag = "0123456"; // Default random bag
    }

    if(bAI[1] || bAI[0])
    {
        if(bDemoMode)
            nAI[1] = 2; // Hard difficulty in demo mode
        else
            nAI[1] = context.settings->aiLevel; // AI level

        nAI[0] = nAI[1]; // for testing purposes
    }

    kInput[0][0] = context.settings->playerOneKeyMap->Left;
    kInput[0][1] = context.settings->playerOneKeyMap->Right;
    kInput[0][2] = context.settings->playerOneKeyMap->Down;
    kInput[0][3] = context.settings->playerOneKeyMap->RotateCounterClockwise;
    kInput[0][4] = context.settings->playerOneKeyMap->RotateClockwise;

    kInput[1][0] = context.settings->playerTwoKeyMap->Left;
    kInput[1][1] = context.settings->playerTwoKeyMap->Right;
    kInput[1][2] = context.settings->playerTwoKeyMap->Down;
    kInput[1][3] = context.settings->playerTwoKeyMap->RotateCounterClockwise;
    kInput[1][4] = context.settings->playerTwoKeyMap->RotateClockwise;

    kPause = context.settings->playerOneKeyMap->PauseGame;

    bTrails = context.settings->trailsEnabled;
    nTrailDelay = nTrailDelayArray[ context.settings->trailsDelay - 1 ];
    nTrailIntensity = nTrailIntensityArray[ context.settings->trailsIntensity - 1 ];

    mStarfield = new StarfieldHandler(window.getSize().x, window.getSize().y);
    input = new Input();
    ai = new AI(nFieldWidth, nFieldHeight, nGameType, nAI[1]);
    custom = new CustomMap;
    sweep = new Sweep(nFieldWidth, nFieldHeight);

    sweep->fadeIn();
    bFadeIn = true;

    tetromino = new Tetromino();

    tile = new Tile(window, nFieldHeight, false);
    text = new Text(font, tile->tileSize, nNumberPlayers);

    context.music->play(Music::GameTheme); // Play game theme

    pauseButton[0] = TextButtonPtr(new TextButton("GAME PAUSED", font));
    pauseButton[0]->setPosition(sf::Vector2f( window.getSize().x / 2, window.getSize().y / 2 - context.settings->cSize * 2.6f));
    pauseButton[0]->setSelect();

    pauseButton[1] = TextButtonPtr(new TextButton("Return to game", font));
    pauseButton[1]->setPosition(sf::Vector2f( window.getSize().x / 2, window.getSize().y / 2 ));

    pauseButton[2] = TextButtonPtr(new TextButton("Exit to Main Menu", font));
    pauseButton[2]->setPosition(pauseButton[1]->getPosition() + sf::Vector2f(0.f, context.settings->cSize * 1.3f));


    // Initialize variables for both players
    for(int player = 0; player < nNumberPlayers; player++)
    {
        // From settings
        nLevel[player] = context.settings->level;

        sMessage[player] = tetromino->constructRandomScrollText(player); // Scroll a random character when the game starts

        pField[player] = new unsigned char[nFieldWidth * nFieldHeight];  // Create play field buffer
        tempField[player] = new unsigned char[nFieldWidth * nFieldHeight];
        fxField[player] = new unsigned char[nFieldWidth * nFieldHeight];

        for(int x = 0; x < nFieldWidth; x++) // Board Boundary
        {
            for(int y = 0; y < nFieldHeight; y++)
            {
                pField[player][y * nFieldWidth + x] =
                        (x == 0 || x == nFieldWidth -1 || y == nFieldHeight - 1) ? 8 : 0;

                if(nMap > -1)
                    pField[player][y * nFieldWidth + x] = custom->getMapLocation(nMap, x, y);

                fxField[player][y * nFieldWidth + x] = 0;
            }
        }

        if(nGameType == 1)
        {
            nSelectedHeight = context.settings->height;

            //  Insert garbage lines for Game B
            for(int y = nFieldHeight - 1 - nHeight[nSelectedHeight]; y < nFieldHeight - 1; y++)
            {
                int amount = rand() % 5 + 2;   // from 2 to 6 garbage pieces per line
                std::vector<int> positionVector;

                for(int i = 1; i < 11; i++) positionVector.push_back(i);

                for(int x = 0; x < amount; x++)
                {
                    int choice = rand() % positionVector.size();
                    int xpos = positionVector.at( choice );

                    positionVector.erase( positionVector.begin() + choice );
                    pField[player][y * nFieldWidth + xpos] = 10;
                }
            }

            nLines[player] = context.settings->lines;

        }

        if(nGameType == 3) // Game D (Sprint)
        {
            nLines[player] = 40; // Hard Limit, it always is 40
            //nLines[player] = 2; // (Debug)
        }

        getNextPiece(player);

        // For Marathon (A-Type), when the player line clears:
        //   (startLevel * 10 + 10)    or,    max(100, (startLevel * 10 - 50))   whichever comes first
        //   the level advances by 1.  After this, the level advances by 1 for every 10 lines.
        nNextLevel[player] = std::min(nLevel[player] * 10 + 10, std::max(100, nLevel[player] * 10 - 50));
    }
}


void GameState::setPieces(int player)
{
    nCurrentPiece[player] = randomBag[player].at(0);      // pick the first number from bag as current piece
    randomBag[player].erase(randomBag[player].begin());   // remove piece from randomBag vector
    nNextPiece[player] = randomBag[player].at(0);         // the next piece in line
    nPieceNumber[player]++;
}


void GameState::getNextPiece(int player)
{
    std::vector<int> tempVector;

    if(randomBag[player].size() < sRandomBag.length())
    {
        // make a new shuffled bag of random pieces (0-6), and add to randomBag
        // in accordance with the Tetris guideline rules

        for(int i = 0; i < sRandomBag.length(); i++)
            tempVector.push_back( (int)sRandomBag.at(i) - 48 );

        std::random_shuffle(tempVector.begin(), tempVector.end() );

        for(int i = 0; i < sRandomBag.length(); i++)
            randomBag[player].push_back( (int)tempVector.at(i) );

        setPieces(player);
    }
    else
        setPieces(player);

    nCurrentX[player] = nFieldWidth / 2 - 1;

    if(nCurrentPiece[player] < 2) nCurrentX[player]--; // Shift O- and I-piece one column left

    if(nCurrentPiece[player] == 1) nCurrentY[player] = -1; // Shift I-piece one row up
    else nCurrentY[player] = 0;

    nCurrentRotation[player] = 0;
}


bool GameState::doesPieceFit(int player, int nTetromino, int nRotation, int nPosX, int nPosY)
{
    for(int px = 0; px < 4; px++) // All fields > 0 are occupied
    {
        for(int py = 0; py < 4; py++)
        {
            int fi = (nPosY + py) * nFieldWidth + (nPosX + px); // Get index into field

            // Check that test is in bounds. Note that out of bounds does not necessarily mean a fail, as the long
            // vertical piece can have cells that lie outside the boundary, so we will just ignore them
            if(nPosX + px >= 0 && nPosX + px < nFieldWidth)
            {
                if(nPosY + py >= 0 && nPosY + py < nFieldHeight)
                {
                    // In bounds so do collision check
                    if(tetromino->tetromino[nTetromino][nRotation][py * 4 + px] != L'.' && pField[player][fi] != 0)
                        return false; // fail on first hit
                }
            }
        }
    }
    return true;
}


void GameState::draw()
{
    sf::RenderWindow& window = *getContext().window;
    window.setView(window.getDefaultView());

    int nWidth = window.getSize().x;    // The width of the display in pixels
    int nHeight = window.getSize().y;   // The height of the display in pixels

    int nWidthPadding = 0;

    if( (nWidth * 1.0f / nHeight * 1.0f) > 1.33334) // Add padding on the side for wide displays
        nWidthPadding = (nWidth - (nHeight / 3 * 4)) / 2;

    if(nNumberPlayers == 1)
        nWidthPadding += (nHeight / 3 * 4 - 15 * tile->tileSize) / 2; // Center everything if there is only one player

    mStarfield->draw(window, getContext().settings->starfieldEnabled); // draw the starfield first

    sf::RectangleShape mRectangle; // Used for making the starfield less visible
    mRectangle.setFillColor(sf::Color(0, 0, 0, 223));


    // Draw both players
    for(int player = 0; player < nNumberPlayers; player++)
    {
        mRectangle.setSize(sf::Vector2f(tile->tileSize, tile->tileSize));

        if(nLineRemovalStep[player] > 0)
        {
            if(!bGamePaused)
            {
                if(nGlobalFrameCounter % 4 == 0 && nLineRemovalFrame[player] != nGlobalFrameCounter)
                {
                    if(vLines[player].size() == 4 && nLineRemovalStep[player] > 1) // Flash here for Tetris
                        bTetrisFlash[player] = true;

                    nLineRemovalFrame[player] = nGlobalFrameCounter;
                    nLineRemovalStep[player]--;

                    for(auto &v : vLines[player])
                    {
                        for(int px = nFieldWidth / 2 - (6 - nLineRemovalStep[player]);
                            px < nFieldWidth / 2 + (6 - nLineRemovalStep[player]); px++)
                        {
                            if(px > 0 && px < nFieldWidth)
                                pField[player][v * nFieldWidth + px] = 8;
                        }
                    }
                }
                else if(nGlobalFrameCounter % 4 != 0)
                    bTetrisFlash[player] = false; // No flash here for Tetris
            }
        }

        // Draw Field
        for(int x = 1; x < nFieldWidth - 1; x++)
        {
            for (int y = 1; y < nFieldHeight - 1; y++)
            {
                int fx = 0;

                if (bTrails)
                {
                    fx = fxField[player][y * nFieldWidth + x];

                    if(nTrailRemovalFrame[player] != nGlobalFrameCounter)
                        if (!bGamePaused && fx % 25 != 0 && nGlobalFrameCounter % nTrailDelay == 0) // Reduce every few frames
                            fxField[player][y * nFieldWidth + x]--;
                }

                sf::Color bg = tile->mColors[7]; // The background color, used for trails in all modes and warnings in Game C

                if(nGameType == 2) // In Game C
                {
                    // Make the background of the victim of an attack turn slightly red just before receiving lines
                    if(nLineRemovalStep[(player + 1) % 2] > 0 && vLines[(player + 1) % 2].size() > 1)
                    {
                        int mRed = (6 - nLineRemovalStep[(player + 1) % 2]) * 5 * (vLines[(player + 1) % 2].size() - 1);

                        bg.r = bg.r + mRed;
                        bg.g = bg.g - mRed / 2;
                        bg.b = bg.b - mRed / 2;
                        bg.a = bg.a + mRed / 2;
                    }
                }

                if ((int) pField[player][y * nFieldWidth + x] == 0) // Blank space, or a trail
                {
                    tile->mTile.setFillColor(bg);

                    if (bTrails)
                    {
                        if (fx % 25 != 0) // This position still has a trail from having a Tetromino move through it
                        {
                            sf::Color fg = tile->mColors[fx / 25];

                            sf::Color mixed = sf::Color(
                                    (bg.r + fg.r) / 2,
                                    (bg.g + fg.g) / 2,
                                    (bg.b + fg.b) / 2,
                                    (bg.a + (255 - bg.a) * (fx % 25) /
                                            nTrailIntensity)
                            );
                            tile->mTile.setFillColor(mixed);
                        }
                    }

                    if (bTetrisFlash[player]) // Flashing background every four frames when player gets Tetris
                        tile->mTile.setFillColor(tile->mColors[9]);
                } else
                    tile->mTile.setFillColor(tile->mColors[pField[player][y * nFieldWidth + x] - 1]);

                tile->mTile.setPosition(x * tile->tileSize, (y - 1) * tile->tileSize);

                tile->mTile.move((nWidth - nWidthPadding - tile->tileSize * nFieldWidth) * player +
                                 nWidthPadding * ((player + 1) % 2),
                                 (nHeight - (nFieldHeight - 2) * tile->tileSize) / 2);

                // Add a box to make the starfield less visible under the playing field
                mRectangle.setPosition( (nWidth - nWidthPadding - tile->tileSize * nFieldWidth) * player +
                                        nWidthPadding * ((player + 1) % 2) + x * tile->tileSize ,
                                        (nHeight - (nFieldHeight - 2) * tile->tileSize) / 2 + (y - 1) * tile->tileSize);

                if (sweep->Visible(x, y))
                {
                    window.draw(mRectangle);
                    window.draw(tile->mTile);
                }
            }
        }

        nTrailRemovalFrame[player] = nGlobalFrameCounter;

        // Draw Next Piece

        int x; // Used for positioning next piece

        if(player == 0)
            x = tile->tileSize * (nFieldWidth) + nWidthPadding;
        else x = (nWidth - nWidthPadding - tile->tileSize * (nFieldWidth + 4));

        // Make starfield less visible underneath the next piece
        mRectangle.setSize(sf::Vector2f(tile->tileSize * 4, tile->tileSize * 4));
        mRectangle.setPosition( x,
                (nHeight - (nFieldHeight - 2) * tile->tileSize) / 2 + player * tile->tileSize * (nFieldHeight - 6));
        window.draw(mRectangle);

        for(int px = 0; px < 4; px++)
        {
            for(int py = 0; py < 4; py++)
            {
                wchar_t temp;

                if(bGameOver[player] || bFadeIn)
                {
                    int index = sweep->percentageFinished() * (sMessage[player].size() - 1) * 5 / 10000;
                    temp = tetromino->getCharacterFromScrolltext(player, index + px, py);
                }
                else
                    temp = tetromino->tetromino[nNextPiece[player]][0][py * 4 + px];

                if(temp != L'.')
                {
                    if(!bGameOver[player])
                    {
                        if(bFadeIn)
                            tile->mTile.setFillColor( tile->mColors[ 8 ]); // Dark Blue
                        else
                            tile->mTile.setFillColor( tile->mColors[ nNextPiece[player]] );
                    }
                    else
                    {
                        if(bWinner[player])
                            tile->mTile.setFillColor( tile->mColors[ 5 ]); // Green (Thumbs up for winning)
                        else
                            tile->mTile.setFillColor( tile->mColors[ 6 ]); // Red (Thumbs down for losing)
                    }
                }
                else
                    tile->mTile.setFillColor( tile->mColors[ 7 ] );

                tile->mTile.setPosition( px * tile->tileSize, py * tile->tileSize);

                tile->mTile.move(x,
                                 (nHeight - (nFieldHeight - 2) * tile->tileSize) / 2 +
                                 player * tile->tileSize * (nFieldHeight - 6) );

                window.draw(tile->mTile);
            }
        }


        if(nLineRemovalStep[player] == 0 && !bWinner[player] && !bFadeIn)
        {
            // Draw Ghost Piece
            bool ghostPieceFits = true;
            int gy = nCurrentY[player];

            while(ghostPieceFits)
            {
                ghostPieceFits = doesPieceFit(
                        player, nCurrentPiece[player], nCurrentRotation[player], nCurrentX[player], gy);
                gy++;
            }

            gy -= 2;

            for(int px = 0; px < 4; px++)
            {
                for(int py = 0; py < 4; py++)
                {
                    wchar_t temp = tetromino->tetromino[nCurrentPiece[player]][nCurrentRotation[player]][py * 4 + px];

                    if(temp != L'.')
                    {
                        tile->mTile.setFillColor( tile->mColors[8] );
                        tile->mTile.setPosition((nCurrentX[player] + px) * tile->tileSize, (gy + py - 1) * tile->tileSize);

                        tile->mTile.move( (nWidth - nWidthPadding - tile->tileSize * nFieldWidth) * player +
                                          nWidthPadding * ((player + 1) % 2),
                                          (nHeight - (nFieldHeight - 2) * tile->tileSize) / 2);

                        if(!bGameOver[player])  // Do not draw ghost piece after Game Over
                        {
                            // Only draw a visible ghost piece that are visible on the board
                            if(tile->mTile.getPosition().y >= (nHeight - (nFieldHeight - 1) * tile->tileSize) / 2)
                                window.draw(tile->mTile);
                        }
                    }
                }
            }

            // Draw Current Piece
            for(int px = 0; px < 4; px++)
            {
                for(int py = 0; py < 4; py++)
                {
                    wchar_t temp = tetromino->tetromino[nCurrentPiece[player]][nCurrentRotation[player]][py * 4 + px];

                    if(temp != L'.')
                    {
                        tile->mTile.setFillColor( tile->mColors[nCurrentPiece[player]] );
                        tile->mTile.setPosition((nCurrentX[player] + px) * tile->tileSize, (nCurrentY[player] + py - 1) * tile->tileSize);

                        if(bTrails && !(nGameType == 1 && nLines[player] == 0))
                        {
                            // This position has been visited by a falling Tetromino, set lifetime of the fx field trail
                            // to its color (current piece) * 10 + 9, where 9 means max intensity (will go down to 0)

                            int pos = (nCurrentY[player] + py) * nFieldWidth + nCurrentX[player] + px;
                            int c = nCurrentPiece[player] * 25 + 24;

                            fxField[player][pos] = c;

                            if(bMovingDown[player] && pos > nFieldWidth)
                                fxField[player][pos - nFieldWidth] = c; // Hide the fact that Tetrominos can fall 2 steps in one frame
                        }

                        tile->mTile.move( (nWidth - nWidthPadding - tile->tileSize * nFieldWidth) * player +
                                          nWidthPadding * ((player + 1) % 2),
                                          (nHeight - (nFieldHeight - 2) * tile->tileSize) / 2);

                        int sweepY = nCurrentY[player] + py - 1;
                        if(sweepY < 1) sweepY = 1;

                        // Only draw tetrominos that are visible on the board
                        if(tile->mTile.getPosition().y >= (nHeight - (nFieldHeight - 1) * tile->tileSize) / 2)
                        {
                            if(sweep->Visible(  nCurrentX[player] + px, sweepY) )
                            {
                                if( !(nGameType == 1 && nLines[player] == 0) )
                                    window.draw(tile->mTile);
                            }
                        }
                    }
                }
            }
        }

        std::string debug = debugText + moreDebugText + "\n";

        //  Draw the text from the text class
        if(nScore[player] > 999999999) nScore[player] = 999999999; // Homage to the NES version, that maxed at 999,999

        std::string sTime = "";

        if(nGameType == 3) // Get a timer string for Sprint Mode
        {
            if(nFinalTime == 0)
            {
                sTime = timerString(pauseTime + timerSprint.getElapsedTime().asMilliseconds(), 0);

                if(bGamePaused) sTime = timerString(pauseTime, 0);
                if(bFadeIn) sTime = timerString(0, 0);
            }
            else
                sTime = timerString(pauseTime + nFinalTime, 0);
        }

        bool bDemoFlasher = false;

        if(bDemoMode)
        {
            if( nGlobalFrameCounter / 60  % 2 == 0 )
                bDemoFlasher = true;
        }

        text->draw(window, player, tile->tileSize, debug, bGameOver[player], bFadeIn, bFadeOut, nScore[player], nLevel[player],
                   nLines[player], nFieldWidth, nFieldHeight, nWidth, nHeight, nWidthPadding, sTime, bDemoFlasher);

        // Line removal
        if(!vLines[player].empty() && nLineRemovalStep[player] == 0)
        {
            for(auto &v : vLines[player]) // Remove the lines, since the animation has finished
            {
                for(int px = 1; px < nFieldWidth - 1; px++)
                {
                    for(int py = v; py > 0; py--)
                        pField[player][py * nFieldWidth + px] = pField[player][(py -1) * nFieldWidth + px];

                    pField[player][px] = 0; // Fixed
                }
            }

            if(nGameType == 2) // In Game C, completing lines will insert garbage for the opponent
                nPunishment[ (player + 1) % 2 ] = vLines[player].size();

            vLines[player].clear();
        }
    } // End draw player


    if(timeOutTimer.getElapsedTime().asSeconds() > exitTime - fadeTime) // Fade out the screen and music before time-out to main menu
    {
        mRectangle.setSize(sf::Vector2f(nWidth, nHeight));
        mRectangle.setPosition(0, 0);

        float factor = (timeOutTimer.getElapsedTime().asMilliseconds() - (exitTime - fadeTime) * 1000.f) / (fadeTime * 1000.f) ;
        if(factor > 1.f) factor = 1.f;

        mRectangle.setFillColor(sf::Color(0, 0, 0, factor * 255));

        if(getContext().settings->musicEnabled)
            getContext().music->setVolume( getContext().settings->musicVolume * (1.f - factor) );

        window.draw(mRectangle);
    }


    if(bGamePaused) // Draw buttons if the game is paused
    {
        mRectangle.setSize(sf::Vector2f(nWidth, getContext().settings->cSize * 7.8f));
        mRectangle.setPosition( 0, nHeight / 2 - getContext().settings->cSize * 3.9f);
        window.draw(mRectangle);

        for(int i = 0; i < 3; i++)
            pauseButton[i]->draw(window);
    }
}


void GameState::insertGarbage(int player, int lines)
{
    int punishment[5] = { 0, 0, 1, 2, 4 };  // number of lines being sent when player completes lines
    int blank = rand() % 10 + 1; // Random blank spot in line(s) being sent

    for(int l = 0; l < punishment[lines]; l++) {

        // Shift the current contents of the pField one line up
        for(int y = 1; y < nFieldHeight - 1; y++)
            for(int x = 1; x < nFieldWidth - 1; x++)
                pField[player][(y - 1) * nFieldWidth + x] = pField[player][y * nFieldWidth + x];

        for(int x = 1; x < nFieldWidth - 1; x++)
            pField[player][ (nFieldHeight - 2) * nFieldWidth + x ] = 10; // Insert garbage line at the bottom

        pField[player][ (nFieldHeight - 2) * nFieldWidth + blank ] = 0; // Except for the blank spot
    }

    if(lines > 1)
        mSounds.play(SoundEffect::Punishment, getSoundPosition(player)); // Sound for Punishment
}


void GameState::endMatch(int player)
{
    if(nGameType == 3) // Since there is no timer.stop(), have to do something else...
        nFinalTime = timerSprint.getElapsedTime().asMilliseconds();

    bGameOver[player] = true;
    bGameOver[(player + 1) % 2] = true;
    bWinner[player] = true;

    sweep->fadeOut();
    bFadeOut = true;

    //  When the game ends, each player gets a string of characters that will play in their next piece box
    //  Could potentially be used for animations as well

    sMessage[player] = text->winnerMessage();
    sMessage[(player + 1) % 2] = text->loserMessage();

    tetromino->constructScrollText(player, sMessage[player]);
    tetromino->constructScrollText( (player + 1) % 2, sMessage[(player + 1) % 2]);

    // Multiplayer messages
    text->TGameOver[player].setString("You\nWon!");
    text->TGameOver[(player + 1) % 2].setString("You\nLost!");



    if(nNumberPlayers == 1) // Single-player messages
    {
        if(bWinner[0])
            text->TGameOver[0].setString("Success!");
        else
            text->TGameOver[0].setString("Game\nOver!");
    }

    // Play sounds accordingly
    if((nNumberPlayers == 1 && ((player + 1) % 2) == 0) || nNumberPlayers == 2)
        mSounds.play(SoundEffect::Lose, getSoundPosition((player + 1) % 2)); // Sound for Losing

    if((nNumberPlayers == 1 && player == 0) || nNumberPlayers == 2)
        mSounds.play(SoundEffect::Win, getSoundPosition(player)); // Sound for Winning

    timeOutTimer.restart(); // Used for returning to main menu / high score entry

}


sf::Vector2f GameState::getSoundPosition(int player)
{
    // In two-player, play the sound effects slightly to the left and right for the corresponding player
    // otherwise, play the sound effect in the center.

    sf::Vector2f mVector;

    if(nNumberPlayers == 1)
        mVector.x = 0.0f;
    else
        mVector.x = -1.5f + 3.0f * player;

    return mVector;
}


bool GameState::update(sf::Time dt)
{
    // Fix that will pause the game if window is moved, after window has been moved
    // there will be one very long 'elapsed time' followed by many with 0 ms - this
    // will skip all the 0 ms updates

    if(LastUpdate.getElapsedTime().asMilliseconds() == 0) return true;


    if(timeOutTimer.getElapsedTime().asSeconds() > exitTime)
    {
        filterHighScore();
    }

    if((!bGameOver[0] && !bGameOver[1]) || bFadeOut)
        timeOutTimer.restart();

    if(bGamePaused) return true; // Do not update if the game is paused



    debugText = "Frame: " + toString(nGlobalFrameCounter) + "\tLastUpdate: " + toString(LastUpdate.getElapsedTime().asMilliseconds()) + " ms\n";

    mStarfield->update(); // Update the starfield

    nGlobalFrameCounter++;

    if(bFadeIn)
    {
        bFadeIn = sweep->performSweep();

        if(nGameType == 3) // Start the timer in Game D (Single Player only)
            timerSprint.restart();
    }

    if(bFadeOut) bFadeOut = sweep->performSweep();

    // Main Loop for both players
    for(int player = 0; player < nNumberPlayers; player++)
    {
        if(nGameType == 1 || nGameType == 3)
        {
            // In Game B and Game D, the first player who reaches 0 lines wins!
            if(nLines[player] == 0 && nLineRemovalStep[player] == 0 && !bWinner[ (player + 1) % 2 ] && !bWinner[player])
                endMatch(player);
        }

        if(!bGameOver[player] && nLineRemovalStep[player] == 0 && !bFadeIn)
        {
            if(nPunishment[player] > 0) // Player has punishment lines coming (Game C)
            {
                insertGarbage( player, nPunishment[player] );
                nPunishment[player] = 0;
            }

            int playerLevel = nLevel[player];
            if(playerLevel > 28) playerLevel = 29; // Same drop speed for all levels over Level 28

            if(nEntryDelay[player] > 0)
            {
                nEntryDelay[player]--;
                bForceDown[player] = false;
            }
            else
            {
                nGravityCounter[player]++;
                bForceDown[player] = (nGravityCounter[player] > nGravity[ playerLevel ] );
            }

            // Handle player movement

            if(bAI[player])  //  Determine the AI movement
            {
                if(!bAIassigned[player])
                {
                    // Give orders to the AI, once per piece

                    for(int i = 0; i < nFieldWidth * nFieldHeight; i++)
                        tempField[player][i] = pField[player][i];

                    //nDASCounter[player] = 16;  //  AI always has DAS charge
                    nDASCounter[player] = nAIDASCounter[ nAI[player] ];

                    bool fail = false;

                    if(nAI[player] < 2 && rand() % 10 < (4 - nAI[player] * 2))
                        fail = true;

                    moreDebugText = ai->InitMove( player, tempField[player], nCurrentPiece[player], nNextPiece[player],
                                                  nCurrentX[player], nCurrentY[player], fail);

                    bAIassigned[player] = true;
                }

                if(nCurrentRotation[player] != ai->bestRotation[player])
                {
                    if(ai->bestRotation[player] == 3)
                        bRotatingCCW[player] = true;
                    else
                        bRotatingCW[player] = true;
                }

                if(nCurrentX[player] > ai->bestMove[player])
                {
                    bMovingLeft[player] = true;
                    //nDASCounter[player] = 16; // Lets the AI move instantly - Cheating!
                }
                else if(nCurrentX[player] < ai->bestMove[player])
                {
                    bMovingRight[player] = true;
                    //nDASCounter[player] = 16; // Cheating!
                }
                else {

                    bMovingDown[player] = false;

                    //if(nGlobalFrameCounter % (3 - nAI[player]) == 0)
                    if(nGlobalFrameCounter % (7 - nAI[player] * 3) == 0) {
                        bMovingDown[player] = true;

                        if(nAI[player] < 2)
                            nSoftDropPoints[player] = 0;
                    }
                }
            }


            // Rotate, but latch to stop wild spinning
            if(bRotatingCW[player] || bRotatingCCW[player])
            {
                int attemptRotation;

                if(bRotatingCW[player])
                {
                    attemptRotation = (nCurrentRotation[player] + 1) % 4;

                    nCurrentRotation[player] += (bRotateHold[player] &&
                                                 doesPieceFit(player, nCurrentPiece[player], attemptRotation,
                                                              nCurrentX[player], nCurrentY[player])) ? 1 : 0;

                    nCurrentRotation[player] = nCurrentRotation[player] % 4;
                }
                else if(bRotatingCCW[player])
                {
                    attemptRotation = (nCurrentRotation[player] - 1);
                    if(attemptRotation < 0) attemptRotation = 3;

                    nCurrentRotation[player] = (bRotateHold[player] &&
                                                doesPieceFit(player, nCurrentPiece[player], attemptRotation,
                                                             nCurrentX[player], nCurrentY[player]))
                                               ? attemptRotation : nCurrentRotation[player];
                }
                bRotateHold[player] = false;

                if(attemptRotation == nCurrentRotation[player])
                    mSounds.play(SoundEffect::Rotate, getSoundPosition(player));
            }
            else
                bRotateHold[player] = true;


            if(bMovingRight[player])
            {
                if(doesPieceFit(player, nCurrentPiece[player], nCurrentRotation[player],
                                nCurrentX[player] + 1, nCurrentY[player]))
                {
                    if(nDASCounter[player] == 0) {
                        nCurrentX[player]++;
                        mSounds.play(SoundEffect::Move, getSoundPosition(player));
                    }

                    nDASCounter[player]++;

                    if(nDASCounter[player] > 16)
                    {
                        nCurrentX[player]++;
                        mSounds.play(SoundEffect::Move, getSoundPosition(player));
                        nDASCounter[player] = 10;
                    }
                }
                else { // Piece does not fit, "DAS wall charge"
                    nDASCounter[player] = 16;
                }
            }

            if(bMovingLeft[player])
            {
                if(doesPieceFit(player, nCurrentPiece[player], nCurrentRotation[player],
                                nCurrentX[player] - 1, nCurrentY[player]))
                {
                    if(nDASCounter[player] == 0)
                    {
                        nCurrentX[player]--;
                        mSounds.play(SoundEffect::Move, getSoundPosition(player));
                    }

                    nDASCounter[player]++;

                    if(nDASCounter[player] > 16)
                    {
                        nCurrentX[player]--;
                        mSounds.play(SoundEffect::Move, getSoundPosition(player));
                        nDASCounter[player] = 10;
                    }
                }
                else { // Piece does not fit, "DAS wall charge"
                    nDASCounter[player] = 16;
                }
            }

            if(bMovingDown[player])
            {
                if(doesPieceFit(player, nCurrentPiece[player], nCurrentRotation[player],
                                nCurrentX[player], nCurrentY[player] + 1))
                {
                    nSoftDropCounter[player]++;

                    if(nSoftDropCounter[player] > 1) // Soft Drop takes two frames on NES
                    {
                        nSoftDropPoints[player]++;
                        nSoftDropCounter[player] = 0;
                        bForceDown[player] = true; // Make sure the piece does not drop multiple rows in one frame
                    }
                }
                else
                    bForceDown[player] = true; // The piece does not fit, set bForceDown to true to lock it
            }

            if(bAI[player])
            {
                bRotatingCW[player] = false;
                bRotatingCCW[player] = false;
                bRotateHold[player] = true;
                bMovingLeft[player] = false;
                bMovingRight[player] = false;
            }


            if(bForceDown[player]) // Force the piece down the playfield if it is time for it
            {
                nGravityCounter[player] = 0; // Reset the gravity counter
                int lockPosition = 0;

                // Test if piece can be moved down
                if(doesPieceFit(player,
                                nCurrentPiece[player], nCurrentRotation[player], nCurrentX[player], nCurrentY[player] + 1))
                {
                    nCurrentY[player]++;  // Yup!
                }
                else
                {
                    // The piece cannot be moved down, lock into place
                    for(int px = 0; px < 4; px++)
                    {
                        for(int py = 0; py < 4; py++)
                        {
                            if(tetromino->tetromino[nCurrentPiece[player]][nCurrentRotation[player]][py * 4 + px] != L'.')
                            {
                                // Finished pieces have one value higher, to differentiate from empty space
                                // Remember to subtract one when drawing sprites in the playing field to show correct color.

                                pField[player][(nCurrentY[player] + py) * nFieldWidth +
                                               (nCurrentX[player] + px)] = nCurrentPiece[player] + 1;

                                lockPosition = nCurrentY[player] + py; // will take the highest value (bottom-most row)

                                mSounds.play(SoundEffect::LockPiece, getSoundPosition(player));
                            }
                        }
                    }

                    // Check for completed lines
                    for(int py = 0; py < 4; py++)
                    {
                        if(nCurrentY[player] + py < nFieldHeight -1)
                        {
                            bool bLine = true;

                            for(int px = 1; px < nFieldWidth - 1; px++)
                                bLine &= (pField[player][(nCurrentY[player] + py) * nFieldWidth + px]) != 0;

                            if(bLine)
                                vLines[player].push_back(nCurrentY[player] + py);
                        }
                    }

                    if(bMovingDown[player])
                    {
                        nScore[player] += nSoftDropPoints[player]; // Add one point per consecutive Soft Drop
                        nSoftDropPoints[player] = 0;
                    }

                    if(!vLines[player].empty())
                    {
                        mSounds.play(SoundEffect::LineCompletion, getSoundPosition(player)); // Sound for line completion

                        if(vLines[player].size() == 4)
                            mSounds.play(SoundEffect::Tetris, getSoundPosition(player)); // Sound for Tetris

                        if(nGameType == 1 || nGameType == 3) // In Game B and Game D, the line count decreases on line completion
                        {
                            nLines[player] -= vLines[player].size();
                            if(nLines[player] < 0) nLines[player] = 0;
                        }
                        else
                            nLines[player] += vLines[player].size(); // In the other modes, line count increases

                        nNextLevel[player] -= vLines[player].size();


                        if(nNextLevel[player] <= 0) // Reached a new level
                        {
                            nLevel[player]++;
                            nNextLevel[player] += 10;
                        }

                        // Add points for clearing lines
                        int bonus[4] = { 40, 100, 300, 1200 };
                        nScore[player] += bonus[vLines[player].size() - 1] * (nLevel[player] + 1);

                        nLineRemovalStep[player] = 6; // Effectively sets logic on hold while animation finishes
                    }

                    nEntryDelay[player] = 18; // Add ARE (Entry Delay) depending on where the last piece was placed on the board
                    if(lockPosition > 5) nEntryDelay[player] = 16;
                    else if(lockPosition > 9) nEntryDelay[player] = 14;
                    else if(lockPosition > 13) nEntryDelay[player] = 12;
                    else if(lockPosition > 17) nEntryDelay[player] = 10;

                    getNextPiece(player); // Pick a new piece

                    bMovingDown[player] = false; // Turn off Soft Drop for player
                    bAIassigned[player] = false; // Make AI ready for new orders

                    // If the piece does not fit straight away, Game Over!
                    bGameOver[player] = !doesPieceFit(player, nCurrentPiece[player], nCurrentRotation[player],
                                                      nCurrentX[player], nCurrentY[player]);

                    // Top-out ends the game in all modes
                    if(bGameOver[player] && !bWinner[player] && !bWinner[ (player + 1) % 2 ])
                        endMatch( (player + 1) % 2 );
                }
            }
        }
    }
    LastUpdate.restart(); // Ugly fix for pausing updates when the window is moved, instead of having separate threads etc.
    mSounds.removeStoppedSounds();

    return true;
}



void GameState::filterHighScore()
{
    // Criteria for being eligible for High Score:
    //      Single-player, No Map, Default Random Bag

    if(nNumberPlayers == 1 && nMap == -1 && sRandomBag == "0123456")
    {
        if(nGameType == 3) // Sprint Mode
        {
            if(nLines[0] == 0) // All 40 lines completed
                getContext().settings->currentScore = pauseTime + nFinalTime; // In Sprint Mode, the time is the score
            else
                getContext().settings->currentScore = 3600000; // One hour finish time if incomplete, bypass high score check
        }
        else // Marathon and Line Clear Modes
            getContext().settings->currentScore = nScore[0]; // In other modes, the points are the score

        getContext().settings->currentLevel = nLevel[0];

        requestStackPop();
        requestStackPush(States::HighScoreEntry); // Go to the High Score Entry state to check if this is valid
        return;
    }

    requestStackPop();
    requestStackPush(States::MainMenu); // The criteria was not met, go to the main menu state
}


bool GameState::handleEvent(const sf::Event& event)
{
    sf::RenderWindow& window = *getContext().window;

    bool exitToMenu = input->handleEvent(*this, event, window, getContext().settings->cSize);

    if(exitToMenu) // One of two things happened: 1) Actively quit from the menu, or 2) Pressed a button after game ended
    {
        if(bActiveQuit)
        {
            requestStackPop();
            requestStackPush(States::MainMenu);
            return true;
        }

        filterHighScore(); // Check if this is a valid high score
        return true;
    }

    if(bPauseMusic)
    {
        getContext().music->setPaused( bGamePaused );
        bPauseMusic = false;
    }

    if(bPauseSound)
    {
        mSounds.play(SoundEffect::Pause); // Play sound when pausing
        bPauseSound = false;
    }

    return true;
}
