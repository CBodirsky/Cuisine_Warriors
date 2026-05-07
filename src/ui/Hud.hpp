#pragma once
#include <SFML/Graphics.hpp>

class Player;

class Hud {
public:
    Hud();

    void update(const Player& player);
    void draw(sf::RenderWindow& window);

private:
    float maxHealth = 100.f;

    sf::RectangleShape healthBack;
    sf::RectangleShape healthFill;
    sf::Text healthText;

    float padding = 20.f;
};
