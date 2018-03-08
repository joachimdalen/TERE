#include <iostream>
#include "starfield_a.h"

StarfieldA::StarfieldA() {}
StarfieldA::~StarfieldA() {}


StarfieldA::StarfieldA(int width, int height)
{
    nWidth = width;
    nHeight = height;

    nSpeed = nSpeed * (height / 720.f); // Make the speed appear the same on all resolutions

    for(int i = 0; i < nNumberStars; i++)
    {
        x[i] = rand() % nWidth - nWidth / 2;
        y[i] = rand() % nHeight - nHeight / 2;

        z[i] = rand() % nWidth / 2;

        pz[i] = z[i];
    }
}


void StarfieldA::update()
{
    // Use a factor that compares how long has passed since the last update
    // to 1/60 second (60 fps) and adjusts accordingly

    float adjustment = nSpeed / (1000.0 / 60) * (Timer.getElapsedTime().asMicroseconds() / 1000.0);

    if(Timer.getElapsedTime().asMilliseconds() > 125) // To make it work with pausing the game
        adjustment = nSpeed;


    for(int i = 0; i < nNumberStars; i++)
    {
        //z[i] = z[i] - nSpeed;
        z[i] = z[i] - adjustment;

        if (z[i] < 1)
        {
            z[i] = nWidth / 2;
            x[i] = rand() % nWidth - nWidth / 2;
            y[i] = rand() % nHeight - nHeight / 2;
            pz[i] = z[i];
        }
    }

    Timer.restart();
}


float map(float x, float in_min, float in_max, float out_min, float out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


void StarfieldA::draw(sf::RenderWindow &window)
{
    float sx, sy, r;

    for(int i = 0; i < nNumberStars; i++)
    {
        sx = map(x[i] / z[i], 0, 1, 0, nWidth / 2);
        sy = map(y[i] / z[i], 0, 1, 0, nHeight / 2);
        r = map(z[i], 0, nWidth / 2, 16, 0);

        float factor = window.getSize().y / 1080.f; // Keep the stars from getting too large on lower resolutions

        sf::CircleShape mCircle((r * factor) / 5);
        //sf::CircleShape mCircle(r / 5);

        mCircle.setFillColor(sf::Color(63, 63, 63));

        if (r > 10)
            mCircle.setFillColor(sf::Color(255, 255, 255));
        else if (r > 7.5)
            mCircle.setFillColor(sf::Color(191, 191, 191));
        else if (r > 5.0)
            mCircle.setFillColor(sf::Color(127, 127, 127));

        mCircle.setPosition(sx - r / 2 + nWidth / 2, sy - r / 2 + nHeight / 2);
        window.draw(mCircle);
        pz[i] = z[i];
    }
}
