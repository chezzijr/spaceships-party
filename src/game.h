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
friend class AI;
private:
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
    std::shared_ptr<Agent> getPlayer1();
    std::shared_ptr<Agent> getPlayer2();
    std::vector<std::shared_ptr<Spaceship>> getSpaceships();
    std::vector<std::shared_ptr<Projectile>> getProjectiles();
    void reset();
    void playerMenu();
    void tutorialMenu();
    int gameLoop();
    bool gameOverMenu(int winner);
public:
    Game();
    ~Game();
    bool init();
    void run();
};

#endif
