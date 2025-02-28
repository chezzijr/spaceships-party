#ifndef POWERUP_H
#define POWERUP_H

#include <SDL2/SDL.h>

#include "math.h"
#include "projectile.h"
#include <unordered_map>
#include <string>

class Powerup {
private:
    Vector2 pos;
    float radius;
    ProjectileType type;
    bool acquired;
public:
    Powerup(Vector2 pos, float radius, ProjectileType type);
    Circle getCollisionShape() const;
    ProjectileType getType() const;
    bool isAcquired() const;
    void acquire();
    void render(SDL_Renderer* renderer) const;
};

#endif
