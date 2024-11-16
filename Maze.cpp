#include "Maze.hpp"
#include <array>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <chrono>
#include "character.hpp"
#include "FoodPoison.hpp"
#include "Wall.cpp"

void Maze::draw_maze(int MAP_W, int MAP_H,sf::RenderWindow& i_window){
    Wall temp_wall;
    Poison temp_poison;
    Health temp_health;
        for (int i= 0; i< MAP_W; i++){ //rows
            for (int j=0; j<MAP_H; j++){ //cols
                if(maze_sketch[j][i] == "#"){
                    temp_wall.draw(i_window,i,j);
                }
                else if(maze_sketch[j][i] == "."){ //health
                    temp_health.add_to_array(i,j);
                    temp_health.draw(i_window,sf::Color::Green);
                }
                else if(maze_sketch[j][i] == "x"){ //poison
                    temp_poison.add_to_array(i,j);
                    temp_poison.draw(i_window,sf::Color::Red);
                }
			}
                
        }
}
