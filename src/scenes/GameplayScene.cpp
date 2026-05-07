#include "GameplayScene.hpp"
#include "core/ResourceManager.hpp"
#include "core/CollisionMask.hpp"
#include <iostream>
#include <SFML/Graphics.hpp>


GameplayScene::GameplayScene() {

    // Load collision mask
    std::cerr << "Loading TestMap1..." << std::endl;
    if (!collisionMask.loadFromFile("assets/textures/TestMap1Masking.png")) {
        std::cerr << "Failed to load mask\n";
    }

    // Load map sprite
    TestMap1 = std::make_unique<sf::Sprite>(
        ResourceManager::getTexture("assets/textures/TestMap1.png")
    );
    TestMap1->setScale({ 1.5f, 1.5f });

    // Camera
    camera.setSize({ 1280.f, 720.f });
    camera.setCenter(player.getPosition());

    //Spawn officers
    Officer o;
    float x = 820.f;
    float y = 3500.f;
    o.setPosition({ x, y });
    officers.push_back(o);

    // Spawn soldiers
    for (int i = 0; i < 5; i++) {
        Soldier s;
        float x = 400.f + static_cast<float>(std::rand() % 400);
        float y = 500.f + static_cast<float>(std::rand() % 400);
        s.setPosition({ x, y });
        soldiers.push_back(s);
    }

    for (int i = 0; i < 4; i++) {
        Soldier s;
        float x = 400.f + static_cast<float>(std::rand() % 400);
        float y = 700.f + static_cast<float>(std::rand() % 400);
        s.setPosition({ x, y });
        soldiers.push_back(s);
    }

    for (int i = 0; i < 4; i++) {
        Soldier s;
        float x = 800.f + static_cast<float>(std::rand() % 400);
        float y = 3500.f + static_cast<float>(std::rand() % 400);
        s.setPosition({ x, y });
        soldiers.push_back(s);
    }

    for (int i = 0; i < 5; i++) {
        Soldier s;
        float x = 2000.f + static_cast<float>(std::rand() % 400);
        float y = 3700.f + static_cast<float>(std::rand() % 400);
        s.setPosition({ x, y });
        soldiers.push_back(s);
    }

    for (int i = 0; i < 5; i++) {
        Soldier s;
        float x = 2000.f + static_cast<float>(std::rand() % 400);
        float y = 3500.f + static_cast<float>(std::rand() % 400);
        s.setPosition({ x, y });
        soldiers.push_back(s);
    }

    for (int i = 0; i < 5; i++) {
        Soldier s;
        float x = 1000.f + static_cast<float>(std::rand() % 400);
        float y = 2500.f + static_cast<float>(std::rand() % 400);
        s.setPosition({ x, y });
        soldiers.push_back(s);
    }

    for (int i = 0; i < 25; i++) {
        Soldier s;
        float x = 800.f + static_cast<float>(std::rand() % 400);
        float y = 2500.f + static_cast<float>(std::rand() % 400);
        s.setPosition({ x, y });
        soldiers.push_back(s);
    }

    soldierHitboxWasActive.resize(soldiers.size(), false);
    officerHitboxWasActive.resize(soldiers.size(), false);

}

//bool GameplayScene::isSolid(int x, int y) const {
//    sf::Color c = collisionMask.getPixel(sf::Vector2u(
//        static_cast<unsigned>(x),
//        static_cast<unsigned>(y)
//    ));
//    return c == sf::Color::Black;
//}

bool GameplayScene::isSolidAt(const sf::Vector2f& pos, float radius) const {

    static const sf::Vector2f offsets[] = {
        { radius, 0.f },
        { -radius, 0.f },
        { 0.f, radius },
        { 0.f, -radius }
    };

    for (auto& o : offsets) {
        float scale = 1.5f;
        sf::Vector2f p = (pos + o) / scale;

        int x = static_cast<int>(p.x);
        int y = static_cast<int>(p.y);

        if (x < 0 || y < 0 ||
            x >= static_cast<int>(collisionMask.getSize().x) ||
            y >= static_cast<int>(collisionMask.getSize().y))
            return true;

        if (collisionMask.isSolid(x, y))
            return true;
    }

    return false;
}


bool GameplayScene::isSolid(int x, int y) const {
    return collisionMask.isSolid(x, y);
}


void GameplayScene::handleEvent(const sf::Event& event) {
    // No input handled here yet
}

void GameplayScene::update(float dt) {

    // Camera smoothing
    sf::Vector2f camPos = camera.getCenter();
    sf::Vector2f target = player.getPosition();
    camera.setCenter(camPos + (target - camPos) * 2.5f * dt);

    // Update entities
    sf::Vector2f playerPos = player.getPosition();
    player.update(dt, *this);
    for (auto& s : soldiers)
        s.update(dt, playerPos, *this);
    for (auto& o : officers)
        o.update(dt, playerPos, *this);

    soldiers.erase(
        std::remove_if(soldiers.begin(), soldiers.end(),
            [](const Soldier& s) { return s.isDead; }),
        soldiers.end()
    );

    officers.erase(
        std::remove_if(officers.begin(), officers.end(),
            [](const Officer& o) { return o.isDead; }),
        officers.end()
    );

    // Keep hitbox tracking vector in sync
    soldierHitboxWasActive.resize(soldiers.size(), false);
    officerHitboxWasActive.resize(officers.size(), false);


    if (player.isDead) {
        // TODO: Player game over situation
        std::cout << "Player died!\n";
    }

    // --- COMBAT ---

    combat.clearHurtboxes();

    combat.registerHurtbox(player.getHurtbox());

    for (auto& s : soldiers) {
        combat.registerHurtbox(s.getHurtbox());
    }
    for (auto& o : officers) {
        combat.registerHurtbox(o.getHurtbox());
    }

    // Register hitboxes, and you don't get to hit every frame!
    bool pActive = player.getActiveHitbox().has_value();
    if (pActive && !playerHitBoxWasActive)
        combat.registerHitboxCircle(player.getActiveHitbox().value());
    playerHitBoxWasActive = pActive;

    for (size_t i = 0; i < soldiers.size(); i++) {
        bool active = soldiers[i].getActiveHitbox().has_value();
        if (active && !soldierHitboxWasActive[i])
            combat.registerHitboxCircle(soldiers[i].getActiveHitbox().value());
        soldierHitboxWasActive[i] = active;
    }

    for (size_t i = 0; i < officers.size(); i++) {
        bool active = officers[i].getActiveHitbox().has_value();
        if (active && !officerHitboxWasActive[i])
            combat.registerHitboxCircle(officers[i].getActiveHitbox().value());
        officerHitboxWasActive[i] = active;
    }

    combat.processHits(dt);
    hud.update(player);
}

void GameplayScene::draw(sf::RenderWindow& window) {

    window.setView(camera);

    window.draw(*TestMap1);

    for (auto& o : officers)
        o.draw(window);

    for (auto& s : soldiers)
        s.draw(window);

    player.draw(window);

    window.setView(window.getDefaultView());
    hud.draw(window);
}
