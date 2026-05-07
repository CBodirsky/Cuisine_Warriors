#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

class Entity;

struct Hurtbox {
    sf::Vector2f center;
    float radius;
    const Entity* owner;
};

struct HitboxCircle {
    sf::Vector2f center;
    float radius;
    const Entity* owner;
    float damage;

    float lifetime = 0.f;
    std::vector<const Entity*> alreadyHit;
};

class CombatManager {
public:
    void registerHurtbox(const Hurtbox& hb);
    void registerHitboxCircle(const HitboxCircle& hb);

    void processHits(float dt);

    void clearHurtboxes();

private:
    std::vector<Hurtbox> hurtboxes;
    std::vector<HitboxCircle> hitboxcircles;
};
