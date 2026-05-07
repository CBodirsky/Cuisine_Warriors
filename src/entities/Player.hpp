#pragma once
#include <SFML/Graphics.hpp>
#include <optional>
#include "../src/entities/Entity.hpp"
#include "../src/battle/CombatManager.hpp"

class GameplayScene;

class Player : public Entity {
public:
    Player();

    void update(float dt) override;
    void update(float dt, const GameplayScene& scene);

    void draw(sf::RenderWindow& window) override;

    void setPosition(const sf::Vector2f& pos) override;
    sf::Vector2f getPosition() const override;
    void applyMovement(const sf::Vector2f& delta);


    Hurtbox getHurtbox() const;
    std::optional<HitboxCircle> getActiveHitbox() const { return activeHitbox; }

    void takeDamage(float amount) override;
    Faction getFaction() const override;

private:
    sf::Sprite sprite;
    float speed = 200.f;
    float hurtRadius = 35.f;

    sf::Vector2f facingDir{ 0.f, -1.f };

    bool isAttacking = false;
    float attackTimer = 0.f;
    float attackDuration = 0.15f;

    sf::RectangleShape attackShape;

    std::optional<HitboxCircle> activeHitbox;

    void startAttack();
};
