#include "Game.h"

Game::Game() {
    score = 0;
    gameover = false;
    best_scores_menu = false;
    event.type = sf::Event::GainedFocus;

    score_text.setFont(Utils::font);
    score_text.setCharacterSize(48);
    score_text.setPosition({ 20.f, 20.f });
    score_text.setFillColor(Utils::black);
    score_text.setString("0");

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

    buttons.push_back(std::shared_ptr<GUI::Button>(
        new GUI::Button("New game", { 230.f, 25.f }, {150.f, 50.f}, GUI::Actions::NEW_GAME, Utils::_8)));
    buttons.push_back(std::shared_ptr<GUI::Button>(
        new GUI::Button("Best results", { 52.f, 350.f }, { 300.f, 100.f }, GUI::Actions::BEST_RESULTS, Utils::background, false)));
    buttons.push_back(std::shared_ptr<GUI::Button>(
        new GUI::Button("Back", { 270.f, 0.f }, { 150.f, 50.f }, GUI::Actions::BACK, Utils::background, false)));

    best_scores.resize(10);
    for (int i = 0; i < best_scores.size(); i++) {
        best_scores[i].setFont(Utils::font);
        best_scores[i].setCharacterSize(44);
        best_scores[i].setPosition({ 10.f, 14.f + i * 46.f });
        best_scores[i].setFillColor(Utils::black);
        best_scores[i].setOutlineThickness(1);
        best_scores[i].setOutlineColor(Utils::_65536);
    }
}

void Game::check_events(sf::Event& event, sf::RenderWindow& window) {
    while (window.pollEvent(event)) {
        switch (event.type) {
        case sf::Event::Closed:
            save();
            window.close();
            break;
        case sf::Event::KeyPressed:
            if (gameover) {
                field.clear();
                score = 0;
                update_score();
                gameover = false;
                buttons[static_cast<size_t>(GUI::Actions::NEW_GAME)]->enable();
                buttons[static_cast<size_t>(GUI::Actions::BEST_RESULTS)]->disable();
                break;
            }
            field.move(event.key.code);
            update_score();
            gameover = field.is_blocked();
            if (gameover) {
                update_best_scores();
                buttons[static_cast<size_t>(GUI::Actions::NEW_GAME)]->disable();
                buttons[static_cast<size_t>(GUI::Actions::BEST_RESULTS)]->enable();
            }
            break;
        default:
            break;
        }
    }

    GUI::Actions action = GUI::Button::check_buttons(sf::Mouse::getPosition(window), buttons);
    switch (action) {
    case GUI::Actions::NEW_GAME:
        update_best_scores();
        field.clear();
        score = 0;
        update_score();
        break;
    case GUI::Actions::BEST_RESULTS:
        form_best_scores_texts();
        best_scores_menu = true;
        buttons[static_cast<size_t>(GUI::Actions::BACK)]->enable();
        buttons[static_cast<size_t>(GUI::Actions::BEST_RESULTS)]->disable();
        break;
    case GUI::Actions::BACK:
        best_scores_menu = false;
        buttons[static_cast<size_t>(GUI::Actions::BACK)]->disable();
        buttons[static_cast<size_t>(GUI::Actions::BEST_RESULTS)]->enable();
        break;
    default:
        break;
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

    for (auto& button : buttons) {
        if (button->is_active()) {
            button->show(window);
        }
    }

    if (best_scores_menu) { // just a crutch, too lazy to create screens logic
        for (auto& text : best_scores) {
            window.draw(text);
        }
        window.display();
        return;
    }

    window.draw(score_text);

    if (!gameover) {
        field.show(window);
        window.draw(added_text);
    }
    else {        
        window.draw(gameover_text);
        window.draw(restart_text);
    }

    window.display();
}

JSON::GameCast Game::make_cast() {
    JSON::GameCast cast;
    for (int i = 0; i < Utils::size; i++) {
        for (int j = 0; j < Utils::size; j++) {
            cast.field.push_back(field[i][j]->get_num());
        }
    }
    cast.score = score;

    return cast;
}

void Game::save() {
    std::ofstream writer("saved_game.json");
    nlohmann::json json = make_cast();  // sophisticated method of creating json with to_json method declared in Utils
    std::cout << json << std::endl;
    writer << std::setw(4) << json << std::endl;
}

void Game::load() {
    std::ifstream reader("saved_game.json");
    JSON::GameCast cast;
    if (reader.is_open()) {
        nlohmann::json json;
        reader >> json;
        cast.field = json["field"].get<std::vector<int>>();
        cast.score = json["score"].get<int>();

        field.set_score(cast.score);
        score = cast.score;

        int index = 0;
        for (int i = 0; i < Utils::size; i++) {
            for (int j = 0; j < Utils::size; j++) {
                field[i][j]->clear();
                field[i][j]->add(cast.field[index++]);
            }
        }
        field.find_free();
    }
    else {
        field.spawn();
        field.spawn();
    }
}

void Game::update_best_scores() {
    std::vector<int> scores = load_best_scores();

    scores.push_back(score);

    std::sort(scores.begin(), scores.end(), [](int a, const int b) -> bool { return a > b; });
    std::unique(scores.begin(), scores.end());

    scores.resize(10);

    std::ofstream writer("best_scores.json");
    nlohmann::json json;    // common method of creating json
    json["scores"] = scores;
    std::cout << json << std::endl;
    writer << std::setw(4) << json << std::endl;
}

std::vector<int> Game::load_best_scores() {
    std::ifstream reader("best_scores.json");
    std::vector<int> scores;
    nlohmann::json json;
    if (reader.is_open()) {
        reader >> json;
        scores = json["scores"].get<std::vector<int>>();
    }
    return scores;
}

void Game::form_best_scores_texts() {
    std::vector<int> scores = load_best_scores();
    for (int i = 0; i < best_scores.size(); i++) {
        best_scores[i].setString(std::to_string(i + 1) + ")\t" + std::to_string(scores[i]));
    }
}

void Game::mainloop(sf::RenderWindow& window) {    
    window.pollEvent(event);

    load();

    while (window.isOpen()) {

        check_events(event, window);

        show(window);
    }
}
