#ifndef AI_H
#define AI_H
#include "player.h"
#include "game.h"
#include "spaceship.h"
#include "math.h"

class Game;
class AI : public Player {
private:
    Game* game;
    float reactionTime;
public:
    AI(int playerNumber, Game* game);
    void handleEvent(SDL_Event& event);
    void update(float deltaTime);
};

#endif