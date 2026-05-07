#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <memory>

class ResourceManager {
public:
    static sf::Texture& getTexture(const std::string& filename);
    static sf::Font& getFont(const std::string& path);

private:
    static std::unordered_map<std::string, std::unique_ptr<sf::Texture>> textures;
};
