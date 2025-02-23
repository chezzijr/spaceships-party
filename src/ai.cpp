#include "ai.h"

AI::AI(std::shared_ptr<GameSettings> gameSettings, int playerNumber)
    : Player(gameSettings, playerNumber)
{}

void AI::handleEvent(SDL_Event& event) {
    // do random
    int num = rand() % 100;
    if (num < 25) {
        rotateAndBoost();
    } else if (num < 50) {
        shoot();
    } else if (num < 75) {
        switchActiveSpaceship();
    } else {
        splitCurrentSpaceship();
    }
}

void AI::update(float deltaTime) {
    // Check if the left key is being held down
    if (rand() % 100 < 50) {
        spaceships[activeSpaceship]->rotate(-gameSettings->rotationSpeed * deltaTime);
    }
    Player::update(deltaTime);
}