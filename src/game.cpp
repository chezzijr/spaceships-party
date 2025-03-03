#include "game.h"
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include "utils.h"
#include "ui.h"


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

    // powerup textures
    sdlSettings->laserPowerup = renderTextAsTexture(renderer, sdlSettings->font, "/", SDL_Color{0, 0, 255});
    sdlSettings->minePowerup = renderTextAsTexture(renderer, sdlSettings->font, "*", SDL_Color{255, 0, 0});
    sdlSettings->plusPowerup = renderTextAsTexture(renderer, sdlSettings->font, "+", SDL_Color{0, 255, 0});
    if (sdlSettings->laserPowerup == nullptr || sdlSettings->minePowerup == nullptr || sdlSettings->plusPowerup == nullptr) {
        std::cerr << "Failed to create powerup texture: " << SDL_GetError() << std::endl;
        return false;
    }

    sdlSettings->stalemateText = renderTextAsTexture(renderer, sdlSettings->font, "Stalemate", textColor);
    sdlSettings->player1WinText = renderTextAsTexture(renderer, sdlSettings->font, "Player 1 Win", textColor);
    sdlSettings->player2WinText = renderTextAsTexture(renderer, sdlSettings->font, "Player 2 Win", textColor);
    if (sdlSettings->stalemateText == nullptr || sdlSettings->player1WinText == nullptr || sdlSettings->player2WinText == nullptr) {
        std::cerr << "Failed to create win text texture: " << SDL_GetError() << std::endl;
        return false;
    }

    // init sound effects'
    sdlSettings->laserSound = Mix_LoadMUS(settings->laserBeamSound.c_str());
    if (sdlSettings->laserSound == nullptr) {
        std::cerr << "Failed to load laser sound: " << Mix_GetError() << std::endl;
        return false;
    }

    sdlSettings->mineSound = Mix_LoadMUS(settings->mineSound.c_str());
    if (sdlSettings->mineSound == nullptr) {
        std::cerr << "Failed to load mine sound: " << Mix_GetError() << std::endl;
        return false;
    }

    sdlSettings->bulletSound = Mix_LoadMUS(settings->bulletSound.c_str());
    if (sdlSettings->bulletSound == nullptr) {
        std::cerr << "Failed to load bullet sound: " << Mix_GetError() << std::endl;
        return false;
    }

    settings->sdlSettings = sdlSettings;


    return true;
}

Game::~Game() {
    if (renderer != nullptr)
        SDL_DestroyRenderer(renderer);
    if (window != nullptr)
        SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void Game::reset() {
    powerups.clear();
    powerupSpawnTimer = 0.0f;
    clk.reset();
}

std::shared_ptr<Agent> Game::getPlayer1() {
    return player1;
}

std::shared_ptr<Agent> Game::getPlayer2() {
    return player2;
}

void Game::playerMenu() {
    UI ui;
    // UI for player selection
    int w = settings->w;
    int h = settings->h;
    int btnW = 300;
    int btnH = 100;
    // middle vertically, flex horizontally
    Button btnHumanPlayer(
        Vector2(w / 4 - btnW / 2, h / 2 - btnH / 2), 
        Vector2(btnW, btnH), 
        SDL_Color{255, 0, 0},
        renderTextAsTexture(renderer, settings->sdlSettings->font, "Human Player", SDL_Color{255, 255, 255}), 
        [&]() {
        player1 = std::make_shared<Player>(1);
        player2 = std::make_shared<Player>(2);
        ui.stop();
    });

    Button btnAIPlayer(
        Vector2(3 * w / 4 - btnW / 2, h / 2 - btnH / 2), 
        Vector2(btnW, btnH), 
        SDL_Color{0, 255, 0}, 
        renderTextAsTexture(renderer, settings->sdlSettings->font, "AI Player", SDL_Color{255, 255, 255}), 
        [&]() {
        player1 = std::make_shared<Player>(1);
        player2 = std::make_shared<AI>(2, this);
        ui.stop();
    });

    ui.addComponent(std::make_shared<Button>(btnHumanPlayer));
    ui.addComponent(std::make_shared<Button>(btnAIPlayer));

    while (ui.isRunning()) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                exit(0);
            }
            ui.handleEvent(&event);
        }

        SDL_RenderClear(renderer);
        // background
        SDL_RenderCopy(renderer, settings->sdlSettings->background, nullptr, nullptr);
        ui.render(renderer);
        SDL_RenderPresent(renderer);
        SDL_Delay(1000 / settings->fps);
    }
}

void Game::tutorialMenu() {
    int w = settings->w;
    int h = settings->h;
    int margin = 20;
    UI ui;
    TTF_Font* font = TTF_OpenFont("assets/font.ttf", 16);
    TextArea player1(
        Vector2(w / 8, h / 4), 
        Vector2(w / 2, h),
        margin,
        font, 
        SDL_Color{255, 255, 255}, 
        "    Player 1:\nLeft to rotate\nDouble left to boost\nUp to fire\nDown to split\nRight to switch spaceship"
    );

    TextArea player2(
        Vector2(w * 5 / 8, h / 4), 
        Vector2(w / 2, h), 
        margin,
        font, 
        SDL_Color{255, 255, 255}, 
        "    Player 2:\nA to rotate\nDouble A to boost\nW to fire\nS to split\nD to switch spaceship"
    );

    Button btnStart(
        Vector2(w / 2 - 150, h - 100), 
        Vector2(300, 100), 
        SDL_Color{0, 255, 0}, 
        renderTextAsTexture(renderer, settings->sdlSettings->font, "Start", SDL_Color{255, 255, 255}), 
        [&]() {
        ui.stop();
    });

    ui.addComponent(std::make_shared<TextArea>(player1));
    ui.addComponent(std::make_shared<TextArea>(player2));
    ui.addComponent(std::make_shared<Button>(btnStart));
    
    while (ui.isRunning()) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                exit(0);
                return;
            }
            ui.handleEvent(&event);
        }

        SDL_RenderClear(renderer);
        // background
        SDL_RenderCopy(renderer, settings->sdlSettings->background, nullptr, nullptr);
        ui.render(renderer);
        SDL_RenderPresent(renderer);
        SDL_Delay(1000 / settings->fps);
    }
}

