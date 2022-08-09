#include "Game.h"

Game::Game(int size) : field(size) {
    score = 0;
    gameover = false;
    event.type = sf::Event::GainedFocus;

    score_text.setFont(Utils::font);
    score_text.setCharacterSize(48);
    score_text.setPosition({ 20.f, 20.f });
    score_text.setFillColor(Utils::black);

    added_text.setFont(Utils::font);
    added_text.setCharacterSize(32);
    added_text.setPosition({ 100.f, 30.f });
    added_text.setFillColor(Utils::black);
    
    gameover_text.setFont(Utils::font);
    gameover_text.setCharacterSize(60);
    gameover_text.setPosition({ 60.f, 200.f });
    gameover_text.setFillColor(Utils::_64);
    gameover_text.setString("Game over!");

    restart_text.setFont(Utils::font);
    restart_text.setCharacterSize(24);
    restart_text.setPosition({ 40.f, 260.f });
    restart_text.setFillColor(Utils::_0);
    restart_text.setString("Press any button to restart...");
}

void Game::check_events(sf::Event& event, sf::RenderWindow& window) {
    while (window.pollEvent(event)) {
        switch (event.type) {
        case sf::Event::Closed:
            window.close();
            break;
        case sf::Event::KeyPressed:
            if (gameover) {
                field.clear();
                score = 0;
                update_score();
                gameover = false;
                break;
            }
            field.move(event.key.code);
            update_score();
            gameover = field.is_blocked();
            break;
        default:
            break;
        }
    }    
}

void Game::update_score() {
    added_text.setString("+ " + std::to_string(field.get_score() - score));
    score = field.get_score();
    score_text.setString(std::to_string(score));
    added_text.setPosition({score_text.getGlobalBounds().left + score_text.getGlobalBounds().width + 20.f, 30.f});
}

void Game::show(sf::RenderWindow& window) {
    window.clear(Utils::background);

    if (!gameover) {
        window.draw(score_text);
        window.draw(added_text);
        field.show(window);
    }
    else {
        window.draw(gameover_text);
        window.draw(restart_text);
    }

    window.display();
}

void Game::mainloop(sf::RenderWindow& window) {    
    window.pollEvent(event);

    // spawn two initial numbers
    field.spawn();
    field.spawn();

    while (window.isOpen()) {

        check_events(event, window);

        show(window);
    }
}
