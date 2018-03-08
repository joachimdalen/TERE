#ifndef STARFIELD_A_H
#define STARFIELD_A_H

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class StarfieldA
{
public:
    StarfieldA();
    StarfieldA(int width, int height);
    virtual ~StarfieldA();

    void update();
    void draw(sf::RenderWindow &window);

    int nWidth, nHeight, nNumberStars = 256;

    float x[256], y[256], z[256], pz[256];
    float nSpeed = 2.0f;

    sf::Clock Timer;
};

#endif //STARFIELD_A_H
