#include "game.h"
#include <memory>

#ifndef _WIN32
int main(int argc, char* argv[])
#else
#include <windows.h>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
#endif
{
    auto settings = GameSettings::get();

    Game game(settings);
    if (!game.init()) {
        return -1;
    }
    game.run();
}