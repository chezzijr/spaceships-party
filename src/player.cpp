#include "player.h"
#include <iostream>
#include <algorithm>

Player::Player(std::shared_ptr<GameSettings> gameSettings, int playerNumber)
    : gameSettings(gameSettings), playerNumber(playerNumber), activeSpaceship(0), lastLeftPressTime(0.0), leftPressCount(0), leftHolding(false)
{

    playerSettings = gameSettings->playerSettings[playerNumber - 1];
    float spawnX = (playerNumber == 1) ? gameSettings->w / 8 : 7 * gameSettings->w / 8;
    for (int i = 1; i <= gameSettings->numStartSpaceships; i++) {
        float spawnY = gameSettings->h / (gameSettings->numStartSpaceships + 1) * i;
        spaceships.push_back(std::make_shared<Spaceship>(playerNumber, gameSettings, spawnX, spawnY));
    }
    spaceships[activeSpaceship]->toggleActive();
}

void Player::handleEvent(SDL_Event& event) {
    SDL_Keycode leftKey = SDL_GetKeyFromScancode(playerSettings.leftBtn);
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == leftKey) {
        if (!leftHolding) {
            Uint32 currentTime = SDL_GetTicks();

            if (double(currentTime - lastLeftPressTime) / 1000.0 <= gameSettings->doublePressThreshold) {
                leftPressCount++;
            } else {
                leftPressCount = 1; // Reset if outside interval
            }

            lastLeftPressTime = currentTime;

            if (leftPressCount == 2) {
                // Double press: Turn 90 degrees left and apply boost
                spaceships[activeSpaceship]->rotate(gameSettings->rotBoostDeg);
                spaceships[activeSpaceship]->applyBoost(); // Boost for 100 ms
                leftPressCount = 0; // Reset press count
            }
        }
        leftHolding = true;
    } else if (event.type == SDL_KEYUP && event.key.keysym.sym == leftKey) {
        leftHolding = false; // Button released

        if (leftPressCount == 2) {
            // Double press: Turn 90 degrees left and apply boost
            spaceships[activeSpaceship]->rotate(gameSettings->rotBoostDeg);
            spaceships[activeSpaceship]->applyBoost(); // Boost for 100 ms
            leftPressCount = 0; // Reset press count
        }
    } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDL_GetKeyFromScancode(playerSettings.shootBtn)) {
        // Shoot
        //! TODO: Implement
        auto projectile = spaceships[activeSpaceship]->fire();
        if (projectile != nullptr) {
            projectiles.push_back(projectile);
        }
    } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDL_GetKeyFromScancode(playerSettings.splitBtn)) {
        // Split
        splitCurrentSpaceship();
    } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDL_GetKeyFromScancode(playerSettings.switchBtn)) {
        // Switch active spaceship
        spaceships[activeSpaceship]->toggleActive();
        activeSpaceship = (activeSpaceship + 1) % spaceships.size();
        spaceships[activeSpaceship]->toggleActive();
    }
}

void Player::update(float deltaTime) {
    // Check if the left key is being held down
    const Uint8* keystate = SDL_GetKeyboardState(NULL);
    if (keystate[playerSettings.leftBtn]) {
        spaceships[activeSpaceship]->rotate(-gameSettings->rotationSpeed * deltaTime);
    }

    for (auto spaceship : spaceships) {
        spaceship->update(deltaTime);
    }

    for (auto projectile : projectiles) {
        projectile->update(deltaTime);
    }

    // removing spaceships with value <= 0 using destroySpaceship
    std::vector<int> destroyedSpaceships;
    for (auto spaceship : spaceships) {
        if (spaceship->value <= 0) {
            destroyedSpaceships.push_back(spaceship->id);
        }
    }

    for (int id : destroyedSpaceships) {
        destroySpaceship(id);
    }

    // removing projectiles that are out of life
    projectiles.erase(std::remove_if(projectiles.begin(), projectiles.end(), [](std::shared_ptr<Projectile> projectile) {
        return projectile->endOfLife();
    }), projectiles.end());
}

