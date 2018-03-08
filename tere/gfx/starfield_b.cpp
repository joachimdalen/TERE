#include <iostream>
#include "starfield_b.h"

StarfieldB::StarfieldB() {}


StarfieldB::StarfieldB(int xResolution, int yResolution, int type)
{
    nType = type; // Which direction will the stars move?

    x_Size = xResolution;
    y_Size = yResolution;

    // Initialize the random generator
    my_distribution_x = std::uniform_int_distribution<int>(0, xResolution);
    my_distribution_y = std::uniform_int_distribution<int>(0, yResolution);

    re_x.seed(std::time(0));
    re_y.seed(std::time(0) + 24);

    // Generate a start set of stars
    for(int i = 0; i < 7; i++)
    {
        while((int)Stars[i].size() <= maxStars[i])
            Stars[i].push_back(Star(my_distribution_x(re_x), my_distribution_y(re_y)));
    }
}


void StarfieldB::update()
{
    // Use a factor that compares how long has passed since the last update
    // to 1/60 second (60 fps) and adjusts accordingly

    float factor = 1.f / (1000.0 / 60) * (Timer.getElapsedTime().asMicroseconds() / 1000.0);

    if(Timer.getElapsedTime().asMilliseconds() > 125) // To make it work with pausing the game
        factor = 1.f;


    // Remove all stars that have exceeded the lower screen border
    for(int i = 0; i < 7; i++)
    {
        switch(nType)
        {
            case 0 : // Stars are moving down
                Stars[i].erase(remove_if(Stars[i].begin(), Stars[i].end(), [&](Star& p_Star) {
                    return (p_Star.getYPos() > y_Size); }), Stars[i].end());
                break;

            case 1 : // Stars are moving up
                Stars[i].erase(remove_if(Stars[i].begin(), Stars[i].end(), [&](Star& p_Star) {
                    return (p_Star.getYPos() < 10.f); }), Stars[i].end());
                break;

            case 2 : // Stars are moving right
                Stars[i].erase(remove_if(Stars[i].begin(), Stars[i].end(), [&](Star& p_Star) {
                    return (p_Star.getXPos() > x_Size); }), Stars[i].end());
                break;

            case 3 : // Stars are moving left
                Stars[i].erase(remove_if(Stars[i].begin(), Stars[i].end(), [&](Star& p_Star) {
                    return (p_Star.getXPos() <= 0); }), Stars[i].end());
                break;
        }
    }

    factor = factor / 720.f * y_Size; // Make the speed appear the same on all resolutions

    // Move every star, according to its size to create a parallax effect
    for(int i = 0; i < 7; i++)
    {
        switch(nType)
        {
            case 0 : // Moving down
                for_each(Stars[i].begin(), Stars[i].end(), [&](Star& p_Star) {
                    p_Star.addYPos(0.4 * factor * (i + 1)); });
                break;

            case 1 : // Moving up
                for_each(Stars[i].begin(), Stars[i].end(), [&](Star& p_Star) {
                    p_Star.addYPos(-0.4 * factor * (i + 1)); });
                break;

            case 2 : // Moving right
                for_each(Stars[i].begin(), Stars[i].end(), [&](Star& p_Star) {
                    p_Star.addXPos(0.4 * factor * (i + 1)); });
                break;

            case 3 : // Moving left
                for_each(Stars[i].begin(), Stars[i].end(), [&](Star& p_Star) {
                    p_Star.addXPos(-0.4 * factor * (i + 1)); });
                break;
        }
    }

    // Create new stars until the set limit is reached
    for(int i = 0; i < 7; i++)
    {
        switch(nType)
        {
            case 0: // Moving down, replace at top
                while((int)Stars[i].size() <= maxStars[i]) Stars[i].emplace_back(Star(my_distribution_x(re_x), 0));
                break;

            case 1: // Moving up, replace at bottom
                while((int)Stars[i].size() <= maxStars[i]) Stars[i].emplace_back(Star(my_distribution_x(re_x), y_Size));
                break;

            case 2: // Moving right, replace at left
                while((int)Stars[i].size() <= maxStars[i]) Stars[i].emplace_back(Star(0, my_distribution_y(re_y)));
                break;

            case 3: // Moving left, replace at right
                while((int)Stars[i].size() <= maxStars[i]) Stars[i].emplace_back(Star(x_Size, my_distribution_y(re_y)));
                break;
        }
    }

    Timer.restart();
}


void StarfieldB::draw(sf::RenderWindow &window)
{
    float factor = window.getSize().y / 1080.f; // Keep the stars from getting too large on smaller resolutions

    for(int i = 0; i < 7; i++)
    {
        for(std::vector<Star>::iterator it = Stars[i].begin(); it != Stars[i].end(); ++it)
        {
            if(it->getYPos() < window.getSize().y)
            {
                sf::CircleShape mCircle(0.3f * (i + 1) * factor);
                mCircle.setFillColor(sf::Color(64 + 27 * (i + 1), 64 + 27 * (i + 1), 64 + 27 * (i + 1)));
                mCircle.setPosition(it->getXPos(), it->getYPos());
                window.draw(mCircle);
            }
        }
    }
}
