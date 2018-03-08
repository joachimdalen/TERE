#ifndef TETROMINO_H
#define TETROMINO_H

#include <iostream>

class Tetromino {

public:
    Tetromino(); // Base constructor
    virtual ~Tetromino();
    // Virtual destructor: Important!  If not freeing Tetromino* pointers
    // Could leak mem if child classes allocate any

    std::wstring alphabet(char c);
    void constructScrollText(int player, std::string text);

    std::string constructRandomScrollText(int player);


    wchar_t getCharacterFromScrolltext(int player, int x, int y);

    std::wstring tetromino[7][4];
    wchar_t scrollText[2][4][100];
};

#endif //TETROMINO_H