void Player::render(SDL_Renderer* renderer, std::unordered_map<std::string, SDL_Texture*> textures) const {
    for (auto spaceship : spaceships) {
        spaceship->render(renderer, textures);
    }

    for (auto projectile : projectiles) {
        projectile->render(renderer);
    }
}

std::vector<std::shared_ptr<Spaceship>> Player::getSpaceships() const {
    return spaceships;
}

void Player::mergeSpaceships(int firstId, int secondId) {
    // Merge two spaceships
    std::shared_ptr<Spaceship> first = nullptr;
    std::shared_ptr<Spaceship> second = nullptr;


    // If the merging spaceships are the active one
    // The new spaceship will be the active one
    // Else not change the active spaceship
    int activeId = spaceships[activeSpaceship]->id;
    for (auto spaceship : spaceships) {
        if (spaceship->id == firstId) {
            first = spaceship;
        } else if (spaceship->id == secondId) {
            second = spaceship;
        }
    }

    if (first == nullptr || second == nullptr) {
        return;
    }

    std::shared_ptr<Spaceship> newSpaceship = std::make_shared<Spaceship>(playerNumber, gameSettings, (first->pos.x + second->pos.x) / 2, (first->pos.y + second->pos.y) / 2);
    newSpaceship->velocity = (first->velocity * first->speed + second->velocity * second->speed).normalize();
    newSpaceship->speed = (first->speed + second->speed) / 2;
    newSpaceship->angle = abs(first->angle - second->angle) / 2;
    newSpaceship->value = first->value + second->value;

    // remove the two old spaceships
    spaceships.erase(std::remove_if(spaceships.begin(), spaceships.end(), [firstId, secondId](std::shared_ptr<Spaceship> spaceship) {
        return spaceship->id == firstId || spaceship->id == secondId;
    }), spaceships.end());

    // add the new spaceship and set it as active
    spaceships.push_back(newSpaceship);
    if (activeId == firstId || activeId == secondId) {
        activeSpaceship = spaceships.size() - 1;
        spaceships[activeSpaceship]->toggleActive();
        return;
    } 
    for (int i = 0; i < spaceships.size(); i++) {
        if (spaceships[i]->id == activeId) {
            activeSpaceship = i;
            return;
        }
    }
}

void Player::destroySpaceship(int id) {
    // switch to the next spaceship if the active one is destroyed
    for (int i = 0; i < spaceships.size(); i++) {
        if (spaceships[i]->id == id) {
            int activeBefore = activeSpaceship;
            if (i < activeBefore) {
                activeSpaceship--;
            }
            spaceships.erase(spaceships.begin() + i);
            if (activeBefore == i) {
                if (activeSpaceship == spaceships.size()) {
                    activeSpaceship = 0;
                }
                spaceships[activeSpaceship]->toggleActive();
            }
            return;
        }
    }
}

bool Player::hasSpaceship() const {
    return !spaceships.empty();
}

void Player::splitCurrentSpaceship() {
    auto spaceship = spaceships[activeSpaceship];
    if (spaceship->value < 2) {
        return;
    }
    auto newSpaceship = std::make_shared<Spaceship>(playerNumber, gameSettings, spaceship->pos.x, spaceship->pos.y);
    newSpaceship->velocity = Vector2(0.0, 0.0) - spaceship->velocity;
    newSpaceship->speed = spaceship->speed / 2;
    newSpaceship->angle = -spaceship->angle;
    newSpaceship->value = spaceship->value / 2;
    newSpaceship->readyForSameSideCollision = false;
    spaceship->value = spaceship->value - newSpaceship->value;
    spaceship->speed = spaceship->speed + spaceship->speed / 2;

    spaceships.push_back(newSpaceship);
    return;
}

std::vector<std::shared_ptr<Projectile>> Player::getProjectiles() const {
    return projectiles;
}
