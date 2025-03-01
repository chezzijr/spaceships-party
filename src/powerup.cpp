#include "powerup.h"

Powerup::Powerup(Vector2 pos, float radius, ProjectileType type)
    : pos(pos), radius(radius), type(type), acquired(false)
{}

Circle Powerup::getCollisionShape() const {
    return Circle(pos, radius);
}

ProjectileType Powerup::getType() const {
    return type;
}

bool Powerup::isAcquired() const {
    return acquired;
}

void Powerup::acquire() {
    acquired = true;
}

void Powerup::render(SDL_Renderer* renderer) const {
    auto gameSettings = GameSettings::get();
    if (type == ProjectileType::LASER_BEAM) {
        SDL_Rect dstRect = {(int)pos.x - radius, (int)pos.y - radius, (int)radius * 2, (int)radius * 2};
        SDL_RenderCopy(renderer, gameSettings->sdlSettings->laserPowerup, nullptr, &dstRect);
    } else if (type == ProjectileType::MINE) {
        SDL_Rect dstRect = {(int)pos.x - radius, (int)pos.y - radius, (int)radius * 2, (int)radius * 2};
        SDL_RenderCopy(renderer, gameSettings->sdlSettings->minePowerup, nullptr, &dstRect);
    } else if (type == ProjectileType::PLUS) {
        SDL_Rect dstRect = {(int)pos.x - radius, (int)pos.y - radius, (int)radius * 2, (int)radius * 2};
        SDL_RenderCopy(renderer, gameSettings->sdlSettings->plusPowerup, nullptr, &dstRect);
    }
}
