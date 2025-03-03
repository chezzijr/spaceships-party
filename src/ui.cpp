#include "ui.h"
#include <iostream>

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

void UI::clear() {
    components.clear();
}

TextArea::TextArea(Vector2 pos, Vector2 size, int margin, TTF_Font* font, SDL_Color color, std::string text)
    : position(pos), size(size), margin(margin), font(font), color(color), text(text) {}

void TextArea::handleEvent(SDL_Event* event) {}

void TextArea::render(SDL_Renderer* renderer) {
    // make sure the text fits in the area
    // devide and conquer to fit the text
    std::vector<std::string> lines = split(text, '\n');
    // std::vector<std::string> words = split(text, ' ');
    // std::string line = "";
    // int w, h;
    // int i = 0;
    // while (i < words.size()) {
    //     TTF_SizeText(font, (line + " " + words[i]).c_str(), &w, &h);
    //     if (w + 2 * margin >= size.x) {
    //         lines.push_back(line);
    //         line = "";
    //     } else {
    //         line += " " + words[i];
    //         i++;
    //     }
    // }

    int y = position.y;
    for (auto& line : lines) {
        int w, h;
        TTF_SizeText(font, line.c_str(), &w, &h);
        SDL_Texture* texture = renderTextAsTexture(renderer, font, line.c_str(), color);
        SDL_Rect rect = {position.x + margin, y, w, TTF_FontHeight(font)};
        SDL_RenderCopy(renderer, texture, NULL, &rect);
        y += TTF_FontHeight(font) + margin;
        SDL_DestroyTexture(texture);
    }
}