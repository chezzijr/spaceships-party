#include "external_force.h"

Force::Force(ForceType type, float strength, float radius, Vector2 position)
    : type(type), strength(strength), radius(radius), position(position) {}

void Force::apply(float delta, const std::vector<std::shared_ptr<Spaceship>>& spaceships, const std::vector<std::shared_ptr<Projectile>>& projectiles) {
    for (auto& spaceship : spaceships) {
        float distance = (spaceship->pos - position).magnitude();
        if (distance < radius) {
            float force = strength * (1 - distance / radius);
            Vector2 direction = (position - spaceship->pos).normalize();
            if (type == ForceType::Repulsion) {
                direction = Vector2(0, 0) - direction;
            }
            spaceship->velocity += direction * force * delta;
        }
    }

    // projectile has angle and speed instead of velocity and speed
    // cast projectile to derived class to access angle
    for (auto& projectile : projectiles) {
        std::shared_ptr<Bullet> bullet = std::dynamic_pointer_cast<Bullet>(projectile);
        if (bullet) {
            float distance = (bullet->pos - position).magnitude();
            if (distance < radius) {
                float force = strength * (1 - distance / radius);
                Vector2 direction = (position - bullet->pos).normalize();
                if (type == ForceType::Repulsion) {
                    direction = Vector2(0, 0) - direction;
                }
                // calculate angle of the direction
                float angle = std::atan2(direction.y, direction.x);
                angle = rad2deg(angle);
                bullet->angle = angle;
                bullet->speed = force;
            }
        }

        std::shared_ptr<Mine> mine = std::dynamic_pointer_cast<Mine>(projectile);
        // mine get dragged by the force, and only has pos
        if (mine) {
            float distance = (mine->pos - position).magnitude();
            if (distance < radius) {
                float force = strength * (1 - distance / radius);
                Vector2 direction = (position - mine->pos).normalize();
                if (type == ForceType::Repulsion) {
                    direction = Vector2(0, 0) - direction;
                }
                mine->pos += direction * force * delta;
            }
        }
    }
}

void Force::render(SDL_Renderer* renderer) const {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 50);
    drawCircle(renderer, {position, radius});
}