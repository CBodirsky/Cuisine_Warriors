#include "Player.hpp"
#include "core/ResourceManager.hpp"
#include "core/InputManager.hpp"
#include "scenes/GameplayScene.hpp"
#include <cmath>
#include <iostream>

Player::Player()
    : sprite(ResourceManager::getTexture("assets/textures/BurgerHero.png"))
{
    sprite.setOrigin(sprite.getLocalBounds().getCenter());
    sprite.setPosition({ 300.f, 4000.f });

    attackShape.setSize({ 20.f, 100.f });
    attackShape.setOrigin({ 10.f, 50.f }); // true center
    attackShape.setFillColor(sf::Color(255, 255, 255, 120));
}

void Player::update(float dt) {
    // unused
}

Faction Player::getFaction() const { return Faction::Player; }

void Player::update(float dt, const GameplayScene& scene) {

    if (InputManager::isAttackPressed() && !isAttacking)
        startAttack();

    if (isAttacking) {
        attackTimer -= dt;
        if (attackTimer <= 0.f) {
            isAttacking = false;
            activeHitbox.reset();
        }
    }

    sf::Vector2f move = InputManager::getMovementInput();
    if (move.x != 0.f || move.y != 0.f)
        facingDir = move;

    sf::Vector2f pos = sprite.getPosition();
    sf::Vector2f fullMove = move * speed * dt;

    sf::Vector2f next = pos + fullMove;
    if (!scene.isSolidAt(next, 12.f)) {
        sprite.setPosition(next);
        return;
    }

    sf::Vector2f nextX = pos + sf::Vector2f(fullMove.x, 0.f);
    if (!scene.isSolidAt(nextX, 12.f)) {
        sprite.setPosition(nextX);
        return;
    }

    sf::Vector2f nextY = pos + sf::Vector2f(0.f, fullMove.y);
    if (!scene.isSolidAt(nextY, 12.f)) {
        sprite.setPosition(nextY);
        return;
    }
}

void Player::applyMovement(const sf::Vector2f& delta) {
    sprite.move(delta);
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(sprite);

    if (isAttacking) {
        window.draw(attackShape);
    }
}

void Player::setPosition(const sf::Vector2f& pos) {
    sprite.setPosition(pos);
}

sf::Vector2f Player::getPosition() const {
    return sprite.getPosition();
}

void Player::startAttack() {
    isAttacking = true;
    attackTimer = attackDuration;

    sf::Vector2f dir = facingDir;
    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (len == 0.f) dir = { 0.f, -1.f };
    else dir /= len;

    attackShape.setPosition(sprite.getPosition() + dir * 100.f);

    float angleDeg = std::atan2(dir.y, dir.x) * 180.f / 3.14159f;
    attackShape.setRotation(sf::degrees(angleDeg + 90.f));

    sf::Vector2f localCenter{ 10.f, 50.f };
    sf::Vector2f worldCenter = attackShape.getTransform().transformPoint(localCenter);

    activeHitbox = HitboxCircle{
        worldCenter,
        30.f,
        this,
        25.f,
        attackDuration,
        {}
    };
}

void Player::takeDamage(float amount) {
    health -= amount;
    if (health <= 0.f) {
        health = 0.f;
        isDead = true;
    }
    std::cout << "PLAYER took " << amount << " damage. Health = " << health << "\n";
}

Hurtbox Player::getHurtbox() const {
    return { getPosition(), hurtRadius, this };
}
