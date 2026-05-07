#include "AIController.hpp"
#include "entities/Entity.hpp"
#include <cmath>
#include "scenes/GameplayScene.hpp"

AIController::AIController() {
    // Initialize attack timer randomly
    attackTimer = attackIntervalMin +
        static_cast<float>(std::rand() % 100) / 100.f *
        (attackIntervalMax - attackIntervalMin);
}

static float vecLength(const sf::Vector2f& v) {
    return std::sqrt(v.x * v.x + v.y * v.y);
}

void AIController::setTarget(const sf::Vector2f& pos) {
    targetPos = pos;
    state = AIState::MoveToTarget;
}

void AIController::setState(AIState newState) {
    state = newState;
}

void AIController::update(Entity& self, float dt, const sf::Vector2f& playerPos, const GameplayScene& scene) {
    sf::Vector2f pos = self.getPosition();
    float desiredDistance = 115.f;

    attackCooldown -= dt;

    switch (state) {

    case AIState::Idle: {
        sf::Vector2f dir = playerPos - pos;
        float dist = vecLength(dir);

        // Aggro if player is close enough
        if (dist < 350.f) {
            state = AIState::ChasePlayer;
            break;
        }

        break;
    }


    //Moves to event points, routing, or other specific locations
    case AIState::MoveToTarget: {
        sf::Vector2f dir = targetPos - pos;
        float len = vecLength(dir);

        if (len > 5.f) {
            dir /= len;
            sf::Vector2f next = (pos + dir * speed * dt);
            if (!scene.isSolidAt(next, 12.f))
                self.setPosition(next);
        }
        else {
            state = AIState::Idle;
        }
        break;
    }

    //While within range, move to position for attacking the player.
    case AIState::ChasePlayer: {
        sf::Vector2f dir = playerPos - pos;
        float dist = vecLength(dir);

        // Disengage if player is far away
        if (dist > 300.f) {
            state = AIState::Idle;
            break;
        }

        if (dist > desiredDistance + 10.f) {
            dir /= dist;
            sf::Vector2f pos = self.getPosition();
            sf::Vector2f fullMove = dir * speed * dt;

            sf::Vector2f next = pos + fullMove;
            if (!scene.isSolidAt(next, 12.f)) {
                self.setPosition(next);
                return;
            }

            sf::Vector2f nextX = pos + sf::Vector2f(fullMove.x, 0.f);
            if (!scene.isSolidAt(nextX, 12.f)) {
                self.setPosition(nextX);
                return;
            }

            sf::Vector2f nextY = pos + sf::Vector2f(0.f, fullMove.y);
            if (!scene.isSolidAt(nextY, 12.f)) {
                self.setPosition(nextY);
                return;
            }
        }
        else if (dist < desiredDistance - 10.f) {
            dir /= dist;
            sf::Vector2f next = (pos - dir * speed * dt);
            if (!scene.isSolidAt(next, -10.f))
                self.setPosition(next);
        }
        else {
            state = AIState::CombatIdle;
            behaviorDuration = 0.5f + static_cast<float>(std::rand() % 200) / 100.f;
            behaviorTimer = 0.f;
            idleMode = std::rand() % 3;
        }

        break;
    }

    //While in range, shuffle left, right, or stay in position. Attacking at semi-random intervals.
    case AIState::CombatIdle: {
        behaviorTimer += dt;
        attackTimer -= dt;

        sf::Vector2f dir = playerPos - pos;
        float dist = vecLength(dir);
        if (dist > 0.f) dir /= dist;

        // If too far, resume chase
        if (dist > desiredDistance + 20.f) {
            state = AIState::ChasePlayer;
            break;
        }

        // If too close (after lunge), back up
        if (dist < desiredDistance - 10.f) {
            sf::Vector2f next = (pos - dir * speed * dt);
            if (!scene.isSolidAt(next, -10.f))
                self.setPosition(next);
            pos = self.getPosition();
            break;
        }

        // Attempt attack
        if (attackTimer <= 0.f) {
            state = AIState::Attack;

            self.startAttack(playerPos);

            attackCooldown = 0.3f; // lunge duration
            attackTimer = attackIntervalMin +
                static_cast<float>(std::rand() % 100) / 100.f *
                (attackIntervalMax - attackIntervalMin);

            break;
        }

        // Shuffle movement
        if (idleMode == 1) {
            sf::Vector2f tangent(-dir.y, dir.x);
            tangent /= vecLength(tangent);
            sf::Vector2f next = (pos + tangent * 40.f * dt);
            if (!scene.isSolidAt(next, 12.f))
                self.setPosition(next);
            pos = self.getPosition();
        }
        else if (idleMode == 2) {
            sf::Vector2f tangent(dir.y, -dir.x);
            tangent /= vecLength(tangent);
            sf::Vector2f next = (pos + tangent * 40.f * dt);
            if (!scene.isSolidAt(next, 12.f))
                self.setPosition(next);
            pos = self.getPosition();
        }

        // Switch shuffle behavior
        if (behaviorTimer >= behaviorDuration) {
            behaviorTimer = 0.f;
            behaviorDuration = 0.5f + static_cast<float>(std::rand() % 200) / 100.f;
            idleMode = std::rand() % 3;
        }

        break;
    }

    //Basic attack logic, lunging into range, then backing off to idle range
    case AIState::Attack: {
        sf::Vector2f pos = self.getPosition();

        sf::Vector2f dir = playerPos - pos;
        float dist = vecLength(dir);
        if (dist > 0.f) dir /= dist;

        float lungeSpeed = speed * 5.8f;
        self.setPosition(pos + dir * lungeSpeed * dt);

        //attack visual
        sf::Vector2f forward = dir;
        sf::Vector2f attackPos = self.getPosition() + forward * 25.f;

        attackCooldown -= dt;
        if (attackCooldown <= 0.f) {
            attackCooldown = 0.45f;
            
            state = AIState::CombatIdle;
        }

        break;
    }
    
    //Retreat logic, primarily for events, low morale, or defeat of nearby officers
    case AIState::Retreat: {
        sf::Vector2f dir = pos - playerPos;
        float len = vecLength(dir);

        if (len > 0.f) {
            dir /= len;
            self.setPosition(pos + dir * speed * dt);
        }
        break;
    }

    }
}
