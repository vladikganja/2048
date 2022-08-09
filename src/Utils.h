#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <random>
#include <chrono>
#include <iostream>
#include <stack>

namespace Utils {

constexpr int WINDOW_WIDTH = 404;
constexpr int WINDOW_HEIGHT = 502;

inline sf::Font font;
inline int size = 0;

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
