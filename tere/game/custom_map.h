#ifndef CUSTOM_MAP_H
#define CUSTOM_MAP_H

#include <fstream>

class CustomMap
{
public:

    CustomMap();
    virtual ~CustomMap();
    // Virtual destructor: Important!  If not freeing CustomMap* pointers
    // Could leak mem if child classes allocate any

    void loadFile(int type); // 0 - preset, 1 - custom
    void createSaveFile(bool clear);
    int getMapLocation(int map, int x, int y);

    int nCustomMap[800]; // For custom maps
    int nPresetMap[2400]; // For the preset maps

    std::ofstream saveToFile;
    std::ifstream loadFromFile;

};


#endif //CUSTOM_MAP_H
