#ifndef SETTINGS_H
#define SETTINGS_H

#include <SDL2/SDL.h>
#include <memory>

struct PlayerSettings {
    SDL_Scancode leftBtn, shootBtn, splitBtn, switchBtn;
};

struct GameSettings {
    static std::shared_ptr<GameSettings> instance;
    static std::shared_ptr<GameSettings> get();
    static std::shared_ptr<GameSettings> defaultInstance();

    const char* title;
    int x, y, w, h;
    int fps;
    const char* backgroundImage;

    // PlayerSettings player1;
    // PlayerSettings player2;
    PlayerSettings playerSettings[2];
    int numStartSpaceships;
    float doublePressThreshold;

    // powerup settings
    float powerupSpawnInterval;
    float powerupRadius;

    // spaceship settings
    int spaceshipSize;
    float rotationSpeed, forceBoost, drag, rotBoostDeg;

    // projectile settings
    float bulletSpeed, bulletRadius, bulletLifeTime;
    float laserBeamLifeTime, laserBeamWidth;
    float mineActivationDuration, mineActiveRadius, mineExplosionRadius, mineExplosionDuration, mineSize;
};



#endif
