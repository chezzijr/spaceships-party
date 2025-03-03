#include "clock.h"

Clock::Clock() : last(0), now(SDL_GetPerformanceCounter()) {}

float Clock::delta()
{
    last = now;
    now = SDL_GetPerformanceCounter();
    float d = float(now - last) / static_cast<double>(SDL_GetPerformanceFrequency());
    return d;
}

void Clock::reset()
{
    last = 0;
    now = SDL_GetPerformanceCounter();
}