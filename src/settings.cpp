#include "settings.h"
#include <nlohmann/json.hpp>
#include <fstream>
using json = nlohmann::json;

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
        .laserBeamSound = "assets/laser.mp3",
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
        .mineSize = 10.0f,
        .sdlSettings = nullptr
    });
}

void GameSettings::init(const char* filename) {
    auto defaultSettings = defaultInstance();
    std::ifstream file(filename);
    // not a valid filen
    if (!file || !file.is_open()) {
        instance = defaultSettings;
        return;
    }
    json j = json::parse(file);
    instance = std::make_shared<GameSettings>(GameSettings{
        .title = j.value("title", defaultSettings->title),
        .x = j.value("x", defaultSettings->x),
        .y = j.value("y", defaultSettings->y),
        .w = j.value("w", defaultSettings->w),
        .h = j.value("h", defaultSettings->h),
        .fps = j.value("fps", defaultSettings->fps),
        .backgroundImage = j.value("backgroundImage", defaultSettings->backgroundImage),
        .laserBeamSound = j.value("laserBeamSound", defaultSettings->laserBeamSound),
        .playerSettings = {
            {
                .leftBtn = j.value("player1.leftBtn", defaultSettings->playerSettings[0].leftBtn),
                .shootBtn = j.value("player1.shootBtn", defaultSettings->playerSettings[0].shootBtn),
                .splitBtn = j.value("player1.splitBtn", defaultSettings->playerSettings[0].splitBtn),
                .switchBtn = j.value("player1.switchBtn", defaultSettings->playerSettings[0].switchBtn),
            }, {
                .leftBtn = j.value("player2.leftBtn", defaultSettings->playerSettings[1].leftBtn),
                .shootBtn = j.value("player2.shootBtn", defaultSettings->playerSettings[1].shootBtn),
                .splitBtn = j.value("player2.splitBtn", defaultSettings->playerSettings[1].splitBtn),
                .switchBtn = j.value("player2.switchBtn", defaultSettings->playerSettings[1].switchBtn),
            }
        },
        .numStartSpaceships = j.value("numStartSpaceships", defaultSettings->numStartSpaceships),
        .doublePressThreshold = j.value("doublePressThreshold", defaultSettings->doublePressThreshold),
        .powerupSpawnInterval = j.value("powerupSpawnInterval", defaultSettings->powerupSpawnInterval),
        .powerupRadius = j.value("powerupRadius", defaultSettings->powerupRadius),
        .spaceshipSize = j.value("spaceshipSize", defaultSettings->spaceshipSize),
        .rotationSpeed = j.value("rotationSpeed", defaultSettings->rotationSpeed),
        .forceBoost = j.value("forceBoost", defaultSettings->forceBoost),
        .drag = j.value("drag", defaultSettings->drag),
        .rotBoostDeg = j.value("rotBoostDeg", defaultSettings->rotBoostDeg),
        .bulletSpeed = j.value("bulletSpeed", defaultSettings->bulletSpeed),
        .bulletRadius = j.value("bulletRadius", defaultSettings->bulletRadius),
        .bulletLifeTime = j.value("bulletLifeTime", defaultSettings->bulletLifeTime),
        .laserBeamLifeTime = j.value("laserBeamLifeTime", defaultSettings->laserBeamLifeTime),
        .laserBeamWidth = j.value("laserBeamWidth", defaultSettings->laserBeamWidth),
        .mineActivationDuration = j.value("mineActivationDuration", defaultSettings->mineActivationDuration),
        .mineActiveRadius = j.value("mineActiveRadius", defaultSettings->mineActiveRadius),
        .mineExplosionRadius = j.value("mineExplosionRadius", defaultSettings->mineExplosionRadius),
        .mineExplosionDuration = j.value("mineExplosionDuration", defaultSettings->mineExplosionDuration),
        .mineSize = j.value("mineSize", defaultSettings->mineSize),
        .sdlSettings = nullptr
    });
}

GameSettings::~GameSettings() {
    if (sdlSettings != nullptr) {
        delete sdlSettings;
    }
}

SDL_Settings::~SDL_Settings() {
    if (background != nullptr) {
        SDL_DestroyTexture(background);
    }
    if (player1WinText != nullptr) {
        SDL_DestroyTexture(player1WinText);
    }
    if (player2WinText != nullptr) {
        SDL_DestroyTexture(player2WinText);
    }
    if (bulletSound != nullptr) {
        Mix_FreeChunk(bulletSound);
    }
    if (laserSound != nullptr) {
        Mix_FreeChunk(laserSound);
    }
    if (mineSound != nullptr) {
        Mix_FreeChunk(mineSound);
    }
}