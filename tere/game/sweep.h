#ifndef SWEEP_H
#define SWEEP_H

class Sweep
{
public:

    Sweep(); // Base constructor
    Sweep(int nFieldWidth, int nFieldHeight); // Constructor with args

    virtual ~Sweep();
    // Virtual destructor: Important!  If not freeing Sweep* pointers
    // Could leak mem if child classes allocate any

    bool performSweep();

    void fadeIn();
    void fadeOut();

    int percentageFinished();
    void resetGrid(int value);
    bool Visible(int x, int y);

    int nFieldWidth, nFieldHeight, nFadeIn, nStep, nLow, nHigh, nDelta;
    int **nGrid;  // Pointer-to-pointer dynamic two-dimensional array
};

#endif //SWEEP_H
