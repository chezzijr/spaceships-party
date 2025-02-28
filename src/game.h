#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <unordered_map>
#include <string>
#include <memory>
#include "clock.h"
#include "player.h"
#include "ai.h"
#include "spaceship.h"
#include "settings.h"
#include "powerup.h"

class Game {
private:
    std::unordered_map<std::string, SDL_Texture*> textures;
    SDL_Window* window;
    SDL_Renderer* renderer;
    Clock clk;
    std::shared_ptr<Agent> player1, player2;
    std::shared_ptr<GameSettings> settings;

    std::vector<Powerup> powerups;
    float powerupSpawnTimer;

    void handleAdversarialCollision();
    void handleMergeCollision();
    void handleProjectileCollision();
    void handlePowerupCollision();
public:
    Game();
    ~Game();
    std::shared_ptr<Agent> getPlayer1();
    std::shared_ptr<Agent> getPlayer2();
    bool init();
    void run();
};

#endif
