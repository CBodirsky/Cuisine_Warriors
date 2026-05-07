#pragma once
#include <SFML/Graphics.hpp>

class GameplayScene;

enum class Faction {
    Player,
    Enemy
};

class Entity {
public:
    virtual ~Entity() = default;

    virtual void update(float dt) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;

    // Position helpers
    virtual void setPosition(const sf::Vector2f& pos) = 0;
    virtual sf::Vector2f getPosition() const = 0;

    virtual void startAttack(const sf::Vector2f& playerPos) {}
    virtual void setAttackShape(const sf::Vector2f& pos, const sf::Vector2f& dir) {}

    virtual void takeDamage(float amount);
    virtual float getHealth() const { return health; }
    virtual Faction getFaction() const = 0;

    bool isDead = false;



protected:
    // Common stats (expand later)
    float health = 100.f;
};
