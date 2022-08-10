#pragma once
#include "Field.h"

class Game {
private:
    Field field;
    std::vector<std::shared_ptr<GUI::Button>> buttons;

    sf::Event event;

    sf::Text score_text;
    sf::Text added_text;
    sf::Text gameover_text;
    sf::Text restart_text;
    int score;
    bool gameover;
    bool best_scores_menu;
    void check_events(sf::Event& event, sf::RenderWindow& window);
    void update_score();
    void show(sf::RenderWindow& window);
    
    JSON::GameCast make_cast();

    void save();
    void load();
    void update_best_scores();
    std::vector<int> load_best_scores();

    std::vector<sf::Text> best_scores;
    void form_best_scores_texts();

public:
    Game();
    void mainloop(sf::RenderWindow& window);
};
