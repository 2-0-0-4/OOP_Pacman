#ifndef CHARACTER_H
#define CHARACTER_H

#include <SFML/Graphics.hpp>
#include "Maze.hpp"
#include <iostream>

struct Position
{
	int x;
	int y;

	
	bool operator==(const Position& i_position)
	{
		return this->x == i_position.x && this->y == i_position.y;
	}
};

// Base class: Character
class Character {
protected:
    sf::CircleShape player; // Represents the character shape
    sf::Vector2f position = {160, 176}; // Initial position
    int speed = 2; // Movement speed
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
    int score = 0;
    std::vector<std::vector<int>> heart_loco{{16, 337},{40,337},{64,337}};
    // Position position;
    // Position position = {160, 144};
public:
    //int speed = 2;
    void draw_data(sf::RenderWindow &temp);
    void increase_lives();
    int movement(Maze &maze, Health& temp_health, Poison& temp_poison);
    int died();
    // Position get_position();
    sf::Vector2f get_position();
    int get_direction();
    // Position set_position();
};


class Ghost : public Character{
    private:

    sf::Vector2f position ;  // Position of the ghost = {160, 112}
    
    sf::Vector2f target; 
    int direction;
    int ghost_id;
    float ghost_speed=1;
    public:
        
        Ghost(int id);
        void draw_ghost(sf::RenderWindow &i_window);
        void set_position(int pos_x, int pos_y);
        void find_pacman( Pacman& i_pacman);
        //void ghost_movement( Pacman& i_pacman,Maze &maze);
        int ghost_movement( Pacman& i_pacman,Maze &maze, int last_direction);
        // int random_d( Pacman& i_pacman,Maze &maze, int last_direction);
        

};
#endif 
