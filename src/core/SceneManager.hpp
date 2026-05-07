#pragma once
#include <memory>

class Scene;

class SceneManager {
public:
    void setScene(std::unique_ptr<Scene> newScene);
    Scene* getCurrentScene();

private:
    std::unique_ptr<Scene> currentScene;
};
