#ifndef GRID_H
#define GRID_H

// Used by the AI to analyze a playing field

class Grid {

public:

    Grid(int FieldWidth, int FieldHeight);
    virtual ~Grid();

    void Insert(int FieldWidth, int FieldHeight, unsigned char *Field);
    int resetCells();
    int getCell(int x, int y);
    void setCell(int x, int y);
    int aggregateHeight();
    int columnHeight(int x);

    void addPiece(int piece_column, int piece_row, std::wstring tetromino, int rotation);

    bool isLine(int y);
    int Lines();
    int Holes();
    int Bumpiness();

    unsigned char *pField, *cField, *tField; //, *tField;
    int nFieldWidth, nFieldHeight;
    int nColumns, nRows;
};

#endif //GRID_H
