#include "ui.h"

Button::Button(Vector2 pos, Vector2 size, SDL_Color color, SDL_Texture* texture, std::function<void()> onClickCallback)
    : position(pos), size(size), color(color), texture(texture), onClickCallback(onClickCallback) {}

void Button::handleEvent(SDL_Event* event) {
    if (event->type == SDL_MOUSEBUTTONDOWN) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        if (x > position.x && x < position.x + size.x && y > position.y && y < position.y + size.y) {
            onClickCallback();
        }
    }
}

void Button::render(SDL_Renderer* renderer) {
    SDL_Rect rect = {position.x, position.y, size.x, size.y};
    // draw rect with color
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);
    // draw texture
    SDL_RenderCopy(renderer, texture, NULL, &rect);
}

UI::UI() : running(true) {}

void UI::handleEvent(SDL_Event* event) {
    for (auto& component : components) {
        component->handleEvent(event);
    }
}

void UI::render(SDL_Renderer* renderer) {
    for (auto& component : components) {
        component->render(renderer);
    }
}

void UI::addComponent(std::shared_ptr<Component> component) {
    components.push_back(component);
}

bool UI::isRunning() {
    return running;
}

void UI::stop() {
    running = false;
}

void UI::start() {
    running = true;
}