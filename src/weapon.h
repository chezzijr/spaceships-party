#ifndef WEAPON_H
#define WEAPON_H

#include <SDL2/SDL.h>

#include <memory>
#include "projectile.h"
#include "settings.h"



class Weapon {
private:
    ProjectileType type;
    float cooldown;
    float cooldownTimer;

    int maxBulletAmmo;
    int bulletAmmo;
public:
    Weapon(ProjectileType type, float cooldown, float cooldownTimer, int maxBulletAmmo, int bulletAmmo);
    std::shared_ptr<Projectile> fire(Vector2 pos, float angle, std::shared_ptr<GameSettings> gameSettings);
    void update(float delta);
    bool canFire() const;
    void pickUpProjectile(ProjectileType projectile);
};


#endif
