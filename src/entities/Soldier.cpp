#include "Soldier.hpp"
#include "core/ResourceManager.hpp"
#include <cmath>
#include "scenes/GameplayScene.hpp"

Soldier::Soldier()
    : sprite(ResourceManager::getTexture("assets/textures/Soldier.png"))
{
    sprite.setOrigin(sprite.getLocalBounds().getCenter());

    attackShape.setSize({ 40.f, 12.f });
    attackShape.setOrigin({ 20.f, 6.f }); // true center
    attackShape.setFillColor(sf::Color(255, 0, 0, 120));

    ai.setState(AIState::ChasePlayer);
}

Faction Soldier::getFaction() const { return Faction::Enemy; }

void Soldier::update(float dt) {
    // unused
}

void Soldier::update(float dt, const sf::Vector2f& playerPos, const GameplayScene& scene) {

    // AI movement + attack decisions
    ai.update(*this, dt, playerPos, scene);

    // Attack timer
    if (isAttacking) {
        attackTimer -= dt;
        if (attackTimer <= 0.f) {
            isAttacking = false;
            activeHitbox.reset();
        }
    }
}

void Soldier::draw(sf::RenderWindow& window) {
    window.draw(sprite);

    if (isAttacking) {
        window.draw(attackShape);
    }
}

void Soldier::setPosition(const sf::Vector2f& pos) {
    sprite.setPosition(pos);
}

sf::Vector2f Soldier::getPosition() const {
    return sprite.getPosition();
}

void Soldier::startAttack(const sf::Vector2f& playerPos) {
    isAttacking = true;
    attackTimer = attackDuration;

    // Direction toward player
    sf::Vector2f dir = playerPos - getPosition();
    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (len == 0.f) dir = { 0.f, 1.f };
    else dir /= len;

    // Position slash
    attackShape.setPosition(getPosition() + dir * 60.f);

    float angleDeg = std::atan2(dir.y, dir.x) * 180.f / 3.14159f;
    attackShape.setRotation(sf::degrees(angleDeg));

    // Compute hitbox center
    sf::Vector2f localCenter{ 20.f, 6.f };
    sf::Vector2f worldCenter = attackShape.getTransform().transformPoint(localCenter);

    // Create persistent hitbox
    activeHitbox = HitboxCircle{
        worldCenter,
        50.f,      // enemy hitbox radius
        this,
        10.f,      // damage
        attackDuration,
        {}
    };
}

Hurtbox Soldier::getHurtbox() const {
    return { getPosition(), hurtRadius, this };
}
