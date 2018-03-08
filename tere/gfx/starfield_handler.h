#ifndef STARFIELD_HANDLER_H
#define STARFIELD_HANDLER_H

#include "starfield_a.h"
#include "starfield_b.h"

class StarfieldHandler {

public:
    StarfieldHandler(int, int);
    virtual ~StarfieldHandler() {}

    void update();
    void draw(sf::RenderWindow &window, bool);

    int nType;

    StarfieldA* starfield_a;
    StarfieldB* starfield_b;
};

#endif //STARFIELD_HANDLER_H
