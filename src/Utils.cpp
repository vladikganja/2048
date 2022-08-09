#include "Utils.h"

namespace Utils {

std::vector<sf::Color> Colors::colors
{_0, _2, _4, _8, _16, _32, _64, _128, _256, _512, _1024, _2048, _4096, _8192, _16384, _32768, _65536 };

sf::Color Colors::at(int value) {
    int index = 0;

    if (value == 0) {
        return colors[0];
    }

    while (!(value & 1)) {
        value >>= 1;
        index++;
    }

    return colors[index];
}

}
