#ifndef CLOCK_H
#define CLOCK_H

#include <SDL2/SDL.h>

class Clock {
private:
    uint64_t last;
    uint64_t now;
public:
    Clock();
    void reset();
    float delta();
};
#endif
