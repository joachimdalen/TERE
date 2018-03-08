#include <cmath>
#include <iostream>
#include "tile.h"

float Tile::circle(float radius, float x)
{
    return radius - sqrt( radius * radius - x * x );
}


Tile::Tile() {
}

Tile::~Tile() {
}



Tile::Tile(sf::RenderWindow &window, int nFieldSize, bool horizontal)
{
    int dx = window.getSize().x;
    int dy = window.getSize().y;

    int pixels = dy;

    if(horizontal)
    {
        double aspect = dx * 1.0 / dy * 1.0;

        if( aspect > 1.33334)
            dx = dy * 4 / 3;

        pixels = dx; //window.getSize().x;
    }

    size_t cornerVertices = 25;
    mTile.setPointCount(cornerVertices * 4 + 4);

    // can use dx instead for the logo, horizontal

    tileSize = (int)round( (double)pixels / (double)(nFieldSize) );

    float radius = tileSize / 5;
    float width = tileSize * 3 / 5;
    float padding = tileSize * 1 / 25;  // 4% padding

    if(padding < 1.0) padding = 1.0;

    float x, y;

    for (size_t i = 0; i <= cornerVertices; i++) {
        x = radius / cornerVertices * i;
        y = circle(radius, radius - radius / cornerVertices * i); // works for top left corner

        mTile.setPoint(i, sf::Vector2f(x + padding, y + padding));
        mTile.setPoint(cornerVertices * 4 + 3 - i, sf::Vector2f(x + padding, radius * 2 + width - y - padding));

        x = radius / cornerVertices * i;
        y = circle(radius, radius / cornerVertices * i); // works for top right corner

        mTile.setPoint(i + cornerVertices + 1, sf::Vector2f(radius + width + x - padding, y + padding));
        mTile.setPoint(cornerVertices * 3 + 2 - i, sf::Vector2f(radius + width + x - padding, radius * 2 + width - y - padding));
    }

    mColors[0] = sf::Color(241, 239, 180);      // PANTONE Yellow 0131 C (Yellow)
    mColors[1] = sf::Color(154, 218, 236);      // PANTONE 9464 C (Lt. Blue)
    mColors[2] = sf::Color(201, 168, 226);      // PANTONE Violet 0631 C  (Purple)
    mColors[3] = sf::Color(242, 189, 225);      // PANTONE Magenta 0521 C (Orange)
    mColors[4] = sf::Color(122, 210, 235);      // PANTONE Blue 0821 C (Dark Blue)
    mColors[5] = sf::Color(159, 230, 218);      // PANTONE Green 0921 C (Green)
    mColors[6] = sf::Color(253, 186, 198);      // PANTONE Red 0331 C (Red)

    mColors[7] = sf::Color(180, 173, 164, 31);  // PANTONE Black 0961 C (Lt. Gray)
    mColors[8] = sf::Color(122, 210, 235, 64);  // PANTONE Blue 0821 C (Dark Blue - Ghost Piece)
    mColors[9] = sf::Color(255, 255, 255);      // White (for the flash effect on Tetris line clear)
}
