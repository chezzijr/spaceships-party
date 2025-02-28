#include "game.h"
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include "utils.h"


Game::Game() 
    : settings(GameSettings::get()), window(nullptr), renderer(nullptr), player1(nullptr), player2(nullptr), powerupSpawnTimer(0.0f)
{}

bool Game::init() {
    // init random seed
    srand(time(nullptr));

    // Init
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return false;
    }

    if (IMG_Init(IMG_INIT_PNG) < 0) {
        std::cerr << "Failed to initialize SDL_image: " << IMG_GetError() << std::endl;
        return false;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "Failed to initialize SDL_mixer: " << Mix_GetError() << std::endl;
        return false;
    }

    if (TTF_Init() < 0) {
        std::cerr << "Failed to initialize SDL_ttf: " << TTF_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow(settings->title.c_str(), settings->x, settings->y, settings->w, settings->h, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        return false;
    }

    // initialize sdl settings
    SDL_Settings* sdlSettings = new SDL_Settings();

    // Load textures
    sdlSettings->background = IMG_LoadTexture(renderer, settings->backgroundImage.c_str());
    if (sdlSettings->background == nullptr) {
        std::cerr << "Failed to load background image: " << IMG_GetError() << std::endl;
        return false;
    }

    sdlSettings->font = TTF_OpenFont("assets/font.ttf", 24);
    if (sdlSettings->font == nullptr) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        return false;
    }

    SDL_Color textColor = {255, 255, 255};
    SDL_Color selectedTextColor = {0, 255, 0};
    for (int i = 1; i <= 9; i++) {
        const char* text = std::to_string(i).c_str();
        SDL_Surface* surface = TTF_RenderText_Solid(sdlSettings->font, text, textColor);
        if (!surface) {
            std::cerr << "Failed to render text: " << TTF_GetError() << std::endl;
            return false;
        }
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        if (!texture) {
            std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
            return false;
        }
        textures[std::to_string(i)] = texture;
    }

    // powerup textures
    sdlSettings->laserPowerup = renderTextAsTexture(renderer, sdlSettings->font, "/", SDL_Color{0, 0, 255});
    sdlSettings->minePowerup = renderTextAsTexture(renderer, sdlSettings->font, "*", SDL_Color{255, 0, 0});
    if (sdlSettings->laserPowerup == nullptr || sdlSettings->minePowerup == nullptr) {
        return false;
    }

    sdlSettings->stalemateText = renderTextAsTexture(renderer, sdlSettings->font, "Stalemate", textColor);
    sdlSettings->player1WinText = renderTextAsTexture(renderer, sdlSettings->font, "Player 1 Win", textColor);
    sdlSettings->player2WinText = renderTextAsTexture(renderer, sdlSettings->font, "Player 2 Win", textColor);
    if (sdlSettings->stalemateText == nullptr || sdlSettings->player1WinText == nullptr || sdlSettings->player2WinText == nullptr) {
        return false;
    }

    settings->sdlSettings = sdlSettings;
    
    // Load players
    player1 = std::make_shared<Player>(1);
    player2 = std::make_shared<AI>(2, this);

    return true;
}

