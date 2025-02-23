#ifndef AI_H
#define AI_H
#include "player.h"

class AI : public Player {
public:
    AI(std::shared_ptr<GameSettings> gameSettings, int playerNumber);
    void handleEvent(SDL_Event& event);
    void update(float deltaTime);
};

#endif