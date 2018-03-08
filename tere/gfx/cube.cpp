#include <cmath>
#include <SFML/Graphics/CircleShape.hpp>
#include "cube.h"
#include "line.h"

Cube::Cube() {
}


Cube::~Cube() {}


void Cube::draw(sf::RenderWindow &window, int padding)
{
    sf::Color mColor = sf::Color(122, 210, 235, 64);  // PANTONE Blue 0821 C (Dark Blue - Ghost Piece)

    float w;
    float h;
    float sz = (window.getSize().y / 3 * 4) * 0.03f;
    float gm = (window.getSize().y - (window.getSize().y / 3 * 4 * 0.8f / 1.618f)) / 2; // Place the cubes so they create a golden rectangle

    for(int i = 0; i < 4; i++)
    {
        if(i < 2)
            w = padding + (window.getSize().y / 3 * 4) * 0.1f;
        else
            w = window.getSize().x - padding - (window.getSize().y / 3 * 4) * 0.1f;

        if(i == 0 || i == 2)
            h = gm;
        else
            h = window.getSize().y - gm;

        switch(i)
        {
            case 0 : gauss(1.0, 1.0); break;
            case 1 : gauss(-1.0, -1.0); break;
            case 2 : gauss(1.0, -1.0); break;
            case 3 : gauss(-1.0, 1.0); break;
        }


        sf::Vector2f lines[12][2] = {

                { {c2D[0][0] * sz + w, c2D[0][1] * sz + h}, { c2D[1][0] * sz + w, c2D[1][1] * sz + h } }, // A to B
                { {c2D[2][0] * sz + w, c2D[2][1] * sz + h}, { c2D[3][0] * sz + w, c2D[3][1] * sz + h } }, // C to D
                { {c2D[0][0] * sz + w, c2D[0][1] * sz + h}, { c2D[2][0] * sz + w, c2D[2][1] * sz + h } }, // A to C
                { {c2D[1][0] * sz + w, c2D[1][1] * sz + h}, { c2D[3][0] * sz + w, c2D[3][1] * sz + h } }, // B to D

                { {c2D[4][0] * sz + w, c2D[4][1] * sz + h}, { c2D[5][0] * sz + w, c2D[5][1] * sz + h } }, // E to F
                { {c2D[6][0] * sz + w, c2D[6][1] * sz + h}, { c2D[7][0] * sz + w, c2D[7][1] * sz + h } }, // G to H
                { {c2D[4][0] * sz + w, c2D[4][1] * sz + h}, { c2D[6][0] * sz + w, c2D[6][1] * sz + h } }, // E to G
                { {c2D[5][0] * sz + w, c2D[5][1] * sz + h}, { c2D[7][0] * sz + w, c2D[7][1] * sz + h } }, // F to H

                { {c2D[0][0] * sz + w, c2D[0][1] * sz + h}, { c2D[4][0] * sz + w, c2D[4][1] * sz + h } }, // A to E
                { {c2D[1][0] * sz + w, c2D[1][1] * sz + h}, { c2D[5][0] * sz + w, c2D[5][1] * sz + h } }, // B to F
                { {c2D[2][0] * sz + w, c2D[2][1] * sz + h}, { c2D[6][0] * sz + w, c2D[6][1] * sz + h } }, // C to G
                { {c2D[3][0] * sz + w, c2D[3][1] * sz + h}, { c2D[7][0] * sz + w, c2D[7][1] * sz + h } }, // D to H

        };

        float nThickness = 5 / 720.f * window.getSize().y;

        sw::Line thickLine;
        thickLine.setThickness(nThickness);
        thickLine.setColor(mColor);

        sf::CircleShape mCircle(nThickness / 2);
        mCircle.setFillColor(mColor);

        mCircle.setOrigin(nThickness / 2, nThickness / 2);

        for(int k = 0; k < 12; k++)
        {
            thickLine.setPoints( lines[k][0], lines[k][1] );
            window.draw(thickLine);
        }

        for(int k = 0; k < 8; k++)
        {
            mCircle.setPosition(c2D[k][0] * sz + w, c2D[k][1] * sz + h);
            window.draw(mCircle);
        }
    }
}

void Cube::update()
{
    degree += 0.01;

    if(degree > M_PI * 2)
        degree = 0.0;
}


void Cube::gauss(double x, double y)
{
    double degreeA, degreeB;

    degreeA = degree * x;
    degreeB = degree * y;

    // Make a copy of the original matrix
    for(int column = 0; column < 8; column++)
        for(int row = 0; row < 4; row++)
        {
            if(row < 3)
                vertices2[column][row] = 0;
            else
                vertices2[column][row] = vertices[column][row];
        }

    double xrotate[3][3] = {
            { 1, 0, 0 },
            { 0, cos(degreeA), -sin(degreeA) },
            { 0, sin(degreeA), cos(degreeA) },
    };

    double yrotate[3][3] = {
            { cos(degreeB), sin(degreeB), 0 },
            { -sin(degreeB), cos(degreeB), 0 },
            { 0, 0, 1 },
    };

    double zrotate[3][3] = {
            { cos(degree), 0, sin(degree) },
            { 0, 1, 0 },
            { -sin(degree), 0, cos(degree) },
    };

    double transform[3][3];


    for(int column = 0; column < 3; column++)
        for(int row = 0; row < 3; row++)
        {
            transform[column][row] = 0;

            for(int counter = 0; counter < 3; counter++)
                transform[column][row] += xrotate[counter][row] * yrotate[column][counter];
        }

    for(int column = 0; column < 8; column++)
        for(int row = 0; row < 3; row++)
            for(int counter = 0; counter < 3; counter++)
                vertices2[column][row] += transform[counter][row] * vertices[column][counter];

    for(int column = 0; column < 8; column++)
        for(int row = 0; row < 4; row++)
        {
            coordinates[column][row] = 0;

            for(int counter = 0; counter < 4; counter++)
                coordinates[column][row] += p[counter][row] * vertices2[column][counter];
        }

    for(int row = 0; row < 4; row++)
        for(int column = 0; column < 8; column++)
            if(row < 3)
                c2D[column][row] = coordinates[column][row] / coordinates[column][3];
}
