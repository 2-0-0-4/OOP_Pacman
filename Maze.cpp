#include "Maze.hpp"
#include <array>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <chrono>
#include "character.hpp"
#include "food_poison.hpp"
#include "Wall.cpp"

void Maze::draw_maze(int MAP_W, int MAP_H,Health& temp_health,Poison& temp_poison,sf::RenderWindow& i_window, bool init){
    Wall temp_wall;
    
        for (int i= 0; i< MAP_W; i++){ //rows
            for (int j=0; j<MAP_H; j++){ //cols
                if(maze_sketch[j][i] == "#"){
                    temp_wall.draw(i_window,i,j);
                }
                else if(maze_sketch[j][i] == " " && init){ //health
                    temp_health.add_to_array(i,j);
                    // temp_health.draw(i_window,sf::Color::Green);
                }
                else if(maze_sketch[j][i] == "x" && init){ //poison
                    temp_poison.add_to_array(i,j);
                    // temp_poison.draw(i_window,sf::Color::Red);
                }
			}
                
        }
}