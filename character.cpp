//g++ character.cpp -I"C:\mingw_dev_lib\include" -L"C:\mingw_dev_lib\lib" -lsfml-graphics -lsfml-window -lsfml-system -o main.exe
#include <SFML/Graphics.hpp>
#include <iostream>
#include "character.hpp"
#include <cstring>
#include <cmath>
using namespace std;

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
        score += 50;
        temp_health.remove_from_array(position.x/16,position.y/16);
        // std::cout<<"here"<<std::endl;
    }
    
    if(temp_poison.find_in_array(position.x/16,position.y/16)){ //poison is there
        lives -= 1;
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

void Pacman::draw_data(sf::RenderWindow &temp){
    sf::Font font;
    font.loadFromFile("ka1.ttf");
    sf::Text text_score;
    sf::Text text;
    text_score.setFont(font);
    text.setFont(font);
    text_score.setFillColor(sf::Color::White);
    text.setFillColor(sf::Color::White);
    text_score.setString(std::to_string(score));
    text.setString("Score: ");
    text_score.setCharacterSize(15);
    text.setCharacterSize(15);
    text_score.setPosition(220,340); //245 340
    text.setPosition(150,340); //200 340
    temp.draw(text_score);
    temp.draw(text);
    for(int i = 0; i < lives; i++){
        sf::Texture texture;
        texture.loadFromFile("heart_1.png");
        sf::Sprite sprite;
        sprite.setTexture(texture); 
        sprite.setScale(sf::Vector2f(0.05f, .05f));
        sprite.setPosition(heart_loco[i][0], heart_loco[i][1]);
        temp.draw(sprite);
    }
}
// Position Pacman::set_position(){
// 	position={120,168};
// }
sf::Vector2f Pacman::get_position() {
    
    return position;
}


int Pacman::get_direction(){
    return direction;
}


//-------------------ghost code----------------------------------------------

Ghost::Ghost(int id) {
    this->ghost_id = id;
    switch (id) {
        case 0:  // Red Ghost (starting position)
            position = {180,112}; 
            break;
        case 1:  // Pink Ghost (starting position)
            position = {160, 38}; 
            break;
        case 2:  // Blue Ghost (starting position)
            position = {160, 304}; 
            break;
        
    }
}



void Ghost::draw_ghost(sf::RenderWindow &i_window){
    sf::CircleShape ghostTop(8.f);
    sf::RectangleShape ghostBottom(sf::Vector2f(16.f, 8.f));

        // colors and positions based on the ghost's ID
        switch (ghost_id) {
            case 0: // red Ghost
                ghostTop.setFillColor(sf::Color::Red);
                ghostBottom.setFillColor(sf::Color::Red);
                break;
            case 1: // pink Ghost
                ghostTop.setFillColor(sf::Color(255, 182, 255));
                ghostBottom.setFillColor(sf::Color(255, 182, 255));
                break;
            case 2: // cyan Ghost
                ghostTop.setFillColor(sf::Color(0, 255, 255));
                ghostBottom.setFillColor(sf::Color(0, 255, 255));
                break;
           
        }

        
        ghostTop.setPosition(position.x, position.y);  
        ghostBottom.setPosition(position.x, position.y + 8.f);  

        // Draw the ghost (both top and bottom parts)
        i_window.draw(ghostTop);
        i_window.draw(ghostBottom);
    }


// void Ghost::set_position(int pos_x, int pos_y){
//     position= {pos_x,pos_y};
// }
void Ghost::find_pacman( Pacman& i_pacman){
    target = i_pacman.get_position();
}








// void Ghost::ghost_movement(Pacman& i_pacman, Maze &maze) {
//     Character c;

//     bool walls[4];
//     walls[0] = c.wall_collision(position.x + ghost_speed, position.y, 21, 21, maze); // Right
//     walls[1] = c.wall_collision(position.x - ghost_speed, position.y, 21, 21, maze); // Left
//     walls[2] = c.wall_collision(position.x, position.y - ghost_speed, 21, 21, maze); // Up
//     walls[3] = c.wall_collision(position.x, position.y + ghost_speed, 21, 21, maze); // Down

    //____________________--------------------------------------------------------------------------------------

//     // // Handle wrap-around behavior if necessary (depending on the maze layout)
//     // if (position.x < 0) {
//     //     position.x = 16 * 21 - ghost_speed; // Wrap from left to right
//     // } else if (position.x >= 16 * 21) {
//     //     position.x = 0; // Wrap from right to left
//     // }

//     // if (position.y < 0) {
//     //     position.y = 16 * 21 - ghost_speed; // Wrap from top to bottom
//     // } else if (position.y >= 16 * 21) {
//     //     position.y = 0; // Wrap from bottom to top
//     // }

//     find_pacman(i_pacman);
//     cout << "Target Position: " << target.x << ", " << target.y << endl;

//     int dx = target.x - position.x;
//     int dy = target.y - position.y;

//     cout << "dx: " << dx << ", dy: " << dy << endl;

//     // Decide movement direction based on the shortest path
//     if (std::abs(dx) > std::abs(dy)) {
//         if (dx > 0) {
//             if (!walls[0]) { 
//                 direction = 0; // Move right
//             }
//         } else {
//             if (!walls[1]) { 
//                 direction = 1; // Move left
//             }
//         }
//     } else {
//         if (dy > 0) {
//             if (!walls[3]){
//                 direction = 3; // Move down
//             }
//         } else {
//             if (!walls[2]){
//             direction = 2; // Move up
//             }
//         }
//     }

//     // Debug: Show chosen direction
//     cout << "Direction chosen: " << direction << endl;

//     // Update position based on chosen direction
//     switch (direction) {
//         case 0: // Move right
            
//             position.x += ghost_speed;
//             cout << "Moving right: " << position.x << endl;
//             break;
//         case 2: // Move up
        
//             position.y -= ghost_speed;
//             cout << "Moving up: " << position.y << endl;
//             break;
//         case 1: // Move left
        
//             position.x -= ghost_speed;
//             cout << "Moving left: " << position.x << endl;
//             break;
//         case 3: // Move down
            
//             position.y += ghost_speed;
//             cout << "Moving down: " << position.y << endl;
//             break;
//     }

//     // Handle wrap-around
//      if (position.x < 0) {
//         position.x = 16 * 21 - ghost_speed;
//     }
//     if (position.x >= 16 * 21) {
//         position.x = 0;
//     }


//     // Debugging final position
//     cout << "Ghost final position: " << position.x << ", " << position.y << endl;
// }


//___________________________________________________________________________________________________________________

//there r a lott of debugging statements that i ade gpt put to figure out where the issue is so ignore that!

void Ghost::ghost_movement(Pacman& i_pacman, Maze& maze) {
    Character c;

    
    bool walls[4];
    walls[0] = c.wall_collision(position.x + ghost_speed, position.y, 21, 21, maze); // Right
    walls[1] = c.wall_collision(position.x - ghost_speed, position.y, 21, 21, maze); // Left
    walls[2] = c.wall_collision(position.x, position.y - ghost_speed, 21, 21, maze); // Up
    walls[3] = c.wall_collision(position.x, position.y + ghost_speed, 21, 21, maze); // Down

    
    find_pacman(i_pacman);
    cout << "Target Position: " << target.x << ", " << target.y << endl;

    // calculate movement direction towards the target
    int dx = target.x - position.x;
    int dy = target.y - position.y;
    cout << "dx: " << dx << ", dy: " << dy << endl;

    

    // route based on the ghost's ID
    switch (ghost_id) {
        case 0: {  // red ghost will chase Pacman directly
            target = i_pacman.get_position();  // Get Pacman's position as the target
            break;
        }
        case 1: {  // pink ghost (chase 4th cell in front of pacman)
            target = i_pacman.get_position();  //  pacman's position first

            // get pcmans current direction to see which way to go
            switch (i_pacman.get_direction()) {
                case 0:  // Right
                    target.x += 16 * 4;  // move 4 cells to the right of pman
                    break;
                case 1:  // up
                    target.y -= 16 * 4;  
                    break;
                case 2:  // left
                    target.x -= 16 * 4;  
                    break;
                case 3:  // down
                    target.y += 16 * 4;  
                    break;
            }
            break;
        }
        case 2: {  // blue ghost (chase 2nd cell in front of pacman)
            target = i_pacman.get_position();  

            
            switch (i_pacman.get_direction()) {
                case 0:  // Right
                    target.x += 16 * 2;  // move 2 cells to the right 
                    break;
                case 1:  // Up
                    target.y -= 16 * 2; 
                case 2:  // Left
                    target.x -= 16 * 2;  
                    break;
                case 3:  // Down
                    target.y += 16 * 2;  
                    break;
            }
            break;
        }
    }

    //  make a decision based on direction
    if (std::abs(dx) > std::abs(dy)) {
        if (dx > 0 && !walls[0]) { // move right
            direction = 0;
        } 
        else if (dx < 0 && !walls[1]) { // move left
            direction = 1;
        }
    } else {
        if (dy > 0 && !walls[3]) { // move down
            direction = 3;
        } 
        else if (dy < 0 && !walls[2]) { // move up
            direction = 2;
        }
    }

    

    

    // move the ghost based on the chosen direction
    bool moved = false; 

    switch (direction) {
        case 0: // move right
            if (!walls[0]) {
                position.x += ghost_speed;
                moved = true;
                cout << "Moving right: " << position.x << endl;
            }
            else {
                // if there's a wall, search for another direction
                for (int i = 0; i < 4; i++) {
                    if (!walls[i]) {
                        direction = i;
                        break;
                    }
                }
            }
            break;
        case 2: // move up
            if (!walls[2]) {
                position.y -= ghost_speed;
                moved = true;
                cout << "Moving up: " << position.y << endl;
            }
            else {
                
                for (int i = 0; i < 4; i++) {
                    if (!walls[i]) {
                        direction = i;
                        break;
                    }
                }
            }
            break;
        case 1: //  left
            if (!walls[1]) {
                position.x -= ghost_speed;
                moved = true;
                cout << "Moving left: " << position.x << endl;
            }
            else {
                
                for (int i = 0; i < 4; i++) {
                    if (!walls[i]) {
                        direction = i;
                        break;
                    }
                }
            }
            break;
        case 3: //  down
            if (!walls[3]) {
                position.y += ghost_speed;
                moved = true;
                cout << "Moving down: " << position.y << endl;
            }
            else {
                // if there's a wall, search for another direction
                for (int i = 0; i < 4; i++) {
                    if (!walls[i]) {
                        direction = i;
                        break;
                    }
                }
            }
            break;
    }

    
    if (!moved) {
        cout << "Hit a wall! Recalculating direction..." << endl;
    }

    // Handle wrap-around (ghost moves off one edge and appears on the opposite side)
    if (position.x < 0) {
        position.x = 16 * 21 - ghost_speed; // Wrap from left to right
    }
    if (position.x >= 16 * 21) {
        position.x = 0; // Wrap from right to left
    }
    
    // Debugging final position
    cout << "Ghost final position: " << position.x << ", " << position.y << endl;
}
