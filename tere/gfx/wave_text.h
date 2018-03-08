#ifndef WAVE_TEXT_H
#define WAVE_TEXT_H

#include <game/tile.h>
#include <game/tetromino.h>

class WaveText
{

public:

    WaveText(sf::RenderWindow &window, std::string header);
    ~WaveText();

    void draw(sf::RenderWindow &window, int yShift);
    void update();

    double nXShift = 0.0;
    int dx, dy, nPixels;
    std::string sHeader;

    sf::Clock Timer;

    Tile* tile;
    Tetromino* tetromino;
};

#endif //WAVE_TEXT_H
