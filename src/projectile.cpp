#include "projectile.h"

Bullet::Bullet(Vector2 pos, float angle, float speed, float maxLifeTime, float radius)
    : pos(pos), angle(angle), speed(speed), maxLifeTime(maxLifeTime), lifeTime(0), radius(radius), eol(false)
{}

void Bullet::update(float delta) {
    pos.x += speed * cos(angle * M_PI / 180) * delta;
    pos.y += speed * sin(angle * M_PI / 180) * delta;
    lifeTime += delta;
}

bool Bullet::isCollidingWith(const Circle& shape) const {
    Circle bulletCollisionShape = {pos, radius};
    return shape.collides(bulletCollisionShape);
}

bool Bullet::endOfLife() const {
    return eol || lifeTime >= maxLifeTime;
}

void Bullet::render(SDL_Renderer* renderer) const {
    // draw a rectangle with pos as the center and radius as the width and height
    SDL_Rect rect = {(int)pos.x - radius / 2, (int)pos.y - radius / 2, (int)radius, (int)radius};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &rect);
}

ProjectileType Bullet::getType() const {
    return ProjectileType::BULLET;
}

LaserBeam::LaserBeam(Vector2 pos, float angle, float maxLifeTime, float width)
    :pos(pos), angle(angle), maxLifeTime(maxLifeTime), lifeTime(0), width(width) 
{}

void LaserBeam::update(float delta) {
    lifeTime += delta;
}

bool LaserBeam::isCollidingWith(const Circle& shape) const {
    // beam collision is a straight line with width (rectangle)
    // The beam will go out of the screen, so we need to check if the spaceship is within the beam's path
    // Distance from the center of shape to the line with pos as the origin and angle as the angle
    float distance = abs((shape.center.x - pos.x) * sin(angle * M_PI / 180) - (shape.center.y - pos.y) * cos(angle * M_PI / 180));
    return distance <= shape.radius + width / 2;
}

bool LaserBeam::endOfLife() const {
    return lifeTime >= maxLifeTime;
}

void LaserBeam::render(SDL_Renderer* renderer) const {
    // draw a line with pos as the start point and angle as the angle
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawLine(renderer, pos.x, pos.y, pos.x + 2000 * cos(angle * M_PI / 180), pos.y + 2000 * sin(angle * M_PI / 180));
}

ProjectileType LaserBeam::getType() const {
    return ProjectileType::LASER_BEAM;
}

Mine::Mine(Vector2 pos, float activationTime, float activeRadius, float explosionRadius)
    : pos(pos), activationTime(activationTime), activeRadius(activeRadius), explosionRadius(explosionRadius), activated(false)
{}

void Mine::update(float delta) {
}

bool Mine::isCollidingWith(const Circle& shape) const {
    Circle mineCollisionShape = {pos, activeRadius};
    if (shape.collides(mineCollisionShape)) {
        // Check if the spaceship is within the explosion radius
        return shape.center.distance(mineCollisionShape.center) <= explosionRadius;
    } return false;
}

bool Mine::endOfLife() const {
    return activated && activationTime <= 0;
}

void Mine::render(SDL_Renderer* renderer) const {
    // draw a circle with pos as the center and activeRadius as the radius
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect rect = {(int)pos.x - 5, (int)pos.y - 5, 10, 10};
    SDL_RenderFillRect(renderer, &rect);
}

ProjectileType Mine::getType() const {
    return ProjectileType::MINE;
}
