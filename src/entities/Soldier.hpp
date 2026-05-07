#pragma once
#include "entities/Entity.hpp"
#include <SFML/Graphics.hpp>
#include <optional>
#include "ai/AIController.hpp"
#include "battle/CombatManager.hpp"

class GameplayScene;

class Soldier : public Entity {
public:
    Soldier();

    void update(float dt) override;
    void update(float dt, const sf::Vector2f& playerPos, const GameplayScene& scene);
    void draw(sf::RenderWindow& window) override;

    void setPosition(const sf::Vector2f& pos) override;
    sf::Vector2f getPosition() const override;

    // NEW: persistent hitbox getter
    std::optional<HitboxCircle> getActiveHitbox() const { return activeHitbox; }

    // NEW: start an attack (AIController will call this)
    void startAttack(const sf::Vector2f& playerPos);

    Hurtbox getHurtbox() const;
    Faction getFaction() const override;

private:
    AIController ai;
    sf::Sprite sprite;
    sf::RectangleShape attackShape;

    bool isAttacking = false;
    float attackTimer = 0.f;
    float attackDuration = 0.35f;

    float speed = 120.f;
    float hurtRadius = 25.f;

    // NEW: persistent hitbox storage
    std::optional<HitboxCircle> activeHitbox;
};
