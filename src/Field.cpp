#include "Field.h"

Tile::Tile(int i, int j) {
    number = 0;
    bg_frame.setPosition({ 2.f + j * (Utils::WINDOW_WIDTH - 4.f) / Utils::size,
                    100.f + i * (Utils::WINDOW_WIDTH - 4.f) / Utils::size });
    bg_frame.setSize({ static_cast<float>(Utils::WINDOW_WIDTH) / Utils::size,
                    static_cast<float>(Utils::WINDOW_WIDTH) / Utils::size });
    bg_frame.setFillColor(Utils::background);

    bg.setPosition({ bg_frame.getPosition().x + 2.f, bg_frame.getPosition().y + 2.f });
    bg.setSize({ bg_frame.getSize().x - 4.f, bg_frame.getSize().y - 4.f });
    bg.setFillColor(Utils::Colors::at(0));

    text.setFont(Utils::font);
    text.setCharacterSize(48);
    text.setFillColor(Utils::black);
}

int Tile::get_num() {
    return number;
}

void Tile::add(int value) {        
    number += value;

    text.setString(std::to_string(number));
    bg.setFillColor(Utils::Colors::at(number));

    sf::FloatRect tile_rect = bg.getGlobalBounds();

    float ratio = text.getGlobalBounds().width / tile_rect.width;

    if (ratio > 0.9f) {
        float coef = 0.9f * tile_rect.width / text.getGlobalBounds().width;
        text.scale({coef, coef});
    }

    sf::FloatRect text_rect = text.getGlobalBounds();

    float left = (tile_rect.width - text_rect.width) / 2.f + tile_rect.left;
    float top = (tile_rect.height - text_rect.height) / 2.f + tile_rect.top - 12.f;

    text.setPosition({left, top});
}

void Tile::clear() {
    if (number != 0) {
        number = 0;
        text.setString("");
        bg.setFillColor(Utils::Colors::at(0));
    }
}

void Tile::show(sf::RenderWindow& window) {
    window.draw(bg_frame);
    window.draw(bg);
    if (number != 0) {
        window.draw(text);
    }
}

void Field::find_free() {
    free.resize(0);
    for (int i = 0; i < Utils::size; i++) {
        for (int j = 0; j < Utils::size; j++) {
            if (field[i][j]->get_num() == 0) {
                free.push_back({ i, j });
            }
        }
    }
}

void Field::push(std::stack<int>& stack, int number, bool& is_last_mixed) {
    if (number != 0) {
        if (!stack.empty()) {
            if (is_last_mixed) {
                stack.push(number);
                is_last_mixed = false;
            }
            else {
                if (stack.top() == number) {
                    stack.pop();
                    stack.push(number * 2);
                    score += number * 2;
                    is_last_mixed = true;
                }
                else {
                    stack.push(number);
                }
            }
        }
        else {
            stack.push(number);
        }
    }
}

size_t Field::get_crc() {
    size_t crc = 0;
    for (int i = 0; i < Utils::size; i++) {
        for (int j = 0; j < Utils::size; j++) {
            if (field[i][j]->get_num() != 0) {
                crc |= 1;
            }
            crc <<= 1;
        }
    }
    return crc;
}

Field::Field() {
    auto now = std::chrono::high_resolution_clock::now();
    gen.seed(static_cast<unsigned>(now.time_since_epoch().count()));
    free.reserve(static_cast<size_t>(Utils::size * Utils::size));

    field.resize(Utils::size);
    for (int i = 0; i < Utils::size; i++) {
        field[i].resize(Utils::size);
        for (int j = 0; j < Utils::size; j++) {
            field[i][j] = std::shared_ptr<Tile>(new Tile(i, j));
            free.push_back({ i, j });
        }
    }

    score = 0;
}

void Field::move(sf::Keyboard::Key key) {

    size_t crc_before = get_crc(); // the state of the field before action

    switch (key) {
    case sf::Keyboard::Left:
        move_left();
        break;
    case sf::Keyboard::Right:
        move_right();
        break;
    case sf::Keyboard::Up:
        move_up();
        break;
    case sf::Keyboard::Down:
        move_down();
        break;
    default:
        break;
    }

    size_t crc_after = get_crc(); // the state of the field after action

    find_free();

    if (free.size() > 0 && crc_before != crc_after) {
        spawn();
    }
}