Game::~Game() {
    for (auto& texture : textures) {
        if (texture.second != nullptr)
            SDL_DestroyTexture(texture.second);
    }

    if (renderer != nullptr)
        SDL_DestroyRenderer(renderer);
    if (window != nullptr)
        SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

std::shared_ptr<Agent> Game::getPlayer1() {
    return player1;
}

std::shared_ptr<Agent> Game::getPlayer2() {
    return player2;
}

void Game::run() {
    bool running = true;
    while (running) {
        float deltaTime = clk.delta();

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            if (player1->hasSpaceship() && player2->hasSpaceship()) {
                player1->handleEvent(event);
                player2->handleEvent(event);
            }
        }

        handleProjectileCollision();
        handleAdversarialCollision();
        handleMergeCollision();
        handlePowerupCollision();

        // Update game state
        player1->update(deltaTime);
        player2->update(deltaTime);

        // Spawn powerups
        powerupSpawnTimer += deltaTime;
        if (powerupSpawnTimer >= settings->powerupSpawnInterval) {
            powerupSpawnTimer = 0.0f;
            float x = rand() % settings->w;
            float y = rand() % settings->h;

            // random laser beam or mine
            ProjectileType type = rand() % 2 == 0 ? ProjectileType::LASER_BEAM : ProjectileType::MINE;
            //! TODO: update mine before enabling it
            // ProjectileType type = ProjectileType::LASER_BEAM;
            Powerup powerup(Vector2(x, y), 10.0f, type);
            powerups.push_back(powerup);
        }

        // background
        SDL_RenderCopy(renderer, settings->sdlSettings->background, nullptr, nullptr);

        player1->render(renderer);
        player2->render(renderer);

        for (auto& powerup : powerups) {
            powerup.render(renderer);
        }

        if (!player1->hasSpaceship() && !player2->hasSpaceship()) {
            SDL_Rect dstRect = {settings->w / 2 - 100, settings->h / 2 - 50, 200, 100};
            SDL_RenderCopy(renderer, settings->sdlSettings->stalemateText, nullptr, &dstRect);
        } else if (!player1->hasSpaceship()) {
            SDL_Rect dstRect = {settings->w / 2 - 100, settings->h / 2 - 50, 200, 100};
            SDL_RenderCopy(renderer, settings->sdlSettings->player2WinText, nullptr, &dstRect);
        } else if (!player2->hasSpaceship()) {
            SDL_Rect dstRect = {settings->w / 2 - 100, settings->h / 2 - 50, 200, 100};
            SDL_RenderCopy(renderer, settings->sdlSettings->player1WinText, nullptr, &dstRect);
        }

        SDL_RenderPresent(renderer);

        SDL_Delay(1000 / settings->fps);
    }
}

void Game::handleAdversarialCollision() {
    auto p1s = player1->getSpaceships();
    auto p2s = player2->getSpaceships();
    std::vector<int> p1collisionCnt(p1s.size(), 0);
    std::vector<int> p2collisionCnt(p2s.size(), 0);

    for (int i = 0; i < p1s.size(); i++) {
        for (int j = 0; j < p2s.size(); j++) {
            auto p1 = p1s[i];
            auto p2 = p2s[j];
            auto p1c = p1->getCollisionShape();
            auto p2c = p2->getCollisionShape();
            if (p1c.collides(p2c)) {
                p1collisionCnt[i]++;
                p2collisionCnt[j]++;
            }
            if (p1c.collides(p2c) && p1->readyForOppositeSideCollision && p2->readyForOppositeSideCollision) {
                p1->value--;
                p2->value--;
                auto diff = p1->pos - p2->pos;
                p1->velocity = (p1->velocity + (p1->pos - p2->pos) * p1->speed).normalize();
                p2->velocity = (p2->velocity + (p2->pos - p1->pos) * p2->speed).normalize();
                p1->speed = (p1->speed + p2->speed) / 2;
                p2->speed = p1->speed;
                p1->readyForOppositeSideCollision = false;
                p2->readyForOppositeSideCollision = false;
            }
        }
    }

    for (int i = 0; i < p1s.size(); i++) {
        if (p1collisionCnt[i] == 0) {
            p1s[i]->readyForOppositeSideCollision = true;
        }
    }
    for (int i = 0; i < p2s.size(); i++) {
        if (p2collisionCnt[i] == 0) {
            p2s[i]->readyForOppositeSideCollision = true;
        }
    }

}

