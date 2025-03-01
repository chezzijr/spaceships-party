#ifndef UI_H
#define UI_H

#include "SDL2/SDL.h"
#include <vector>
#include <memory>
#include <functional>
#include "math.h"

class Component {
public:
    virtual void handleEvent(SDL_Event* event) = 0;
    virtual void render(SDL_Renderer* renderer) = 0;
};

class Button : public Component {
private:
    Vector2 position;
    Vector2 size;
    SDL_Color color;
    SDL_Texture* texture;
    std::function<void()> onClickCallback;
public:
    Button(Vector2 pos, Vector2 size, SDL_Color color, SDL_Texture* texture, std::function<void()> onClickCallback);
    void handleEvent(SDL_Event* event);
    void render(SDL_Renderer* renderer);
};

class UI : public Component {
private:
    std::vector<std::shared_ptr<Component>> components;
    bool running;
public:
    UI();
    void handleEvent(SDL_Event* event);
    void render(SDL_Renderer* renderer);
    void addComponent(std::shared_ptr<Component> component);
    bool isRunning();
    void stop();
    void start();
};

#endif