#pragma once
#include "Utils.h"

class Tile {
private:
    sf::RectangleShape bg_frame;
    sf::RectangleShape bg;
    sf::Text text;
    int number;

public:
    Tile(int i, int j);

    int get_num();
    void add(int value);
    void clear();
    void show(sf::RenderWindow& window);
};

class Field {
private:
    std::mt19937 gen;
    std::vector<std::vector<std::shared_ptr<Tile>>> field;
    std::vector<sf::Vector2i> free;
    int score;

    void push(std::stack<int>& stack, int number, bool& is_last_mixed);
    size_t get_crc();

public:

    Field();

    std::vector<std::shared_ptr<Tile>> operator[](int index);

    void move(sf::Keyboard::Key key);
    void move_up();
    void move_down();
    void move_left();
    void move_right();
    void spawn();

    void find_free();
    void set_score(int value);
    int get_score();
    bool is_blocked();
    void clear();

    void show(sf::RenderWindow& window);
};
