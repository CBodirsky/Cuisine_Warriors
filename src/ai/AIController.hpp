#pragma once
#include <SFML/System/Vector2.hpp>

class GameplayScene;

class Entity;

enum class AIState {
    Idle,
    MoveToTarget,
    ChasePlayer,
    CombatIdle,
    Attack,
    Retreat
};

class AIController {
public:
    AIController();

    void update(Entity& self, float dt, const sf::Vector2f& playerPos, const GameplayScene& scene);

    void setTarget(const sf::Vector2f& pos);
    void setState(AIState newState);

private:
    AIState state = AIState::Idle;
    sf::Vector2f targetPos;
    float speed = 100.f;
    float attackTimer = 0.f;
    float attackIntervalMin = 3.f;
    float attackIntervalMax = 10.5f;
    float attackCooldown = 0.f;
    float behaviorTimer = 0.f;
    float behaviorDuration = 0.f;
    int idleMode = 0; // 0 = idle, 1 = left, 2 = right



};

