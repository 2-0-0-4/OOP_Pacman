//g++ character.cpp -I"C:\mingw_dev_lib\include" -L"C:\mingw_dev_lib\lib" -lsfml-graphics -lsfml-window -lsfml-system -o main.exe
#include <SFML/Graphics.hpp>
#include <iostream>
#include "character.hpp"
#include <cmath>

// class Character{
//     protected:
//         sf::RectangleShape player;
//         sf::Vector2f position = {160,128};
//         int speed = 16;

//     public:
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
// } ;


// class Pacman : public Character{
//     private:
//         int lives = 3;
//     public:
//         // void increase_speed(){
//         //     speed = 3;
//         // }
        void Pacman::increase_lives(){
            if (lives < 3){
                lives += 1;
            }
        }

        void Pacman::movement(const Maze &maze){

            
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

        //     sf::Vector2f index;
        //     index.x = position.x/16;
        //     index.y = position.y/16; 
        //     if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        //         position.x -= speed; // Move left
        //         std::cout << "left " << position.x << ", " << position.y <<  std::endl;
        //         std::cout << "left " << index.x << ", " << index.y <<  std::endl;
                
        //     }
        //     if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        //         position.x += speed; // Move right
        //         player.move(speed,0.f);
        //         std::cout << "right " << position.x << ", " << position.y <<  std::endl;
        //         std::cout << "left " << index.x << ", " << index.y <<  std::endl;
        //     }
        //     if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        //         position.y -= speed; // up
        //         std::cout << "up " << position.x << ", " << position.y <<  std::endl;
        //         std::cout << "left " << index.x << ", " << index.y <<  std::endl;
        //     }
        //     if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        //         position.y += speed; // down
        //         std::cout << "down " << position.x << ", " << position.y <<  std::endl;
        //         std::cout << "left " << index.x << ", " << index.y <<  std::endl;
        //     }
            
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






// int main(){
//     sf::RenderWindow window(sf::VideoMode(400, 300), "Hit or Miss Ball");
//     Character man;
//     window.setFramerateLimit(60);
    
//     while (window.isOpen()) {
//         sf::Event event;
//         while (window.pollEvent(event)) {
//             if (event.type == sf::Event::Closed)
//             window.close();
//         }
//         man.draw(window);
//         man.movement();
//         man.respawn();
//         // Display the contents
//         window.display();
//         window.clear();
//     }
// }
