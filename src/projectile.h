#ifndef PROJECTILE_H
#define PROJECTILE_H

#ifdef _WIN32
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif
#include <memory>
#include "math.h"
#include "settings.h"

enum class ProjectileType {
    BULLET,
    LASER_BEAM,
    MINE
};

class Projectile {
public:
    virtual void update(float delta) = 0;
    virtual bool isCollidingWith(const Circle& shape) const = 0;
    virtual bool endOfLife() const = 0;
    virtual void render(SDL_Renderer* renderer) const = 0;
    virtual ProjectileType getType() const = 0;
};

class Bullet : public Projectile {
public:
    Vector2 pos;
    float angle;
    float speed;
    float lifeTime;
    float maxLifeTime;
    float radius;
    bool eol;
    Bullet(Vector2 pos, float angle, float speed, float maxLifeTime, float radius);
    void update(float delta) override;
    bool isCollidingWith(const Circle& shape) const override;
    bool endOfLife() const override;
    void render(SDL_Renderer* renderer) const override;
    ProjectileType getType() const override;
};

class LaserBeam : public Projectile {
public:
    Vector2 pos;
    float angle;
    float lifeTime;
    float maxLifeTime;
    float width; // width of the beam
    LaserBeam(Vector2 pos, float angle, float maxLifeTime, float width);
    void update(float delta) override;
    bool isCollidingWith(const Circle& shape) const override;
    bool endOfLife() const override;
    void render(SDL_Renderer* renderer) const override;
    ProjectileType getType() const override;
};

class Mine : public Projectile {
public:
    Vector2 pos;
    float activationTime;
    bool activated;
    float activeRadius; // If enemy is within this radius, the mine will be activated
    float explosionRadius;
    Mine(Vector2 pos, float activationTime, float activeRadius, float explosionRadius);
    void update(float delta) override;
    bool isCollidingWith(const Circle& shape) const override;
    bool endOfLife() const override;
    void render(SDL_Renderer* renderer) const override;
    ProjectileType getType() const override;
};

#endif
