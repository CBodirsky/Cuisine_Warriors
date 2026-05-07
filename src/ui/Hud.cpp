#include "Hud.hpp"
#include "entities/Player.hpp"
#include "core/ResourceManager.hpp"
#include <SFML/Graphics.hpp>

Hud::Hud()
    : healthText(ResourceManager::getFont("assets/fonts/arial.ttf"), "")
{
    healthBack.setSize({ 300.f, 25.f });
    healthBack.setFillColor(sf::Color(50, 50, 50, 200));
    healthBack.setOutlineColor(sf::Color::Black);
    healthBack.setOutlineThickness(2.f);

    healthFill.setSize({ 300.f, 25.f });
    healthFill.setFillColor(sf::Color(200, 50, 50));

    healthText.setCharacterSize(18);
    healthText.setFillColor(sf::Color::White);
}

void Hud::update(const Player& player) {
    float hp = player.getHealth();
    float ratio = hp / maxHealth;
    if (ratio < 0.f) ratio = 0.f;

    healthFill.setSize({ 300.f * ratio, 25.f });
    healthText.setString("HP: " + std::to_string((int)hp));
}

void Hud::draw(sf::RenderWindow& window) {
    sf::Vector2f basePos(
        padding,
        window.getSize().y - padding - 25.f
    );

    healthBack.setPosition(basePos);
    healthFill.setPosition(basePos);
    healthText.setPosition({ basePos.x + 5.f, basePos.y - 22.f });

    window.draw(healthBack);
    window.draw(healthFill);
    window.draw(healthText);
}
