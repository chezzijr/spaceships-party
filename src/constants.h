#ifndef CONSTANTS_H
#define CONSTANTS_H

const char* const TITLE = "Spaceship Movement";
const int X = SDL_WINDOWPOS_CENTERED;
const int Y = SDL_WINDOWPOS_CENTERED;
const int W = 900;
const int H = 900;
const int FPS = 60;
const char* const BACKGROUND_IMAGE = "assets/bg.jpg";
const int NUM_START_SPACESHIPS = 5;
const float DOUBLE_PRESS_THRESHOLD = 0.2f;
const int SPACESHIP_SIZE = 32;
const float ROTATION_SPEED = 360.0f;
const float FORCE_BOOST = 300.0f;
const float DRAG = 0.99f;
const int PLAYER1_LEFT_BTN = SDL_SCANCODE_LEFT;
const int PLAYER1_SHOOT_BTN = SDL_SCANCODE_UP;
const int PLAYER1_SPLIT_BTN = SDL_SCANCODE_DOWN;
const int PLAYER1_SWITCH_BTN = SDL_SCANCODE_RIGHT;
const int PLAYER2_LEFT_BTN = SDL_SCANCODE_A;
const int PLAYER2_SHOOT_BTN = SDL_SCANCODE_W;
const int PLAYER2_SPLIT_BTN = SDL_SCANCODE_S;
const int PLAYER2_SWITCH_BTN = SDL_SCANCODE_D;

#endif
