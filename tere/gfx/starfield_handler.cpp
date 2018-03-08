#include <SFML/Graphics/RenderWindow.hpp>
#include "starfield_handler.h"

StarfieldHandler::StarfieldHandler(int xResolution, int yResolution)
{
    srand(time(NULL));


    nType = rand() % 6;


    //nType = 0;

    if(nType < 2)
        starfield_a = new StarfieldA(xResolution, yResolution);
    else
        starfield_b = new StarfieldB(xResolution, yResolution, nType - 2);
}


void StarfieldHandler::update()
{
    // Starfield is still updated even if drawing has been disabled

    if(nType < 2)
        starfield_a->update();
    else
        starfield_b->update();
}


void StarfieldHandler::draw(sf::RenderWindow &window, bool enabled)
{
    if(enabled)
    {
        if(nType < 2)
            starfield_a->draw(window);
        else
            starfield_b->draw(window);
    }
}
