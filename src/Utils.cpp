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

namespace GUI {

Button::Button( const std::string& str, const sf::Vector2f& pos, const sf::Vector2f& size, Actions _mode, 
                const sf::Color& color, bool _active) {
    hovered = false;
    clicked = false;
    text_color = color;
    active = _active;
    mode = _mode;
    button.setPosition(pos);
    button.setSize(size);
    button.setFillColor(text_color);
    text.setFont(Utils::font);
    text.setString(str);
    text.setCharacterSize(48);
    text.setFillColor(Utils::background);
    text.setOutlineThickness(1.f);

    sf::FloatRect button_rect = button.getGlobalBounds();

    float ratio = text.getGlobalBounds().width / button_rect.width;

    if (ratio > 0.9f) {
        float coef = 0.9f * button_rect.width / text.getGlobalBounds().width;
        text.scale({ coef, coef });
    }

    float left = (size.x - text.getGlobalBounds().width) / 2.f + pos.x;
    float top = (size.y - text.getGlobalBounds().height) / 2.f + pos.y - 6.f;

    text.setPosition({ left, top });
}

Actions Button::check_buttons(sf::Vector2i _mouse_pos, std::vector<std::shared_ptr<Button>>& buttons) {
    sf::Vector2f mouse_pos = { static_cast<float>(_mouse_pos.x), static_cast<float>(_mouse_pos.y) };

    for (auto& button : buttons) {
        if (button->get_rect().getGlobalBounds().contains(mouse_pos) && button->is_active()) {
            button->hover();
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                if (!button->is_clicked()) {
                    button->click();
                    return button->get_mode();
                }
            }
            else {
                button->unclick();
            }
        }
        else {
            button->unhover();
        }
    }

    return Actions::NONE;
}

Actions Button::get_mode() {
    return mode;
}

bool Button::is_active() {
    return active;
}

void Button::disable() {
    active = false;
}

void Button::enable() {
    active = true;
}

void Button::hover() { 
    if (!hovered) {
        text.setFillColor(Utils::black);
    }
    hovered = true;
}

void Button::unhover() {
    if (hovered) {
        text.setFillColor(Utils::background);
    }
    hovered = false;
}

bool Button::is_clicked() {
    return clicked;
}

void Button::click() {
    clicked = true;
}

void Button::unclick() {
    clicked = false;
}

void Button::show(sf::RenderWindow& window) {
    window.draw(button);
    window.draw(text);
}

}

