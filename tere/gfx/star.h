#ifndef STAR_H
#define STAR_H

class Star
{
public:
    Star() {}
    Star(float, float);
    ~Star() {}
    float getXPos();
    float getYPos() const;
    void addYPos(float);
    void addXPos(float);

private:
    float xPos;
    float yPos;
};

#endif //STAR_H
