#include "utils.h"

void drawCircle(SDL_Renderer* renderer, const Circle& circle) {
    int radius = (int)circle.radius;
    int centerX = (int)circle.center.x;
    int centerY = (int)circle.center.y;
    for (int y = -radius; y <= radius; y++) {
        int dx = (int)sqrt(radius * radius - y * y); // Calculate horizontal distance
        SDL_RenderDrawLine(renderer, centerX - dx, centerY + y, centerX + dx, centerY + y);
    }
}

void drawCircleRing(SDL_Renderer* renderer, const Circle& circle, int width) {
    int radius = (int)circle.radius;
    int centerX = (int)circle.center.x;
    int centerY = (int)circle.center.y;
    for (int y = -radius; y <= radius; y++) {
        int dx = (int)sqrt(radius * radius - y * y); // Calculate horizontal distance
        for (int i = -width / 2; i <= width / 2; i++) {
            SDL_RenderDrawPoint(renderer, centerX - dx + i, centerY + y);
        }
    }
}

// 1: left, 2: right, 3: top, 4: bottom
RayIntersection getRayIntersectionBorder(const Vector2& pos, float angle, int SCREEN_WIDTH, int SCREEN_HEIGHT) {
    float dx = std::cos(angle);
    float dy = std::sin(angle);
    
    // Initialize t-values to a large number
    float tLeft = FLT_MAX, tRight = FLT_MAX, tTop = FLT_MAX, tBottom = FLT_MAX;
    
    // Compute t for left/right borders if possible
    if (dx < 0)
        tLeft = (0 - pos.x) / dx;
    if (dx > 0)
        tRight = (SCREEN_WIDTH - pos.x) / dx;
    
    // Compute t for top/bottom borders if possible
    if (dy < 0)
        tTop = (0 - pos.y) / dy;
    if (dy > 0)
        tBottom = (SCREEN_HEIGHT - pos.y) / dy;
    
    // Determine the smallest positive t and corresponding side
    float tMin = FLT_MAX;
    BorderSide side = BorderSide::LEFT;
    if (tLeft >= 0 && tLeft < tMin) { tMin = tLeft; side = BorderSide::LEFT; }
    if (tRight >= 0 && tRight < tMin) { tMin = tRight; side = BorderSide::RIGHT; }
    if (tTop >= 0 && tTop < tMin) { tMin = tTop; side = BorderSide::TOP; }
    if (tBottom >= 0 && tBottom < tMin) { tMin = tBottom; side = BorderSide::BOTTOM; }
    
    // Calculate the intersection point using pos + tMin * (dx, dy)
    Vector2 intersectionPoint = { pos.x + tMin * dx, pos.y + tMin * dy };

    return { side, intersectionPoint };
}