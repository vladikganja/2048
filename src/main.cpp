#include "Game.h"

int main(int argc, char** argv) {

    sf::RenderWindow window(sf::VideoMode(Utils::WINDOW_WIDTH, Utils::WINDOW_HEIGHT), "2048", sf::Style::Close);

    Utils::font.loadFromFile("../../multimedia/CyrilicOld.TTF");

    Game game(argc > 1 ? static_cast<int>(*argv[1]) : 4);
    game.mainloop(window);

    return 0;
}