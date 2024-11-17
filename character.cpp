//g++ character.cpp -I"C:\mingw_dev_lib\include" -L"C:\mingw_dev_lib\lib" -lsfml-graphics -lsfml-window -lsfml-system -o main.exe
#include <SFML/Graphics.hpp>
#include <iostream>
#include "character.hpp"
#include <cmath>


void Character::draw(sf::RenderWindow &temp){
    
    sf::CircleShape player(8.f);
    player.setFillColor(sf::Color::Yellow);
    player.setPosition(position.x, position.y);
    temp.draw(player);

}

void Character::respawn(){
        //position = {200,150};
}

bool Character::wall_collision(int pos_x, int pos_y, int MAP_H, int MAP_W, const Maze& maze){
        bool output = false;

        //getting the exact position of pacman by dviding current pos coordinates by cell size
        float cell_x = pos_x / static_cast<float>(16);
        float cell_y = pos_y / static_cast<float>(16);

        // can intersect 4 cells at most.
        // checking all 4 cells around it to see if the 2 are interacting.
        for ( int i = 0; i < 4; i++)
        {
            int x = 0;
            int y = 0;

            switch (i)
            {
                case 0: //top left cell (flooring it bcs we get the upper most/smalllest val)
                {
                    x = static_cast<int>(floor(cell_x));
                    y = static_cast<int>(floor(cell_y));
                    break;
                }
                case 1: //top right cell (ceil the x and floor the y bcs right and top)
                {
                    x = static_cast<int>(ceil(cell_x));
                    y = static_cast<int>(floor(cell_y));
                    break;
                }
                case 2: //bottom left cell
                {
                    x = static_cast<int>(floor(cell_x));
                    y = static_cast<int>(ceil(cell_y));
                    break;
                }
                case 3: //bottom right cell
                {
                    x = static_cast<int>(ceil(cell_x));
                    y = static_cast<int>(ceil(cell_y));
                    break;
                }
            }
            // if we're somewhere on the map
            if (0 <= x && 0 <= y && MAP_H > y && MAP_W > x)
                {
                    //if a wall # is on the same coordinates as any of our 4 cases
                    if (maze.maze_sketch[y][x]=="#")
                    {  //there will be a collision, hence wont be able to move
                        output = true;
                    }
                }
        
            
    }
    return output;
        
}
void Pacman::increase_lives(){
    if (lives < 3){
        lives += 1;
    }
}

void Pacman::movement(Maze &maze, Health &temp_health, Poison &temp_poison){
    sf::Vector2f index;
    index.x = position.x / 16;  // Assuming the cell size is 16x16
    index.y = position.y / 16;
    
    // the 4 possible wallls when pacman is moving
    bool walls[4];
    walls[0] = wall_collision(speed + position.x, position.y, 21,21, maze); //right
    walls[1] = wall_collision(position.x - speed, position.y, 21,21, maze); // left
    walls[2] = wall_collision(position.x, position.y - speed, 21,21, maze); // up
    walls[3] = wall_collision( position.x, speed + position.y, 21,21, maze); //down

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        if (walls[0]==false) //moving right and theres no wall, then keep moving towards the right
        {
            direction = 0;
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        if (walls[1]==false){
            direction = 1;
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        if (walls[2]==false){
            direction = 2;
        }
    }

    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        if (walls[3]==false){
            direction = 3;
        }
    }

    if (walls[direction]== false) // if theres no wall in that direction
    {
        switch (direction)
        {
            case 0:  //moves to the right
            {
                position.x += speed;
                break;
            }
            case 1:  //moves to the left
            {
                position.x -= speed;
                break;
            }
            case 2://moves up
            { 
                position.y -= speed;
                break;
            }
            case 3: //moves down
            {
                position.y += speed;
                break;
            }
        }
    } //maze.maze_sketch[position.y/16][position.x/16] == "."
    if(temp_health.find_in_array(position.x/16,position.y/16)){ //health is there
        // maze.maze_sketch[position.y/16][position.x/16] = " ";
        temp_health.remove_from_array(position.x/16,position.y/16);
        // std::cout<<"here"<<std::endl;
    }
    
    if(temp_poison.find_in_array(position.x/16,position.y/16)){ //poison is there
        temp_poison.remove_from_array(position.x/16,position.y/16);
    }
    // 16 is our cells size and 21 is the width of our maze.
    if (-16> position.x)
    {
        position.x = 16*21 - speed;  //wraps it around and makess it come out from the right
    }
    else if (16*21 <= position.x)
    {
        position.x=speed - 16;  //wraps it around and makess it come out from the left
    }

        }

void Pacman::died(){
    if (lives == 0){
        std::cout << "game over!" << std::endl;
    }
    else{
        lives -= 1;
        respawn();
    }
}   

void Pacman::draw_lives(sf::RenderWindow &temp){
    
}
