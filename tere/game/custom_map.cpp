#include <iostream>
#include "custom_map.h"

CustomMap::CustomMap()
{
    loadFile(0); // Load preset maps
    loadFile(1); // Load custom maps
}

CustomMap::~CustomMap() {}


int CustomMap::getMapLocation(int map, int x, int y)
{
    int cellValue;

    if(x < 1 || x > 10 || y > 20) return 8;
    if(y < 1) return 0;

    y = y - 1;
    x = x - 1;

    if(map < 12)
        cellValue = nPresetMap[ map * 200 + y * 10 + x ] + 1;
    else
    {
        map -= 12;
        cellValue = nCustomMap[ map * 200 + y * 10 + x ] + 1;
    }

    if(cellValue == 8)
        cellValue = 0;

    if(cellValue < 0 || cellValue > 7)
        std::cout << "cellValue: " << cellValue << "\n";

    return cellValue;
}


void CustomMap::createSaveFile(bool clear) // Create a new save file, or update the current one
{
    char c;

    saveToFile.open("save/custom.map");

    for(int i = 0; i < 800; i++)
    {
        c = '.';

        if(!clear)
        {
            if(nCustomMap[i] > -1 && nCustomMap[i] < 7)
                c = nCustomMap[i] + 48;
        }

        saveToFile << c;

        if((i+1) % 10 == 0) saveToFile << std::endl;
        if((i+1) % 200 == 0) saveToFile << std::endl;

    }
    saveToFile.close();
}


void CustomMap::loadFile(int type)
{
    int cells;

    if(type == 0)
    {
        loadFromFile.open("save/preset.map");
        cells = 2400;
    }
    else
    {
        loadFromFile.open("save/custom.map");
        cells = 800;
    }

    for(int i = 0; i < cells; i++)
    {
        char c;
        int value;

        loadFromFile >> c;

        if(c == '\n') loadFromFile >> c;

        value = (int)c - 48;

        if(value == -2) value = 7;

        if(value < 0 || value > 7)
        {
            // Something is wrong with the file, make a new one
            loadFromFile.close();

            createSaveFile(true); // Clear the save file
            return;
        }

        if(type == 0)
            nPresetMap[i] = value;
        else
            nCustomMap[i] = value;
    }

    loadFromFile.close();
}
