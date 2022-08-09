#pragma once
#include "Field.h"

class Game {
private:
    Field field;
    sf::Event event;

    sf::Text score_text;
    sf::Text added_text;
    sf::Text gameover_text;
    sf::Text restart_text;
    int score;
    bool gameover;
    void check_events(sf::Event& event, sf::RenderWindow& window);
    void update_score();
    void show(sf::RenderWindow& window);

public:
    Game(int size);
    void mainloop(sf::RenderWindow& window);
};
