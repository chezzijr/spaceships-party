#include "weapon.h"

Weapon::Weapon(WeaponSettings settings)
    : WeaponSettings(settings)
{}

std::shared_ptr<Projectile> Weapon::fire(Vector2 pos, float angle, std::shared_ptr<GameSettings> gameSettings) {
    switch (type) {
        case ProjectileType::BULLET:
            if (bulletAmmo <= 0) {
                return nullptr;
            }
            bulletAmmo--;
            return std::make_shared<Bullet>(pos, angle, gameSettings->bulletSpeed, gameSettings->bulletLifeTime, gameSettings->bulletRadius);
        case ProjectileType::LASER_BEAM:
            type = ProjectileType::BULLET;
            return std::make_shared<LaserBeam>(pos, angle, gameSettings->laserBeamLifeTime, gameSettings->laserBeamWidth);
        case ProjectileType::MINE:
            type = ProjectileType::BULLET;
            return std::make_shared<Mine>(pos, gameSettings->mineActivationTime, gameSettings->mineActiveRadius, gameSettings->mineExplosionRadius);
    }
    return nullptr;
}

void Weapon::update(float delta) {
    if (type == ProjectileType::BULLET) {
        cooldownTimer -= delta;
        if (cooldownTimer < 0) {
            cooldownTimer = cooldown;
            bulletAmmo = std::min(bulletAmmo + 1, maxBulletAmmo);
        }
    }
}

bool Weapon::canFire() const {
    return bulletAmmo > 0;
}

void Weapon::pickUpProjectile(ProjectileType projectile) {
    switch (projectile) {
        case ProjectileType::LASER_BEAM:
            type = ProjectileType::LASER_BEAM;
            break;
        case ProjectileType::MINE:
            type = ProjectileType::MINE;
            break;
    }
}
