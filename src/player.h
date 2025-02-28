#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>

#include "spaceship.h"
#include "settings.h"
#include "projectile.h"
#include <vector>
#include <unordered_map>
#include <memory>

class Agent {
public:
    virtual void handleEvent(SDL_Event& event) = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render(SDL_Renderer* renderer) const = 0;
    virtual std::vector<std::shared_ptr<Spaceship>> getSpaceships() const = 0;
    virtual std::vector<std::shared_ptr<Projectile>> getProjectiles() const = 0;
    virtual void mergeSpaceships(int firstId, int secondId) = 0;
    virtual void destroySpaceship(int id) = 0;
    virtual bool hasSpaceship() const = 0;
    virtual void splitCurrentSpaceship() = 0;
    virtual void rotate(float deltaTime) = 0;
    virtual void rotateAndBoost() = 0;
    virtual void shoot() = 0;
    virtual void switchActiveSpaceship() = 0;
    virtual int pNumber() = 0;
};

class Player : public Agent {
protected:
    std::vector<std::shared_ptr<Spaceship>> spaceships;
    std::vector<std::shared_ptr<Projectile>> projectiles;
    // std::vector<Bullet> bullets; // for future use, collision detection
    size_t activeSpaceship;
    PlayerSettings playerSettings;
    std::shared_ptr<GameSettings> gameSettings;
    int playerNumber;
    // Double press logic
    Uint32 lastLeftPressTime;
    int leftPressCount;
    bool leftHolding;

    public:
    Player(int playerNumber);
    void handleEvent(SDL_Event& event) override;
    void update(float deltaTime) override;
    void render(SDL_Renderer* renderer) const override;
    std::vector<std::shared_ptr<Spaceship>> getSpaceships() const override;
    std::vector<std::shared_ptr<Projectile>> getProjectiles() const override;
    void mergeSpaceships(int firstId, int secondId) override;
    void destroySpaceship(int id) override;
    bool hasSpaceship() const override;
    void splitCurrentSpaceship() override;
    void rotate(float deltaTime) override;
    void rotateAndBoost() override;
    void shoot() override;
    void switchActiveSpaceship() override;
    int pNumber() override;
};

#endif
