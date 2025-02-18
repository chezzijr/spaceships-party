#include "game.h"
#include <memory>
int main(int argc, char* argv[]) {
    auto settings = GameSettings::get();

    Game game(settings);
    if (!game.init()) {
        return -1;
    }
    game.run();
}
