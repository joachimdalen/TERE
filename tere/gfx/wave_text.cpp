#include <math.h>
#include "wave_text.h"

WaveText::WaveText(sf::RenderWindow &window, std::string header)
{
    dx = window.getSize().x;
    dy = window.getSize().y;

    sHeader = header;

    // Four pixels per character, plus one pixel separating each character and eight pixels on both sides
    nPixels = sHeader.length() * 4 + sHeader.length() - 1;
    tile = new Tile(window, nPixels + 16, true);

    // Reusing this code from the main game to make a logo
    tetromino = new Tetromino();
    tetromino->constructScrollText(0, sHeader);
}


WaveText::~WaveText() {}


void WaveText::draw(sf::RenderWindow &window, int yShift)
{
    double aspect = dx * 1.0 / dy * 1.0;
    int padding = 0;

    if( aspect > 1.33334)
    padding = (dx - (dy * 4 / 3)) / 2;

    int centerPadding = ((dy * 4 / 3) - tile->tileSize * nPixels) / 2;

    double mSin;

    for(int y = 0; y < 4; y++)
    {
        for(int x = 0; x < nPixels; x++)
        {
            mSin = sin( 2 * M_PI / nPixels * x + nXShift ) * tile->tileSize;

            tile->mTile.setFillColor( tile->mColors[7]);

            if(tetromino->getCharacterFromScrolltext(0, x, y) == L'X')
                tile->mTile.setFillColor( tile->mColors[9] );

            tile->mTile.setPosition(padding + centerPadding + x * tile->tileSize, (y + 1) * tile->tileSize + mSin + yShift);
            window.draw(tile->mTile);
        }
    }
}


void WaveText::update()
{
    double adjustment = 0.05 / (1000.0 / 60) * (Timer.getElapsedTime().asMicroseconds() / 1000.0);

    nXShift = nXShift + adjustment;
    //nXShift = nXShift + 0.05;

    if(nXShift > 2 * M_PI)
        nXShift = 0.0;

    Timer.restart();
}
