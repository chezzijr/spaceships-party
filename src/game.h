#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <unordered_map>
#include <string>
#include <memory>
#include "clock.h"
#include "player.h"
#include "spaceship.h"
#include "settings.h"
#include "powerup.h"

class Game {
private:
    std::unordered_map<std::string, SDL_Texture*> textures;
    SDL_Window* window;
    SDL_Renderer* renderer;
    Clock clk;
    std::unique_ptr<Player> player1, player2;
    std::shared_ptr<GameSettings> settings;

    std::vector<Powerup> powerups;
    float powerupSpawnTimer;

    void handleAdversarialCollision();
    void handleMergeCollision();
    void handleProjectileCollision();
    void handlePowerupCollision();
public:
    Game(std::shared_ptr<GameSettings> settings);
    ~Game();
    bool init();
    void run();
};

#endif
