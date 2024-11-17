#ifndef CHARACTER_H
#define CHARACTER_H

#include <SFML/Graphics.hpp>
#include "Maze.hpp"
#include <iostream>


// Base class: Character
class Character {
protected:
    sf::CircleShape player; // Represents the character shape
    sf::Vector2f position = {160, 128}; // Initial position
    int speed = 1; // Movement speed
    int direction=0; //which direction its gonna go in

public:
    void draw(sf::RenderWindow &temp);
    virtual void respawn();
    bool wall_collision(int pos_x, int pos_y, int MAP_W, int MAP_H, const Maze& maze);
};

// Derived class: Pacman
class Pacman : public Character {
private:
    int lives = 3; // Number of lives Pacman has

public:
    void draw_lives(sf::RenderWindow &temp);
    void increase_lives();
    void movement(Maze &maze, Health& temp_health, Poison& temp_poison);
    void died();
};

#endif 
