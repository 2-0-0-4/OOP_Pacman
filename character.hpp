#ifndef CHARACTER_H
#define CHARACTER_H

#include <SFML/Graphics.hpp>
#include "Maze.hpp"
#include <iostream>

class Ghost;
// Base class: Character
class Character
{
protected:
    sf::Sprite player;                  // Represents the character shape
    sf::Vector2f position = {160, 176}; // Initial position
    int speed = 2;                      // Movement speed
    int direction = 0;                  // which direction its gonna go in

public:
    void draw(sf::RenderWindow &temp);
    virtual void respawn();
    bool wall_collision(int pos_x, int pos_y, int MAP_W, int MAP_H, const Maze &maze);
    void setPosition(int x, int y);
    sf::Vector2f getPosition();
    sf::FloatRect getGlobalBounds();
};

// Derived class: Pacman
class Pacman : public Character
{
private:
    int lives = 2; // Number of lives Pacman has (3 lives starting from 0)
    int score = 0;
    std::vector<std::vector<int>> heart_loco{{16, 337}, {40, 337}, {64, 337}};

public:
    void draw_data(sf::RenderWindow &temp);
    void increase_lives();
    int movement(Maze &maze, Health &temp_health, Poison &temp_poison, std::vector<Ghost> ghosts);
    int died();
    int get_direction();
    void reset();
    void draw_score(sf::RenderWindow &window);
};

class Ghost : public Character
{
private:
    sf::Vector2f position; // Position of the ghost = {160, 112}

    sf::Vector2f target;
    int direction;
    int ghost_id;
    float ghost_speed = 1;

public:
    Ghost(int id);
    void draw_ghost(sf::RenderWindow &i_window);
    void find_target(Pacman &i_pacman);
    int ghost_movement(Pacman &i_pacman, Maze &maze, int last_direction);
};

#endif
