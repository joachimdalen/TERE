#include <iostream>

#include "sweep.h"

Sweep::Sweep() {
}


Sweep::Sweep(int FieldWidth, int FieldHeight)
{
    nFieldWidth = FieldWidth;
    nFieldHeight = FieldHeight;

    nGrid = new int*[nFieldHeight - 2];

    for(int i = 0; i < nFieldHeight - 2; ++i)
        nGrid[i] = new int[nFieldWidth - 2];

    for(int y = 0; y < nFieldHeight - 2; y++)
        for(int x = 0; x < nFieldWidth - 2; x++)
            nGrid[y][x] = 0;
}


Sweep::~Sweep() {}


bool Sweep::Visible(int x, int y)
{
    if(nGrid[y - 1][x - 1] == 1) return true;
    return false;
}


void Sweep::resetGrid(int value)
{
    for(int y = 0; y < nFieldHeight - 2; y++)
        for(int x = 0; x < nFieldWidth - 2; x++)
            nGrid[y][x] = value;
}


void Sweep::fadeIn() { nFadeIn = 1; nStep = 205; nLow = 205; nHigh = -7; nDelta = abs(nHigh - nLow); }
void Sweep::fadeOut() { nFadeIn = 0; nStep = -120; nLow = -120; nHigh = 202; nDelta = abs(nHigh - nLow); }


int Sweep::percentageFinished()
{
    return abs( (nLow - nStep) * 10000 / nDelta );
}


bool Sweep::performSweep()
{
    int value = 0, step = nStep;
    resetGrid(1);

    int xmin = 0, xmax = nFieldWidth - 2;
    int ymin = 0, ymax = nFieldHeight - 2;

    while(step > 0)
    {
        for(int x = xmin; x < xmax; x++)
        {
            if(step == 0) break;
            nGrid[ymin][x] = value;
            step--;
        }

        xmax--;

        for(int y = ymin + 1; y < ymax; y++)
        {
            if(step == 0) break;
            nGrid[y][xmax] = value;
            step--;
        }

        ymax--;

        for(int x = xmax - 1; x >= xmin;  x--)
        {
            if(step == 0) break;
            nGrid[ymax][x] = value;
            step--;
        }

        xmin++;

        for(int y = ymax - 1; y > ymin; y--)
        {
            if(step == 0) break;
            nGrid[y][xmin - 1] = value;
            step--;
        }

        ymin++;
    }

    bool sweepInProgress = true;

    if(nFadeIn == 1)
    {
        nStep -= 4;
        if(nStep == -7) sweepInProgress = false;
    }
    else {
        nStep += 2;
        if(nStep == 202) sweepInProgress = false;
    }

    return sweepInProgress;
}
