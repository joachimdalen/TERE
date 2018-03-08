#ifndef AI_H
#define AI_H

#include "tetromino.h"
#include "grid.h"

class AI {

public:

    AI(int FieldWidth, int FieldHeight, int GameType, int AI);
    virtual ~AI();
    // Virtual destructor: Important!  If not freeing AI* pointers
    // Could leak mem if child classes allocate any

    int getCurrentX(int Piece);
    int getCurrentY(int Piece);

    bool doesPieceFit(int player, int nTetromino, unsigned char *Field, int nRotation, int nPosX, int nPosY);

    double ExecuteMove(int player, unsigned char *Field, int Piece, int phase, bool fail);

    std::string InitMove(int player, unsigned char *Field, int CurrentPiece,
                         int NextPiece, int CurrentX, int CurrentY, bool fail);


    unsigned char *pField[2];

    int nFieldWidth, nFieldHeight, nGameType, nAI;

    int nCurrentPiece[2], nNextPiece[2], nCurrentX[2], nCurrentY[2];
    int bestMove[2], bestRotation[2];

    // ===========================================
    // Original values, without separate weights for different line clears
/*
    double heightWeight = 0.510066;
    double linesWeight = 0.760666;
    double holesWeight = 0.35663;
    double bumpinessWeight = 0.184483;
 */


    // =======================================================
    // Different AI for Game A [0], Game B [1], and Game C [2]

    double heightWeight[3] = { 0.50421489508213, 0.2211829443504159, 0.503733225 };
    double linesWeight[3][5] = {
            {
                    0,
                    0.19224985695692436,
                    0.3884754157808101,
                    0.5733397187009789,
                    0.764349586589314004
            },
            {
                    0,
                    0.46785908534281023,
                    0.27727888156039876,
                    0.26841702434172104,
                    0.12378384637017341
            },
            {
                    0,
                    0.024308469,
                    0.096198506,
                    -0.025921265,
                    0.430041978
            },
    };
    double holesWeight[3] = { 0.35663, 0.7203740341614273, 0.557862913 };
    double bumpinessWeight[3] = { 0.20753935611383195, 0.22134478660816986, 0.274592131 };


    Tetromino* tetromino;
    Grid *grid[2][2];

};

#endif //AI_H
