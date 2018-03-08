#include "tetromino.h"

Tetromino::Tetromino() {

    // Some code examples have one of each tetromino, and use a rotation
    // algorithm - but doing that will not make the rotation accurate
    // according to the NES right-hand rotation, so I have hard-coded
    // every Tetromino (4x4) and their four rotations:

    tetromino[0][0].append(L".....XX..XX.....");    // O, 0
    tetromino[0][1].append(L".....XX..XX.....");    // O, 90
    tetromino[0][2].append(L".....XX..XX.....");    // O, 180
    tetromino[0][3].append(L".....XX..XX.....");    // O, 270

    tetromino[1][0].append(L"........XXXX....");    // I, 0
    tetromino[1][1].append(L"..X...X...X...X.");    // I, 90
    tetromino[1][2].append(L"........XXXX....");    // I, 180
    tetromino[1][3].append(L"..X...X...X...X.");    // I, 270

    tetromino[2][0].append(L"....XXX..X......");    // T, 0
    tetromino[2][1].append(L".X..XX...X......");    // T, 90
    tetromino[2][2].append(L".X..XXX.........");    // T, 180
    tetromino[2][3].append(L".X...XX..X......");    // T, 270

    tetromino[3][0].append(L"....XXX.X.......");    // L, 0
    tetromino[3][1].append(L"XX...X...X......");    // L, 90
    tetromino[3][2].append(L"..X.XXX.........");    // L, 180
    tetromino[3][3].append(L".X...X...XX.....");    // L, 270

    tetromino[4][0].append(L"....XXX...X.....");    // J, 0
    tetromino[4][1].append(L".X...X..XX......");    // J, 90
    tetromino[4][2].append(L"X...XXX.........");    // J, 180
    tetromino[4][3].append(L".XX..X...X......");    // J, 270

    tetromino[5][0].append(L".....XX.XX......");    // S, 0
    tetromino[5][1].append(L".X...XX...X.....");    // S, 90
    tetromino[5][2].append(L".....XX.XX......");    // S, 180
    tetromino[5][3].append(L".X...XX...X.....");    // S, 270

    tetromino[6][0].append(L"....XX...XX.....");    // Z, 0
    tetromino[6][1].append(L"..X..XX..X......");    // Z, 90
    tetromino[6][2].append(L"....XX...XX.....");    // Z, 180
    tetromino[6][3].append(L"..X..XX..X......");    // Z, 270
}


Tetromino::~Tetromino() {
}


std::wstring Tetromino::alphabet(char c)
{
    std::wstring custom;

    switch(c)
    {
        case 'A' : custom = L"XXXXX..XXXXXX..X"; break;
        case 'B' : custom = L"XX..X.X.XX.XX.X."; break;
        case 'C' : custom = L"XXXXX...X...XXXX"; break;
        case 'D' : custom = L"XXX.X..XX..XXXX."; break;
        case 'E' : custom = L"XXXXXXX.X...XXXX"; break;
        case 'F' : custom = L"XXXXX...XXX.X..."; break;
        case 'G' : custom = L"XXXXX...X..XXXXX"; break;
        case 'H' : custom = L"X..XX..XXXXXX..X"; break;
        case 'I' : custom = L"XXXX.X...X..XXXX"; break;
        case 'J' : custom = L"XXXX...XX..XXXXX"; break;
        case 'K' : custom = L"X.XXXX..X.X.X..X"; break;
        case 'L' : custom = L"X...X...X...XXXX"; break;
        case 'M' : custom = L"XXXXX.XXX..XX..X"; break;
        case 'N' : custom = L"XX.XX.XXX..XX..X"; break;
        case 'O' : custom = L"XXXXX..XX..XXXXX"; break;
        case 'P' : custom = L"XXXXX..XXXXXX..."; break;
        case 'Q' : custom = L"XXXXX..XX.XXXXXX"; break;
        case 'R' : custom = L"XXXXX..XXXXXX.X."; break;
        case 'S' : custom = L"XXXXX...XXXX.XXX"; break;
        case 'T' : custom = L"XXXX.X...X...X.."; break;
        case 'U' : custom = L"X..XX..XX..XXXXX"; break;
        case 'V' : custom = L"X..XX..XX..X.XX."; break;
        case 'W' : custom = L"X..XX..XX.XXXXXX"; break;
        case 'X' : custom = L"X..X.XX..XX.X..X"; break;
        case 'Y' : custom = L"X..XX..XXXXX.X.."; break;
        case 'Z' : custom = L"XXXX..X..X..XXXX"; break;

        case '0' : custom = L"XXXXX..XX..XXXXX"; break;
        case '1' : custom = L"XXXX..X...X.XXXX"; break;
        case '2' : custom = L"XXXX..X..X..XXXX"; break;
        case '3' : custom = L"XXXX.XXX...XXXXX"; break;
        case '4' : custom = L"X..XX..XXXXX...X"; break;
        case '5' : custom = L"XXXXX...XXXX.XXX"; break;
        case '6' : custom = L"X...XXXXX..XXXXX"; break;
        case '7' : custom = L"XXXX...X...X...X"; break;
        case '8' : custom = L"XXX.X.XXXX.X.XXX"; break;
        case '9' : custom = L"XXXXX..XXXXX...X"; break;

        case '!' : custom = L"..X...X.......X."; break;
        case '&' : custom = L"XX..X.X.X.XXXX.."; break;
        case '%' : custom = L"X..X..X..X..X..X"; break;
        case '^' : custom = L".X..X.X........."; break;
        case '*' : custom = L"X..X.XX..XX.X..X"; break;
        case '[' : custom = L".XX..X...X...XX."; break;
        case ']' : custom = L".XX...X...X..XX."; break;
        case '=' : custom = L"XXXX....XXXX...."; break;
        case '+' : custom = L".X..XXX..X......"; break;

        case 'a' : custom = L"........X...X..."; break;
        case 'b' : custom = L"....X...XX..XX.."; break;
        case 'c' : custom = L"XX..XXX.XXX.XX.."; break;
        case 'd' : custom = L".XX.XXXXXXXX.XX."; break;
        case 'e' : custom = L"..XX.XXX.XXX..XX"; break;
        case 'f' : custom = L".......X..XX..XX"; break;
        case 'g' : custom = L"...........X...X"; break;

        default : custom = L"................"; break;
    }

    return custom;
}


wchar_t Tetromino::getCharacterFromScrolltext(int player, int x, int y)
{
    return scrollText[player][y][x];
}


void Tetromino::constructScrollText(int player, std::string text)
{
    std::wstring temp;

    for(int c = 0; c < text.size(); c++)
    {
        temp = alphabet( text.at(c) );

        for(int y = 0; y < 4; y++)
        {
            for(int x = 0; x < 4; x++)
                scrollText[player][y][x + c * 5] = temp[y * 4 + x];

            scrollText[player][y][4 + c * 5] = L'.';
        }
    }
}



std::string Tetromino::constructRandomScrollText(int player)
{
    std::string s;

    switch( rand() % 4 )
    {
        case 0 : s = " ! "; break;
        case 1 : s = " * "; break;
        case 2 : s = " ^ "; break;
        case 3 : s = " + "; break;
    }

    constructScrollText(player, s);
    return s;
}
