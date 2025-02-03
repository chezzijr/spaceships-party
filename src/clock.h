#ifndef CLOCK_H
#define CLOCK_H

#ifdef _WIN32
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

class Clock {
private:
    uint64_t last;
    uint64_t now;
public:
    Clock();

    float delta();
};
#endif