void Game::handleMergeCollision() {
    auto p1s = player1->getSpaceships();
    auto p2s = player2->getSpaceships();
    std::vector<int> p1collisionCnt(p1s.size(), 0);
    std::vector<int> p2collisionCnt(p2s.size(), 0);

    for (int i = 0; i < p1s.size(); i++) {
        for (int j = i + 1; j < p1s.size(); j++) {
            auto p1 = p1s[i];
            auto p2 = p1s[j];
            auto p1c = p1->getCollisionShape();
            auto p2c = p2->getCollisionShape();
            if (p1c.collides(p2c)) {
                p1collisionCnt[i]++;
                p1collisionCnt[j]++;
            }
            if (p1c.collides(p2c) && p1->readyForSameSideCollision && p2->readyForSameSideCollision) {
                player1->mergeSpaceships(p1->id, p2->id);
            }
        }
    }

    for (int i = 0; i < p1s.size(); i++) {
        if (p1collisionCnt[i] == 0) {
            p1s[i]->readyForSameSideCollision = true;
        }
    }

    for (int i = 0; i < p2s.size(); i++) {
        for (int j = i + 1; j < p2s.size(); j++) {
            auto p1 = p2s[i];
            auto p2 = p2s[j];
            auto p1c = p1->getCollisionShape();
            auto p2c = p2->getCollisionShape();
            if (p1c.collides(p2c)) {
                p2collisionCnt[i]++;
                p2collisionCnt[j]++;
            }
            if (p1c.collides(p2c) && p1->readyForSameSideCollision && p2->readyForSameSideCollision) {
                player2->mergeSpaceships(p1->id, p2->id);
            }
        }
    }

    for (int i = 0; i < p2s.size(); i++) {
        if (p2collisionCnt[i] == 0) {
            p2s[i]->readyForSameSideCollision = true;
        }
    }

}

void Game::handleProjectileCollision() {
    auto p1s = player1->getSpaceships();
    auto p2s = player2->getSpaceships();
    auto p1p = player1->getProjectiles();
    auto p2p = player2->getProjectiles();

    for (auto projectile : p1p) {
        for (auto ship : p2s) {
            if (projectile->isCollidingWith(ship->getCollisionShape())) {
                if (projectile->getType() == ProjectileType::BULLET) {
                    ship->value--;
                    auto bullet = std::dynamic_pointer_cast<Bullet>(projectile);
                    bullet->eol = true;
                } else if (projectile->getType() == ProjectileType::MINE) {
                    auto proj = std::dynamic_pointer_cast<Mine>(projectile);
                    // activated by the enemy
                    if (!proj->activated) {
                        proj->activated = true;
                    }
                } else {
                    ship->value = 0;
                }
            }
        }
    }

    for (auto projectile : p2p) {
        for (auto ship : p1s) {
            if (projectile->isCollidingWith(ship->getCollisionShape())) {
                if (projectile->getType() == ProjectileType::BULLET) {
                    ship->value--;
                    // invalidate the projectile
                    auto bullet = std::dynamic_pointer_cast<Bullet>(projectile);
                    bullet->eol = true;
                } else if (projectile->getType() == ProjectileType::MINE) {
                    auto proj = std::dynamic_pointer_cast<Mine>(projectile);
                    // activated by the enemy
                    if (!proj->activated) {
                        proj->activated = true;
                    }
                } else {
                    ship->value = 0;
                }
            }
        }
    }

    p1s.insert(p1s.end(), p2s.begin(), p2s.end()); // all spaceships
    p1p.insert(p1p.end(), p2p.begin(), p2p.end()); // all projectiles
    // mine of any player will kill all spaceships in range if exploded
    for (auto projectile : p1p) {
        for (auto ship : p1s) {
            if (projectile->getType() == ProjectileType::MINE) {
                auto proj = std::dynamic_pointer_cast<Mine>(projectile);
                if (proj->exploding && proj->isCollidingWith(ship->getCollisionShape())) {
                    ship->value = 0;
                }
            }
        }
    }
}

void Game::handlePowerupCollision() {
    auto p1s = player1->getSpaceships();
    auto p2s = player2->getSpaceships();

    for (auto& powerup : powerups) {
        auto powerupShape = powerup.getCollisionShape();
        for (auto p1 : p1s) {
            if (powerupShape.collides(p1->getCollisionShape())) {
                powerup.acquire();
                p1->pickUpProjectile(powerup.getType());
            }
        }

        for (auto p2 : p2s) {
            if (powerupShape.collides(p2->getCollisionShape())) {
                powerup.acquire();
                p2->pickUpProjectile(powerup.getType());
            }
        }
    }

    powerups.erase(std::remove_if(powerups.begin(), powerups.end(), [](const Powerup& powerup) {
        return powerup.isAcquired();
    }), powerups.end());
}
