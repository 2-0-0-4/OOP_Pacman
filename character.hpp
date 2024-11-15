#ifndef CHARACTER_H
#define CHARACTER_H

#include <SFML/Graphics.hpp>
#include <iostream>

// Base class: Character
class Character {
protected:
    sf::RectangleShape player; // Represents the character shape
    sf::Vector2f position = {160, 128}; // Initial position
    int speed = 16; // Movement speed

public:
    void draw(sf::RenderWindow &temp);
    virtual void respawn();
};

// Derived class: Pacman
class Pacman : public Character {

private:
    int lives = 3; // Number of lives Pacman has

public:
    void increase_lives();
    void movement();
    void died();
};

#endif 
