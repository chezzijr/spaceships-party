#include "projectile.h"
#include <algorithm>
#include "utils.h"
#include <iostream>
Bullet::Bullet(Vector2 pos, float angle, float speed, float maxLifeTime, float radius)
    : pos(pos), angle(angle), speed(speed), maxLifeTime(maxLifeTime), lifeTime(0), radius(radius), eol(false)
{}

void Bullet::update(float delta) {
    pos.x += speed * cos(deg2rad(angle)) * delta;
    pos.y += speed * sin(deg2rad(angle)) * delta;
    lifeTime += delta;

    auto gameSettings = GameSettings::get();
    if (pos.x < radius / 2 || pos.x > gameSettings->w - radius / 2) {
        angle = 180 - angle;
    }
    if (pos.y < radius / 2 || pos.y > gameSettings->h - radius / 2) {
        angle = -angle;
    }
    // clamp the position to the screen
    pos.x = std::clamp(pos.x, radius / 2, gameSettings->w - radius / 2);
    pos.y = std::clamp(pos.y, radius / 2, gameSettings->h - radius / 2);
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
    bool directHit = distance <= shape.radius + width / 2;

    // reflected ray
    auto settings = GameSettings::get();
    RayIntersection res = getRayIntersectionBorder(pos, deg2rad(angle), settings->w, settings->h);
    float reflectAngle = angle;
    if (res.side == BorderSide::LEFT || res.side == BorderSide::RIGHT) {
        reflectAngle = 180 - angle;
    } else if (res.side == BorderSide::TOP || res.side == BorderSide::BOTTOM) {
        reflectAngle = -angle;
    }
    Vector2 reflectPos = res.intersectionPoint;

    distance = abs((shape.center.x - reflectPos.x) * sin(reflectAngle * M_PI / 180) - (shape.center.y - reflectPos.y) * cos(reflectAngle * M_PI / 180));
    bool reflectHit = distance <= shape.radius + width / 2;

    return directHit || reflectHit;
}

bool LaserBeam::endOfLife() const {
    return lifeTime >= maxLifeTime;
}

void LaserBeam::render(SDL_Renderer* renderer) const {
    // draw a line with pos as the start point and angle as the angle
    SDL_SetRenderDrawColor(renderer, 0x00 , 0xdd, 0xc0, 255);

    // detect if the ray cuts the border of the screen and which
    auto settings = GameSettings::get();
    RayIntersection res = getRayIntersectionBorder(pos, deg2rad(angle), settings->w, settings->h);
    float reflectAngle = angle;
    if (res.side == BorderSide::LEFT || res.side == BorderSide::RIGHT) {
        reflectAngle = 180 - angle;
    } else if (res.side == BorderSide::TOP || res.side == BorderSide::BOTTOM) {
        reflectAngle = -angle;
    }
    Vector2 reflectPos = res.intersectionPoint;
    // direct ray
    for (int dx = -width / 2; dx <= width / 2; dx++) {
        for (int dy = -width / 2; dy <= width / 2; dy++) {
            SDL_RenderDrawLine(renderer, pos.x + dx, pos.y + dy, pos.x + 2000 * cos(deg2rad(angle)) + dx, pos.y + 2000 * sin(deg2rad(angle)) + dy);
        }
    }

    // reflected ray
    for (int dx = -width / 2; dx <= width / 2; dx++) {
        for (int dy = -width / 2; dy <= width / 2; dy++) {
            SDL_RenderDrawLine(renderer, reflectPos.x + dx, reflectPos.y + dy, reflectPos.x + 2000 * cos(reflectAngle * M_PI / 180) + dx, reflectPos.y + 2000 * sin(reflectAngle * M_PI / 180) + dy);
        }
    }
}

ProjectileType LaserBeam::getType() const {
    return ProjectileType::LASER_BEAM;
}

Mine::Mine(Vector2 pos, float size, float activationDuration, float activeRadius, float explosionRadius, float explosionDuration)
    : 
    pos(pos), 
    activationDuration(activationDuration), 
    activeRadius(activeRadius), 
    explosionRadius(explosionRadius), 
    activated(false), 
    eol(false), 
    explosionDuration(explosionDuration),
    exploding(false),
    size(size)
{
    // std::cout << "Activation duration: " << activationDuration << std::endl;
}

void Mine::update(float delta) {
    // std::cout << "Delta time: "  << delta << std::endl;
    if (activated && activationDuration > 0) {
        // std::cout << "Activation duration: " << activationDuration << std::endl;
        activationDuration -= delta;
    } else if (activationDuration <= 0.0f && explosionDuration > 0) {
        // Explode
        exploding = true;
        // std::cout << "Explosion duration: " << explosionDuration << std::endl;
        explosionDuration -= delta;
    } else if (explosionDuration <= 0) {
        eol = true;
    }
}

bool Mine::isCollidingWith(const Circle& shape) const {
    if (!activated) {
        Circle mineCollisionShape = {pos, activeRadius};
        if (shape.collides(mineCollisionShape)) {
            // Check if the spaceship is within the explosion radius
            return shape.center.distance(mineCollisionShape.center) <= explosionRadius;
        } 
    } else if (exploding) {
        Circle explosionCollisionShape = {pos, explosionRadius};
        return shape.collides(explosionCollisionShape);
    }
    return false;
}

bool Mine::endOfLife() const {
    return eol;
}

void Mine::render(SDL_Renderer* renderer) const {
    // draw a square with pos as the center and activeRadius as the half width
    // when activated, rotate the square
    auto settings = GameSettings::get();
    if (!exploding) {
        int size = this->size;
        if (!activated) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        } else {
            // shrink
            size = this->size * activationDuration / settings->mineActivationDuration;
            float colorScale = 255.0 * activationDuration / settings->mineActivationDuration;
            SDL_SetRenderDrawColor(renderer, std::max(int(colorScale), 0), 0, 0, 255);
        }
        Circle mineCollisionShape = {pos, size};
        drawCircle(renderer, mineCollisionShape);
    } else {
        Circle explosionCollisionShape = {pos, explosionRadius};
        SDL_SetRenderDrawColor(renderer, 255, 165, 0, 255);
        drawCircle(renderer, explosionCollisionShape);
    }
}

ProjectileType Mine::getType() const {
    return ProjectileType::MINE;
}
