#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <random>
#include <chrono>
#include <iostream>
#include <stack>
#include "nlohmann/json.hpp"
#include <fstream>
#include <algorithm>

namespace Utils {

constexpr int WINDOW_WIDTH = 404;
constexpr int WINDOW_HEIGHT = 502;

inline sf::Font font;
constexpr int size = 4;

const sf::Color background(187, 173, 160);
const sf::Color black(0, 0, 0);

const sf::Color _0(205, 193, 180);
const sf::Color _2(238, 228, 218);
const sf::Color _4(238, 225, 201);
const sf::Color _8(243, 178, 122);
const sf::Color _16(246, 150, 100);
const sf::Color _32(247, 124, 95);
const sf::Color _64(247, 95, 59);
const sf::Color _128(237, 208, 115);
const sf::Color _256(198, 237, 115);
const sf::Color _512(144, 237, 115);
const sf::Color _1024(115, 237, 166);
const sf::Color _2048(115, 233, 237);
const sf::Color _4096(115, 166, 237);
const sf::Color _8192(117, 115, 237);

const sf::Color _16384(172, 115, 237);
const sf::Color _32768(219, 115, 237);
const sf::Color _65536(237, 115, 115);

class Colors {
public:
    static std::vector<sf::Color> colors;
    static sf::Color at(int value);
};

}

namespace GUI {

enum class Actions {
    NONE = -1,
    NEW_GAME = 0,
    BEST_RESULTS = 1,
    BACK = 2
};

class Button {
private:
    sf::RectangleShape button;
    sf::Text text;
    sf::Color text_color;
    Actions mode;

    bool active;
    bool hovered;
    bool clicked;

public:
    Button( const std::string& str, const sf::Vector2f& pos, const sf::Vector2f& size, Actions mode, 
            const sf::Color& color, bool active = true);
    sf::RectangleShape& get_rect() {
        return button;
    }
    sf::Text& get_text() {
        return text;
    }
    static Actions check_buttons(sf::Vector2i mouse_pos, std::vector<std::shared_ptr<Button>>& buttons);
    Actions get_mode();

    bool is_active();
    void disable();
    void enable();

    void hover();
    void unhover();

    bool is_clicked();
    void click();
    void unclick();

    void show(sf::RenderWindow& window);
};

}

namespace JSON {

class GameCast {
public:
    GameCast() {
        score = 0;
        field.reserve(static_cast<size_t>(Utils::size * Utils::size));
    }
    std::vector<int> field;
    int score;
};

inline void to_json(nlohmann::json& j, const GameCast& cast) {
    j = nlohmann::json{ {"field", cast.field}, {"score", cast.score}};
}

inline void from_json(const nlohmann::json& j, GameCast& cast) {
    j.at("field").get_to(cast.field);
    j.at("score").get_to(cast.score);
}

}
