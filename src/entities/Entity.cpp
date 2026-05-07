#include "Entity.hpp"
#include <iostream>


void Entity::takeDamage(float amount) {
    health -= amount;
    if (health <= 0.f) {
        health = 0.f;
        isDead = true;
    }
    std::cout << "Entity " << this << " took " << amount << " damage. Health = " << health << "\n";
}
