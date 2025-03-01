#ifndef UTILS_H
#define UTILS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "math.h"

void drawCircle(SDL_Renderer* renderer, const Circle& circle);
void drawCircleRing(SDL_Renderer* renderer, const Circle& circle, int width);
SDL_Texture* renderTextAsTexture(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color color);

enum class BorderSide {
    LEFT = 1,
    RIGHT = 2,
    TOP = 3,
    BOTTOM = 4
};
struct RayIntersection {
    BorderSide side;
    Vector2 intersectionPoint;
};
RayIntersection getRayIntersectionBorder(const Vector2& pos, float angle, int SCREEN_WIDTH, int SCREEN_HEIGHT);

#endif

