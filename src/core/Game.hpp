#pragma once
#include "core/SceneManager.hpp"
#include <SFML/Graphics.hpp>

class Game {
public:
	Game();
	void run();

private:
	sf::RenderWindow window;
	SceneManager sceneManager;
};