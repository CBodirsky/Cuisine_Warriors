#include "ResourceManager.hpp"
#include <stdexcept>
#include <iostream>
#include <SFML/Config.hpp>
using namespace std;

unordered_map<string, unique_ptr<sf::Texture>> ResourceManager::textures;

sf::Texture& ResourceManager::getTexture(const string& filename) {
    cout << "Trying to load: "
        << filesystem::absolute(filename)
        << "\n";
    // If texture already loaded, return it
    auto it = textures.find(filename);
    if (it != textures.end()) {
        return *it->second;
    }
    // Otherwise load it
    auto texture = std::make_unique<sf::Texture>();
    
    if (!texture->loadFromFile(filename)) {
        throw runtime_error("Failed to load texture: " + filename);
    }

    sf::Texture& ref = *texture;
    textures[filename] = move(texture);
    return ref;

    cout << "Trying to load: " << filesystem::absolute(filename) << "\n";

}

sf::Font& ResourceManager::getFont(const string& path) {
    static unordered_map<string, sf::Font> fonts;

    auto it = fonts.find(path);
    if (it != fonts.end())
        return it->second;

    sf::Font font;
    if (!font.openFromFile(path)) {
        throw runtime_error("Failed to load font: " + path);
    }

    fonts.emplace(path, move(font));
    return fonts[path];
}



