#pragma once
#include <SFML/Graphics.hpp>

class Scene {
public:
    virtual void handleEvent(const sf::Event&) = 0;
    virtual void update(float dt) = 0;
    virtual void draw(sf::RenderWindow&) = 0;
    virtual ~Scene() = default;
};
