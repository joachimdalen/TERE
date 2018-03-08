#ifndef CUBE_H
#define CUBE_H

#include <SFML/Graphics/RenderWindow.hpp>

class Cube
{
public:

    Cube();
    virtual ~Cube();

    void update();
    void draw(sf::RenderWindow &window, int);
    void gauss(double x, double y);

    double degree = 0.0;

    float p[4][4] = {
            { 1.f, 0.f, 0.f, 0.f },
            { 0.f, 1.f, 0.f, 0.f },
            { 0.f, 0.f, 0.f, -1.f/10.f },
            { 0.f, 0.f, 0.f, 1.f },
    };

    float vertices[8][4] = {
            { -1.f, 1.f, -1.f, 1.f},
            { 1.f, 1.f, -1.f, 1.f },
            { -1.f, -1.f, -1.f, 1.f },
            { 1.f, -1.f, -1.f, 1.f },
            { -1.f, 1.f, 1.f, 1.f },
            { 1.f, 1.f, 1.f, 1.f },
            { -1.f, -1.f, 1.f, 1.f },
            { 1.f, -1.f, 1.f, 1.f },
    };

    float vertices2[8][4];
    float coordinates[8][4];
    float c2D[8][3];
};

#endif //CUBE_H
