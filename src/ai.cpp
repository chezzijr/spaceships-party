#include "ai.h"
#include <algorithm>
#include <memory>
#include <iostream>

const float REACTION_TIME = 0.5f;

AI::AI(int playerNumber, Game* game)
    : Player(playerNumber), game(game), reactionTime(REACTION_TIME)
{}

// AI need not handle events
void AI::handleEvent(SDL_Event& event) {}

void AI::update(float deltaTime) {
    rotate(deltaTime);
    
    std::shared_ptr<Agent> player = (game->getPlayer1()->pNumber() == pNumber()) ? game->getPlayer2() : game->getPlayer1();
    auto enemies = player->getSpaceships();
    auto spaceship = spaceships[activeSpaceship];
    for (auto& enemy : enemies) {
        Vector2 direction = enemy->pos - spaceship->pos;
        float angle = spaceship->velocity.angleBetween(direction); // in degrees
        if (-5 <= angle && angle <= 5) {
            shoot();
        }
    }
    
    reactionTime -= deltaTime;
    if (reactionTime <= 0) {
        int randomAction = rand() % 100;
        reactionTime = REACTION_TIME;
        if (randomAction < 33) {
            rotateAndBoost();
        } else if (randomAction < 66) {
            splitCurrentSpaceship();
        } else {
            switchActiveSpaceship();
        }
        
    }
    Player::update(deltaTime);
}