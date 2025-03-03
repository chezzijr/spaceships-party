#ifndef EXTERNAL_FORCE_H
#define EXTERNAL_FORCE_H

#include "math.h"
#include "spaceship.h"
#include "projectile.h"
#include "settings.h"
#include <memory>
#include <vector>
#include "utils.h"

class Force {
private:
    ForceType type;
    float strength;
    float radius;
    Vector2 position;
public:
    Force(ForceType type, float strength, float radius, Vector2 position);
    void apply(float delta, const std::vector<std::shared_ptr<Spaceship>>& spaceships, const std::vector<std::shared_ptr<Projectile>>& projectiles);
    void render(SDL_Renderer* renderer) const;
};

#endif