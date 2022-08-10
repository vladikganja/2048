#include "Game.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(Utils::WINDOW_WIDTH, Utils::WINDOW_HEIGHT), "2048", sf::Style::Close);

    Utils::font.loadFromFile("../../multimedia/CyrilicOld.TTF");

    Game game;
    game.mainloop(window);

    return 0;
}
