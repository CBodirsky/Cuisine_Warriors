#include "InputManager.hpp"
#include <cmath>
#include <SFML/Graphics.hpp>


sf::Vector2f InputManager::getMovementInput() {
    sf::Vector2f dir(0.f, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) dir.y -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) dir.y += 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) dir.x -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) dir.x += 1.f;

    // Normalize
    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (len > 0.f)
        dir /= len;
    return dir;
}
bool InputManager::isAttackPressed() {
    return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);
}
bool InputManager::isInteractPressed() {
    return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E);
}
