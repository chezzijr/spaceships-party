#ifndef SPACESHIP_H
#define SPACESHIP_H

#include <SDL2/SDL.h>

#include <memory>
#include <cmath>
#include <unordered_map>
#include "math.h"
#include "settings.h"
#include "weapon.h"

class Spaceship {
public:
    static int idCounter;
    int id;
    Vector2 pos;
    Vector2 velocity;
    int playerNumber;
    int value;
    Weapon weapon;

    float speed;  // Magnitude of velocity vector
    float angle;   // Rotation angle in degrees
    bool active;
    bool readyForSameSideCollision; // Used to prevent multiple collisions
    bool readyForOppositeSideCollision; // Used to prevent multiple collisions
    std::shared_ptr<GameSettings> gameSettings;

    Spaceship(int playerNumber, std::shared_ptr<GameSettings> settings, float startX, float startY);
    float minX() const;
    float minY() const;
    float maxX() const;
    float maxY() const;
    Circle getCollisionShape() const;
    void toggleActive();

    void rotate(float degrees);
    void applyForce(float magnitude);
    void update(float deltaTime);
    void render(SDL_Renderer* renderer, std::unordered_map<std::string, SDL_Texture*> textures) const;
    void applyBoost();
    std::shared_ptr<Projectile> fire();
    void pickUpProjectile(ProjectileType type);
};

#endif
