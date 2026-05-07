#pragma once
#include <SFML/Graphics.hpp>

class CollisionMask {
public:
    bool loadFromFile(const std::string& path) {
        return mask.loadFromFile(path);
    }

    bool isSolid(int x, int y) const {
        if (x < 0 || y < 0 ||
            x >= static_cast<int>(mask.getSize().x) ||
            y >= static_cast<int>(mask.getSize().y))
            return true;

        return mask.getPixel({ (unsigned)x, (unsigned)y }) == sf::Color::Black;
    }

    sf::Vector2u getSize() const {
        return mask.getSize();
    }

private:
    sf::Image mask;
};
