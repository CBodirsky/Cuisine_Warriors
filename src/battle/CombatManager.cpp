#include "battle/CombatManager.hpp"
#include "entities/Entity.hpp"
#include <algorithm>
#include <cmath>

void CombatManager::registerHurtbox(const Hurtbox& hb) {
    hurtboxes.push_back(hb);
}

void CombatManager::clearHurtboxes() {
    hurtboxes.clear();
}

void CombatManager::registerHitboxCircle(const HitboxCircle& hb) {
    hitboxcircles.push_back(hb);
}

void CombatManager::processHits(float dt) {


    for (auto& hb : hitboxcircles) {

        for (auto& hurt : hurtboxes) {

            if (hb.owner == hurt.owner)
                continue;

            // Don't hit allies
            if (hb.owner->getFaction() == hurt.owner->getFaction())
                continue;

            if (std::find(hb.alreadyHit.begin(),
                hb.alreadyHit.end(),
                hurt.owner) != hb.alreadyHit.end())
                continue;

            float dx = hurt.center.x - hb.center.x;
            float dy = hurt.center.y - hb.center.y;
            float dist2 = dx * dx + dy * dy;
            float radSum = hb.radius + hurt.radius;

            if (dist2 <= radSum * radSum) {
                const_cast<Entity*>(hurt.owner)->takeDamage(hb.damage);
                hb.alreadyHit.push_back(hurt.owner);
            }
        }

        hb.lifetime -= dt;
    }

    hitboxcircles.erase(
        std::remove_if(hitboxcircles.begin(), hitboxcircles.end(),
            [](const HitboxCircle& hb) { return hb.lifetime <= 0.f; }),
        hitboxcircles.end()
    );
}
