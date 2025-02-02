#ifndef PLAYER_H
#define PLAYER_H
#include <SDL2/SDL.h>
#include "spaceship.h"
#include "settings.h"
#include <vector>
#include <unordered_map>
#include <memory>

class Player {
private:
    std::vector<std::shared_ptr<Spaceship>> spaceships;
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
    Player(std::shared_ptr<GameSettings> gameSettings, int playerNumber);
    void handleEvent(SDL_Event& event);
    void update(float deltaTime);
    void render(SDL_Renderer* renderer, std::unordered_map<std::string, SDL_Texture*> textures) const;
    std::vector<std::shared_ptr<Spaceship>> getSpaceships() const;
    void mergeSpaceships(int firstId, int secondId);
    void destroySpaceship(int id);
    bool hasSpaceship() const;
    void splitCurrentSpaceship();
};

#endif
