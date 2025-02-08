#ifndef SETTINGS_H
#define SETTINGS_H

struct PlayerSettings {
    SDL_Scancode leftBtn, shootBtn, splitBtn, switchBtn;
};

struct GameSettings {
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
    float rotationSpeed, forceBoost, drag;

    // projectile settings
    float bulletSpeed, bulletRadius, bulletLifeTime;
    float laserBeamLifeTime, laserBeamWidth;
    float mineActivationTime, mineActiveRadius, mineExplosionRadius;
};

#endif
