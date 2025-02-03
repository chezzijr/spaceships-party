#ifndef GAME_H
#define GAME_H

#ifdef _WIN32
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#else
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#endif

#include <unordered_map>
#include <string>
#include <memory>
#include "clock.h"
#include "player.h"
#include "spaceship.h"
#include "settings.h"

class Game {
private:
    std::unordered_map<std::string, SDL_Texture*> textures;
    SDL_Window* window;
    SDL_Renderer* renderer;
    Clock clk;
    // Player *player1, *player2;
    std::unique_ptr<Player> player1, player2;
    std::shared_ptr<GameSettings> settings;

    void handleAdversarialCollision();
    void handleMergeCollision();
public:
    Game(std::shared_ptr<GameSettings> settings);
    ~Game();
    bool init();
    void run();
};

#endif
