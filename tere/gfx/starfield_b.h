#ifndef STARFIELD_B_H
#define STARFIELD_B_H

#include <SFML/Graphics.hpp>
#include <random>
#include "star.h"

class StarfieldB
{
public:
    StarfieldB();
    StarfieldB(int, int, int);
    virtual ~StarfieldB() {}

    void update();
    void draw(sf::RenderWindow &window);


protected:

    int maxStars[7] = { 128, 64, 32, 16, 8, 4, 2 };

    sf::Uint16 x_Size;
    sf::Uint16 y_Size;
    int nType;

    std::vector<Star> Stars[7];

    std::default_random_engine re_x;
    std::default_random_engine re_y;
    std::uniform_int_distribution<int> my_distribution_x;
    std::uniform_int_distribution<int> my_distribution_y;

    sf::Clock Timer;
};

#endif //STARFIELD_B_H
