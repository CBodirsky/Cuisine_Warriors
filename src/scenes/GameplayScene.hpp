#pragma once
#include "core/Scene.hpp"
#include <SFML/Graphics.hpp>
#include "entities/Player.hpp"
#include "entities/Soldier.hpp"
#include "entities/Officer.hpp"
#include "battle/CombatManager.hpp"
#include "ui/Hud.hpp"
#include "core/CollisionMask.hpp"

class GameplayScene : public Scene {
public:
    GameplayScene();
    Hud hud;
    CollisionMask collisionMask;

    void handleEvent(const sf::Event& event) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;
    bool isSolid(int x, int y) const;
    bool isSolidAt(const sf::Vector2f& pos, float radius = 8.f) const;

private:
    Player player;
    CombatManager combat;
    std::vector<Soldier> soldiers;
    std::vector<Officer> officers;

    bool playerHitBoxWasActive = false;
    std::vector<bool> soldierHitboxWasActive;
    std::vector<bool> officerHitboxWasActive;

    std::unique_ptr<sf::Sprite>TestMap1;
    sf::Clock clock;
    sf::View camera;
};
