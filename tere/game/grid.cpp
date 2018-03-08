#include <iostream>
#include <fstream>

#include "grid.h"

Grid::Grid(int FieldWidth, int FieldHeight)
{
    cField = new unsigned char[FieldWidth * FieldHeight]; // keep a scratch playing field
}


Grid::~Grid() {
}


void Grid::Insert(int FieldWidth, int FieldHeight, unsigned char *Field)
{
    nFieldHeight = FieldHeight;
    nFieldWidth = FieldWidth;
    pField = Field; // The original playing field

    nColumns = nFieldWidth - 2;
    nRows = nFieldHeight - 2;
}


int Grid::resetCells()
{
    for(int i = 0; i < nFieldHeight * nFieldWidth; i++)
        cField[i] = pField[i];
}


int Grid::getCell(int x, int y)
{
    return (int)cField[ nFieldWidth * (y + 1) + x + 1 ];
}


void Grid::setCell(int x, int y)
{
    cField[ (y + 1) * nFieldWidth + x + 1 ] = 1;
}


int Grid::aggregateHeight()
{
    int total = 0;

    for(int x = 0; x < nColumns ; x++)
        total += columnHeight(x);

    return total;
}


int Grid::columnHeight(int x)
{
    int y = 0;

    for(; y < nRows && getCell(x, y) == 0; y++);

    return nRows - y;

}


void Grid::addPiece(int piece_column, int piece_row, std::wstring tetromino, int rotation)
{
    for(int py = 0; py < 4; py++)
    {
        for(int px = 0; px < 4; px++)
        {
            int _row = piece_row + py;
            int _column = piece_column + px;

            if(tetromino[py * 4 + px] != L'.' && _row >= 0)
            {
                setCell(_column, _row);
            }
        }
    }
}


bool Grid::isLine(int y)
{
    for(int x = 0; x < nColumns; x++)
        if(getCell(x, y) == 0)
            return false;

    return true;
}


int Grid::Lines()
{
    int amount = 0;

    for(int y = 0; y < nRows; y++)
    {
        if(isLine(y))
            amount++;
    }

    return amount;
}


int Grid::Holes()
{
    int amount = 0;

    for(int x = 0; x < nColumns; x++)
    {
        bool block = false;

        for(int y = 0; y < nRows; y++)
        {
            if(getCell(x, y) != 0)
                block = true;
            else if(getCell(x, y) == 0 && block)
                amount++;
        }
    }
    return amount;
}


int Grid::Bumpiness()
{
    int amount = 0;

    for(int x = 0; x < nColumns - 1; x++)
    {
        amount += abs( columnHeight(x) - columnHeight(x + 1));
    }

    return amount;
}
