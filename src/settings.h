#ifndef SETTINGS_H
#define SETTINGS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <memory>
#include <string>

struct PlayerSettings {
    SDL_Scancode leftBtn, shootBtn, splitBtn, switchBtn;
};

enum class ProjectileType {
    BULLET,
    LASER_BEAM,
    MINE
};

struct WeaponSettings {
    float bulletSpeed, bulletRadius, bulletLifeTime;
    float laserBeamLifeTime, laserBeamWidth;
    float mineActivationDuration, mineActiveRadius, mineExplosionRadius, mineExplosionDuration, mineSize;

};

struct SDL_Settings {
    SDL_Texture* background;
    SDL_Texture* player1WinText;
    SDL_Texture* player2WinText;
    SDL_Texture* stalemateText;
    TTF_Font* font;
    Mix_Music* bulletSound;
    Mix_Music* laserSound;
    SDL_Texture* laserPowerup;
    Mix_Music* mineSound;
    SDL_Texture* minePowerup;

    ~SDL_Settings();
};
struct GameSettings {
    // singleton pattern
    static std::shared_ptr<GameSettings> instance;
    static std::shared_ptr<GameSettings> get();
    static void init(const char* filename);
    static std::shared_ptr<GameSettings> defaultInstance();

    std::string title;
    int x, y, w, h;
    int fps;
    std::string backgroundImage;
    std::string laserBeamSound;
    std::string mineSound;

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
    SDL_Settings* sdlSettings;
    // WeaponSettings* weaponSettings;

    ~GameSettings();
};



#endif
