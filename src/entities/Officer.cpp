#include "Officer.hpp"
#include "core/ResourceManager.hpp"
#include <cmath>
#include "scenes/GameplayScene.hpp"

Officer::Officer()
    : sprite(ResourceManager::getTexture("assets/textures/Officer.png"))
{
    sprite.setOrigin(sprite.getLocalBounds().getCenter());

    attackShape.setSize({ 40.f, 12.f });
    attackShape.setOrigin({ 20.f, 6.f }); // true center
    attackShape.setFillColor(sf::Color(255, 0, 0, 120));

    ai.setState(AIState::ChasePlayer);
}

Faction Officer::getFaction() const { return Faction::Enemy; }


void Officer::update(float dt) {
    // unused
}

void Officer::update(float dt, const sf::Vector2f& playerPos, const GameplayScene& scene) {

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

void Officer::draw(sf::RenderWindow& window) {
    window.draw(sprite);

    if (isAttacking) {
        window.draw(attackShape);
    }
}

void Officer::setPosition(const sf::Vector2f& pos) {
    sprite.setPosition(pos);
}

sf::Vector2f Officer::getPosition() const {
    return sprite.getPosition();
}

void Officer::startAttack(const sf::Vector2f& playerPos) {
    isAttacking = true;
    attackTimer = attackDuration;

    // Direction toward player
    sf::Vector2f dir = playerPos - getPosition();
    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (len == 0.f) dir = { 0.f, 1.f };
    else dir /= len;

    // Position slash
    attackShape.setPosition(getPosition() + dir * 70.f);

    float angleDeg = std::atan2(dir.y, dir.x) * 180.f / 3.14159f;
    attackShape.setRotation(sf::degrees(angleDeg));

    // Compute hitbox center
    sf::Vector2f localCenter{ 25.f, 8.f };
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

Hurtbox Officer::getHurtbox() const {
    return { getPosition(), hurtRadius, this };
}
