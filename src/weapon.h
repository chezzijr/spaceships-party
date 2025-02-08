#ifndef WEAPON_H
#define WEAPON_H

#ifdef _WIN32
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

#include <memory>
#include "projectile.h"
#include "settings.h"

struct WeaponSettings {
    ProjectileType type;
    float cooldown;
    float cooldownTimer;

    int maxBulletAmmo;
    int bulletAmmo;

    float bulletSpeed, bulletRadius, bulletLifeTime;
    float laserBeamLifeTime, laserBeamWidth;
    float mineActivationTime, mineActiveRadius, mineExplosionRadius;

};

class Weapon : public WeaponSettings {
public:
    Weapon(WeaponSettings settings);
    std::shared_ptr<Projectile> fire(Vector2 pos, float angle, std::shared_ptr<GameSettings> gameSettings);
    void update(float delta);
    bool canFire() const;
    void pickUpProjectile(ProjectileType projectile);
};


#endif
