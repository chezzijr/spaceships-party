#include "spaceship.h"
#include <iostream>
#include "utils.h"

int Spaceship::idCounter = 0;

Spaceship::Spaceship(int playerNumber, float startX, float startY)
    : id(idCounter++), 
    playerNumber(playerNumber), 
    value(1), 
    gameSettings(GameSettings::get()), 
    pos(Vector2(startX, startY)), 
    velocity(Vector2(1.0, 0.0)), 
    speed(0.0), 
    angle(0.0), 
    active(false),
    readyForSameSideCollision(true),
    readyForOppositeSideCollision(true),
    // weapon(Weapon(WeaponSettings{
    //     .type = ProjectileType::BULLET,
    //     .cooldown = 1.0f,
    //     .cooldownTimer = 0.0f,
    //     .maxBulletAmmo = 2,
    //     .bulletAmmo = 2,
    //     .bulletSpeed = settings->bulletSpeed,
    //     .bulletRadius = settings->bulletRadius,
    //     .bulletLifeTime = settings->bulletLifeTime,
    //     .laserBeamLifeTime = settings->laserBeamLifeTime,
    //     .laserBeamWidth = settings->laserBeamWidth,
    //     .mineActivationDuration = settings->mineActivationDuration,
    //     .mineActiveRadius = settings->mineActiveRadius,
    //     .mineExplosionRadius = settings->mineExplosionRadius,
    //     .mineExplosionDuration = settings->mineExplosionDuration,
    //     .mineSize = settings->mineSize
    // }))
    weapon(Weapon(ProjectileType::BULLET, 1.0f, 0.0f, 2, 2))
{
}

float Spaceship::minX() const {
    return pos.x - gameSettings->spaceshipSize / 2;
}

float Spaceship::minY() const {
    return pos.y - gameSettings->spaceshipSize / 2;
}

float Spaceship::maxX() const {
    return pos.x + gameSettings->spaceshipSize / 2;
}

float Spaceship::maxY() const {
    return pos.y + gameSettings->spaceshipSize / 2;
}


Circle Spaceship::getCollisionShape() const {
    return Circle(pos, gameSettings->spaceshipSize / 2);
}

void Spaceship::toggleActive() {
    active = !active;
}

void Spaceship::rotate(float degrees) {
    angle += degrees;
    if (angle >= 360.0f) {
        angle -= 360.0f;
    } else if (angle < 0.0f) {
        angle += 360.0f;
    }
    // update the velocity vector to match the new angle
    float rad = deg2rad(angle);
    float newVelX = std::cos(rad) * velocity.magnitude();
    float newVelY = std::sin(rad) * velocity.magnitude();
    velocity = Vector2(newVelX, newVelY).normalize();
}

void Spaceship::applyForce(float magnitude) {
    float rad = deg2rad(angle);
    speed = magnitude;
    velocity.x += std::cos(rad) * magnitude;
    velocity.y += std::sin(rad) * magnitude;
    velocity = velocity.normalize();
}

void Spaceship::update(float deltaTime) {
    // Apply boost force temporarily
    // if (applyingBoost && SDL_GetTicks() > boostEndTime) {
    //     applyingBoost = false;
    // }

    // Update position using velocity
    pos += velocity * speed * deltaTime;

    // Apply drag to simulate friction
    // velocity *= DRAG;
    speed *= gameSettings->drag;

    // clamp
    if (minX() < 0) pos.x = gameSettings->spaceshipSize / 2;
    if (minY() < 0) pos.y = gameSettings->spaceshipSize / 2;
    if (maxX() >= gameSettings->w) pos.x = gameSettings->w - gameSettings->spaceshipSize / 2;
    if (maxY() >= gameSettings->h) pos.y = gameSettings->h - gameSettings->spaceshipSize / 2;

    weapon.update(deltaTime);
}

void Spaceship::render(SDL_Renderer* renderer) const {
    // SDL_Texture* texture = textures.at(std::to_string(value));
    SDL_Texture* texture = renderTextAsTexture(renderer, gameSettings->sdlSettings->font, std::to_string(value).c_str(), SDL_Color{255, 255, 255});

    int size = gameSettings->spaceshipSize;
    SDL_Rect rect = {static_cast<int>(minX()), static_cast<int>(minY()), size, size};
    SDL_RenderCopyEx(renderer, texture, NULL, &rect, angle + 90.0f, NULL, SDL_FLIP_NONE);

    // Optionally render spaceship direction
    if (active) {
        float tipX = pos.x + size / 2 * std::cos(deg2rad(angle));
        float tipY = pos.y + size / 2 * std::sin(deg2rad(angle));
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderDrawLine(renderer, pos.x, pos.y, tipX, tipY);
    }
}

void Spaceship::applyBoost() {
    // applyingBoost = true;
    // boostEndTime = SDL_GetTicks() + duration;
    applyForce(gameSettings->forceBoost);
}

std::shared_ptr<Projectile> Spaceship::fire() {
    return weapon.fire(pos, angle, gameSettings);
}

void Spaceship::pickUpProjectile(ProjectileType type) {
    weapon.pickUpProjectile(type);
}
