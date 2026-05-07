#include "SceneManager.hpp"
#include "Scene.hpp"

void SceneManager::setScene(std::unique_ptr<Scene> newScene) {
    currentScene = std::move(newScene);
}

Scene* SceneManager::getCurrentScene() {
    return currentScene.get();
}
