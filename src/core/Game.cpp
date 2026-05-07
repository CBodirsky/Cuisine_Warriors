#include "core/Game.hpp"
#include "core/Scene.hpp"
#include "scenes/IntroScene.hpp"
#include "scenes/GameplayScene.hpp"



Game::Game()
    : window(sf::VideoMode({ 800, 600 }), "Cuisine Warriors") {
    sceneManager.setScene(std::make_unique<GameplayScene>());
}

void Game::run() {
    sf::Clock clock;

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        //sf::Event event;
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }


        if (sceneManager.getCurrentScene())
            sceneManager.getCurrentScene()->update(dt);

        window.clear();
        if (sceneManager.getCurrentScene())
            sceneManager.getCurrentScene()->draw(window);
        window.display();
    }
}
