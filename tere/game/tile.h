#ifndef TILE_H
#define TILE_H

#include <SFML/Graphics.hpp>

class Tile {

public:

    Tile(); // Base constructor
    Tile(sf::RenderWindow &window, int nFieldSize, bool horizontal); // Constructor with args

    virtual ~Tile();
    // Virtual destructor: Important!  If not freeing Tile* pointers
    // Could leak mem if child classes allocate any

    virtual float circle(float radius, float x);
    // Virtual: ensures that the right version is called on Tile* pointers
    // and Tile& references

    sf::ConvexShape         mTile;
    sf::Color mColors[10];
    int tileSize;
    bool bHorizontal;
};

#endif //TILE_H