int Game::gameLoop() {
    bool running = true;
    while (running) {
        float deltaTime = clk.delta();

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                exit(0);
                return false;
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
        if (player1->hasSpaceship() && player2->hasSpaceship()) {
            player1->update(deltaTime);
            player2->update(deltaTime);
        }

        // Spawn powerups
        powerupSpawnTimer += deltaTime;
        if (powerupSpawnTimer >= settings->powerupSpawnInterval) {
            powerupSpawnTimer = 0.0f;
            float x = rand() % settings->w;
            float y = rand() % settings->h;

            // random laser beam or mine
            // ProjectileType type = rand() % 2 == 0 ? ProjectileType::LASER_BEAM : ProjectileType::MINE;
            int r = rand() % 3;
            ProjectileType pw[]{ProjectileType::LASER_BEAM, ProjectileType::MINE, ProjectileType::PLUS};
            ProjectileType type = pw[r];
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
            // SDL_Rect dstRect = {settings->w / 2 - 100, settings->h / 2 - 50, 200, 100};
            // SDL_RenderCopy(renderer, settings->sdlSettings->stalemateText, nullptr, &dstRect);
            return 0;
        } else if (!player1->hasSpaceship()) {
            // SDL_Rect dstRect = {settings->w / 2 - 100, settings->h / 2 - 50, 200, 100};
            // SDL_RenderCopy(renderer, settings->sdlSettings->player2WinText, nullptr, &dstRect);
            return 2;
        } else if (!player2->hasSpaceship()) {
            // SDL_Rect dstRect = {settings->w / 2 - 100, settings->h / 2 - 50, 200, 100};
            // SDL_RenderCopy(renderer, settings->sdlSettings->player1WinText, nullptr, &dstRect);
            return 1;
        }

        SDL_RenderPresent(renderer);

        SDL_Delay(1000 / settings->fps);
    }
}

bool Game::gameOverMenu(int winner) {
    int w = settings->w;
    int h = settings->h;
    bool cont = true;
    UI ui;
    std::string status = winner == 0 ? "Stalemate" : winner == 1 ? "Player 1 Win" : "Player 2 Win";
    TextArea text(
        Vector2(w / 2 - 100, h / 4), 
        Vector2(200, 100), 
        20, 
        settings->sdlSettings->font, 
        SDL_Color{255, 255, 255}, 
        status
    );
    Button btnRestart(
        Vector2(w / 2 - 150, h / 2 - 50), 
        Vector2(300, 100), 
        SDL_Color{0, 255, 0}, 
        renderTextAsTexture(renderer, settings->sdlSettings->font, "Restart", SDL_Color{255, 255, 255}), 
        [&]() {
        ui.stop();
    });
    Button btnQuit(
        Vector2(w / 2 - 150, h / 2 + 50), 
        Vector2(300, 100), 
        SDL_Color{255, 0, 0}, 
        renderTextAsTexture(renderer, settings->sdlSettings->font, "Quit", SDL_Color{255, 255, 255}), 
        [&]() {
        cont = false;
        ui.stop();
    });

    ui.addComponent(std::make_shared<TextArea>(text));
    ui.addComponent(std::make_shared<Button>(btnRestart));
    ui.addComponent(std::make_shared<Button>(btnQuit));

    while (ui.isRunning()) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                exit(0);
                return false;
            }
            ui.handleEvent(&event);
        }

        // background
        ui.render(renderer);
        SDL_RenderPresent(renderer);
        SDL_Delay(1000 / settings->fps);
    }

    return cont;
}

void Game::run() {
    bool cont = true;
    while (cont) {
        playerMenu();
        tutorialMenu();
        int winner = gameLoop();
        cont = gameOverMenu(winner);
        reset();
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

std::vector<std::shared_ptr<Spaceship>> Game::getSpaceships() {
    std::vector<std::shared_ptr<Spaceship>> spaceships;
    auto p1s = player1->getSpaceships();
    auto p2s = player2->getSpaceships();
    spaceships.insert(spaceships.end(), p1s.begin(), p1s.end());
    spaceships.insert(spaceships.end(), p2s.begin(), p2s.end());
    return spaceships;
}

std::vector<std::shared_ptr<Projectile>> Game::getProjectiles() {
    std::vector<std::shared_ptr<Projectile>> projectiles;
    auto p1p = player1->getProjectiles();
    auto p2p = player2->getProjectiles();
    projectiles.insert(projectiles.end(), p1p.begin(), p1p.end());
    projectiles.insert(projectiles.end(), p2p.begin(), p2p.end());
    return projectiles;
}