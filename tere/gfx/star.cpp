#include "star.h"

Star::Star(float p_X_Pos, float p_Y_Pos)
{
    xPos = p_X_Pos;
    yPos = p_Y_Pos;
}

float Star::getXPos() { return xPos; }
float Star::getYPos() const { return yPos; }
void Star::addYPos(float y) { yPos += y; }
void Star::addXPos(float x) { xPos += x; }
