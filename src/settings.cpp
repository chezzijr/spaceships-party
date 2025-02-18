#include "settings.h"

std::shared_ptr<GameSettings> GameSettings::instance = nullptr;

std::shared_ptr<GameSettings> GameSettings::get() {
    if (instance == nullptr) {
        instance = defaultInstance();
    }
    return instance;
}

std::shared_ptr<GameSettings> GameSettings::defaultInstance() {
    return std::make_shared<GameSettings>(GameSettings{
        .title = "Spaceship Movement",
        .x = SDL_WINDOWPOS_CENTERED,
        .y = SDL_WINDOWPOS_CENTERED,
        .w = 1200,
        .h = 900,
        .fps = 60,
        .backgroundImage = "assets/bg.jpg",
        .playerSettings = {
            {
                .leftBtn = SDL_SCANCODE_LEFT,
                .shootBtn = SDL_SCANCODE_UP,
                .splitBtn = SDL_SCANCODE_DOWN,
                .switchBtn = SDL_SCANCODE_RIGHT,
            }, {
                .leftBtn = SDL_SCANCODE_A,
                .shootBtn = SDL_SCANCODE_W,
                .splitBtn = SDL_SCANCODE_S,
                .switchBtn = SDL_SCANCODE_D,
            }
        },
        .numStartSpaceships = 3,
        .doublePressThreshold = 0.2f,
        .powerupSpawnInterval = 2.0f,
        .powerupRadius = 16.0f,
        .spaceshipSize = 32,
        .rotationSpeed = 270.0f,
        .forceBoost = 300.0f,
        .drag = 0.99f,
        .rotBoostDeg = -90.0f,
        .bulletSpeed = 500.0f,
        .bulletRadius = 8.0f,
        .bulletLifeTime = 2.0f,
        .laserBeamLifeTime = 0.1f,
        .laserBeamWidth = 6.0f,
        .mineActivationDuration = 1.0f,
        .mineActiveRadius = 100.0f,
        .mineExplosionRadius = 150.0f,
        .mineExplosionDuration = 0.2f,
        .mineSize = 10.0f
    });
}