void Field::move_up() {
    for (int col = 0; col < Utils::size; col++) {
        std::stack<int> stack;
        bool is_last_mixed = false;
        for (int line = 0; line < Utils::size; line++) {
            push(stack, field[line][col]->get_num(), is_last_mixed);
            field[line][col]->clear();
        }
        if (stack.size() != 0) {
            for (int line = static_cast<int>(stack.size()) - 1; line >= 0; line--) {
                field[line][col]->add(stack.top());
                stack.pop();
            }
        }
    }
}

void Field::move_down() {
    for (int col = 0; col < Utils::size; col++) {
        std::stack<int> stack;
        bool is_last_mixed = false;
        for (int line = Utils::size - 1; line >= 0; line--) {
            push(stack, field[line][col]->get_num(), is_last_mixed);
            field[line][col]->clear();
        }
        if (stack.size() != 0) {
            for (int line = Utils::size - static_cast<int>(stack.size()); line < Utils::size; line++) {
                field[line][col]->add(stack.top());
                stack.pop();
            }
        }
    }
}

void Field::move_left() {
    for (int line = 0; line < Utils::size; line++) {
        std::stack<int> stack;
        bool is_last_mixed = false;
        for (int col = 0; col < Utils::size; col++) {
            push(stack, field[line][col]->get_num(), is_last_mixed);
            field[line][col]->clear();
        }
        if (stack.size() != 0) {
            for (int col = static_cast<int>(stack.size()) - 1; col >= 0; col--) {
                field[line][col]->add(stack.top());
                stack.pop();
            }
        }
    }
}

void Field::move_right() {
    for (int line = 0; line < Utils::size; line++) {
        std::stack<int> stack;
        bool is_last_mixed = false;
        for (int col = Utils::size - 1; col >= 0; col--) {
            push(stack, field[line][col]->get_num(), is_last_mixed);
            field[line][col]->clear();
        }
        if (stack.size() != 0) {
            for (int col = Utils::size - static_cast<int>(stack.size()); col < Utils::size; col++) {
                field[line][col]->add(stack.top());
                stack.pop();
            }
        }
    }
}

std::vector<std::shared_ptr<Tile>> Field::operator[](int index) {
    return field[index];
}

void Field::spawn() {
    int index = gen() % free.size();
    field[free[index].x][free[index].y]->add(((gen() % 2) + 1) * 2);
    free.erase(free.begin() + index);
}

void Field::set_score(int value) {
    score = value;
}

int Field::get_score() {
    return score;
}

bool Field::is_blocked() {
    // fulfillness
    for (int i = 0; i < Utils::size; i++) {
        for (int j = 0; j < Utils::size; j++) {
            if (field[i][j]->get_num() == 0) {
                return false;
            }
        }
    }

    // vertical block
    for (int col = 0; col < Utils::size; col++) {
        for (int line = 0; line < Utils::size - 1; line++) {
            if (field[line][col]->get_num() == field[line + 1][col]->get_num()) {
                return false;
            }
        }
    }

    // horizontal block
    for (int line = 0; line < Utils::size; line++) {
        for (int col = 0; col < Utils::size - 1; col++) {
            if (field[line][col]->get_num() == field[line][col + 1]->get_num()) {
                return false;
            }
        }
    }

    return true;
}

void Field::clear() {
    free.resize(0);
    for (int i = 0; i < Utils::size; i++) {
        for (int j = 0; j < Utils::size; j++) {
            field[i][j]->clear();
            free.push_back({ i, j });
        }
    }
    spawn();
    spawn();

    score = 0;
}

void Field::show(sf::RenderWindow& window) {
    for (int i = 0; i < Utils::size; i++) {
        for (int j = 0; j < Utils::size; j++) {
            field[i][j]->show(window);
        }
    }
}
