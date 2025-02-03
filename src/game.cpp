#include "game.h"
#include <iostream>

SDL_Texture* loadTextureFromPath(SDL_Renderer* renderer, const char* path) {
    SDL_Surface* surface = IMG_Load(path);
    if (!surface) {
        std::cerr << "Failed to load image: " << IMG_GetError() << std::endl;
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(surface);
        return nullptr;
    }

    SDL_FreeSurface(surface);
    return texture;
}

Game::Game(std::shared_ptr<GameSettings> settings) 
    : settings(settings), window(nullptr), renderer(nullptr), player1(nullptr), player2(nullptr) {
}

bool Game::init() {
    // Init
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return false;
    }

    if (IMG_Init(IMG_INIT_PNG) < 0) {
        std::cerr << "Failed to initialize SDL_image: " << IMG_GetError() << std::endl;
        return false;
    }

    if (TTF_Init() < 0) {
        std::cerr << "Failed to initialize SDL_ttf: " << TTF_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow(settings->title, settings->x, settings->y, settings->w, settings->h, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        return false;
    }

    // Load textures
    SDL_Texture* background = loadTextureFromPath(renderer, settings->backgroundImage);
    if (!background) {
        return false;
    }
    textures["background"] = background;

    TTF_Font* font = TTF_OpenFont("assets/font.ttf", 24);
    if (!font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        return false;
    }
    SDL_Color textColor = {255, 255, 255};
    SDL_Color selectedTextColor = {0, 255, 0};
    for (int i = 1; i <= 9; i++) {
        const char* text = std::to_string(i).c_str();
        SDL_Surface* surface = TTF_RenderText_Solid(font, text, textColor);
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

    SDL_Surface* surface = TTF_RenderText_Solid(font, "STALEMATE", textColor);
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
    textures["stalemate"] = texture;

    
    // Load players
    player1 = std::make_unique<Player>(settings, 1);
    player2 = std::make_unique<Player>(settings, 2);

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

void Game::run() {
    bool running = true;
    while (running) {
        float deltaTime = clk.delta();

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            player1->handleEvent(event);
            player2->handleEvent(event);
        }

        handleAdversarialCollision();
        handleMergeCollision();

        // Update game state
        player1->update(deltaTime);
        player2->update(deltaTime);

        // background
        SDL_RenderCopy(renderer, textures["background"], nullptr, nullptr);

        player1->render(renderer, textures);
        player2->render(renderer, textures);

        if (!player1->hasSpaceship() && !player2->hasSpaceship()) {
            SDL_Rect dstRect = {settings->w / 2 - 100, settings->h / 2 - 50, 200, 100};
            SDL_RenderCopy(renderer, textures["stalemate"], nullptr, &dstRect);
        } else if (!player1->hasSpaceship()) {
            std::cout << "Player 2 wins!" << std::endl;
            running = false;
        } else if (!player2->hasSpaceship()) {
            std::cout << "Player 1 wins!" << std::endl;
            running = false;
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
                if (p1->value <= 0) {
                    player1->destroySpaceship(p1->id);
                }
                if (p2->value <= 0) {
                    player2->destroySpaceship(p2->id);
                }
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
