#include <iostream>
#include <utility/utility.h>

#include "ai.h"

AI::AI(int FieldWidth, int FieldHeight, int GameType, int AI)
{
    nFieldWidth = FieldWidth;
    nFieldHeight = FieldHeight;
    nGameType = GameType;
    nAI = AI;

    //std::cout << "nAI: " << nAI << "\n";
    //std::cout << "GameType: " << GameType << "\n";

    tetromino = new Tetromino();

    for(int player = 0; player < 2; player++)
    {
        for(int phase = 0; phase < 2; phase++)
        {
            grid[player][phase] = new Grid(nFieldWidth, nFieldHeight);
        }
    }

    if(nGameType == 3) // Same AI for Game D (Sprint) as for Game A (Though in the final release the AI will never play this)
        nGameType = 0;
}


AI::~AI() {
}


bool AI::doesPieceFit(int player, int nTetromino, unsigned char *Field, int nRotation, int nPosX, int nPosY)
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
                    if(tetromino->tetromino[nTetromino][nRotation][py * 4 + px] != L'.' && Field[fi] != 0)
                        return false; // fail on first hit
                }
            }
        }
    }
    return true;
}


int AI::getCurrentX(int Piece)
{
    if(Piece < 2) return nFieldWidth / 2 - 2;  // Shift O- and I-pieces one column left
    else return nFieldWidth / 2 - 1;
}


int AI::getCurrentY(int Piece)
{
    if(Piece == 1) return -1; // Shift I-piece one row up
    else return 0;
}


double AI::ExecuteMove(int player, unsigned char *Field, int Piece, int phase, bool fail)
{
    double bestScore = 0.0;
    int nTestRotation = 0;

    grid[player][phase]->Insert(nFieldWidth, nFieldHeight, Field);

    for(int rotation = 0; rotation < 4; rotation++)
    {
        nTestRotation = rotation;

        int nLeft = getCurrentX(Piece);
        int nDown = getCurrentY(Piece);

        // Move as far left as possible
        for(int x = -1; x < nCurrentX[player]; x++)
        {
            if(doesPieceFit(player, Piece, Field, nTestRotation, x, nDown))
                if(x < nLeft) nLeft = x;
        }

        while( doesPieceFit(player, Piece, Field, nTestRotation, nLeft, nDown) )
        {
            // Move down as far as possible
            bool movingDown = true;
            int gy = nDown;

            while(movingDown)
            {
                movingDown = doesPieceFit(player, Piece, Field, nTestRotation, nLeft, gy);
                gy++;
            }

            gy -= 2;

            grid[player][phase]->resetCells();

            unsigned char tempField[nFieldWidth * nFieldHeight];

            for(int i = 0; i < nFieldWidth * nFieldHeight; i++)
                tempField[i] = Field[i];


            grid[player][phase]->addPiece(nLeft - 1, gy - 1,
                                          tetromino->tetromino[Piece][nTestRotation],
                                          nTestRotation);

            for(int py = 0; py < 4; py++)
                for(int px = 0; px < 4; px++)
                {
                    if(tetromino->tetromino[Piece][nTestRotation][py * 4 + px] != L'.')
                        tempField[(gy + py) * nFieldWidth + (nLeft + px) ] = 1;
                }

            double score;

            if(phase == 1)
            {
                int linesCompleted = grid[player][phase]->Lines();

                double holesWeightTemporary = holesWeight[nGameType];

                if(nAI < 2) // Make the AI dumb(er) for Easy and Medium difficulty
                {
                    if(fail) // 4% chance on Easy, 2% chance on Medium
                        holesWeightTemporary = -0.2;  // 0.05
                }

/*
                score = -heightWeight[nGameType] * grid[player][phase]->aggregateHeight() +
                        linesWeight[nGameType][linesCompleted] * linesCompleted -
                        holesWeight[nGameType] * grid[player][phase]->Holes() -
                        bumpinessWeight[nGameType] * grid[player][phase]->Bumpiness();
*/

                score = -heightWeight[nGameType] * grid[player][phase]->aggregateHeight() +
                        linesWeight[nGameType][linesCompleted] * linesCompleted -
                        holesWeightTemporary * grid[player][phase]->Holes() -
                        bumpinessWeight[nGameType] * grid[player][phase]->Bumpiness();



            }
            else {
                score = ExecuteMove(player, tempField, nNextPiece[player], 1, fail);
            }

            if(score > bestScore || bestScore == 0.0)
            {
                bestScore = score;

                if(phase == 0)
                {
                    bestMove[player] = nLeft;
                    bestRotation[player] = nTestRotation;
                }
            }
            nLeft++;
        }
    }
    return bestScore;
}


std::string AI::InitMove(int player, unsigned char *Field, int CurrentPiece,
                     int NextPiece, int CurrentX, int CurrentY, bool fail)
{
    std::string debug = "";

    pField[player] = Field;
    nCurrentPiece[player] = CurrentPiece;
    nNextPiece[player] = NextPiece;

    nCurrentX[player] = CurrentX;
    nCurrentY[player] = CurrentY;

    ExecuteMove(player, pField[player], nCurrentPiece[player], 0, fail);

    return "";
}
