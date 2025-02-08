#include "game.h"
#include <memory>
int main(int argc, char* argv[]) {
    auto settings = std::make_shared<GameSettings>(GameSettings{
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
        .numStartSpaceships = 5,
        .doublePressThreshold = 0.2f,
        .powerupSpawnInterval = 5.0f,
        .powerupRadius = 16.0f,
        .spaceshipSize = 32,
        .rotationSpeed = 360.0f,
        .forceBoost = 300.0f,
        .drag = 0.99f,
        .bulletSpeed = 500.0f,
        .bulletRadius = 8.0f,
        .bulletLifeTime = 1.0f,
        .laserBeamLifeTime = 0.1f,
        .laserBeamWidth = 10.0f,
        .mineActivationTime = 1.0f,
        .mineActiveRadius = 100.0f,
        .mineExplosionRadius = 200.0f,
    });

    Game game(settings);
    if (!game.init()) {
        return -1;
    }
    game.run();
}
