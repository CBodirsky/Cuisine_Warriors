#pragma once
#include <SFML/Window.hpp>
#include <SFML/System/Vector2.hpp>

class InputManager {
public:
    // Movement input (WASD)
    static sf::Vector2f getMovementInput();

    // Actions
    static bool isAttackPressed();
    static bool isInteractPressed();
};
