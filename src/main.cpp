#include "Game.h"

int main(int argc, char** argv) {
    sf::RenderWindow window(sf::VideoMode(Utils::WINDOW_WIDTH, Utils::WINDOW_HEIGHT), "2048", sf::Style::Close);

    Utils::font.loadFromFile("../../multimedia/CyrilicOld.TTF");

    Game game(argc > 1 ? *argv[1] - 48 : 4);
    game.mainloop(window);

    return 0;
}