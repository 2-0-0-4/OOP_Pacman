// g++ character.cpp -I"C:\mingw_dev_lib\include" -L"C:\mingw_dev_lib\lib" -lsfml-graphics -lsfml-window -lsfml-system -o main.exe
#include <SFML/Graphics.hpp>
#include <iostream>
#include "character.hpp"
#include <cstring>
#include <cmath>
using namespace std;

void Character::draw(sf::RenderWindow &temp)
{

    // sf::CircleShape player(8.f);
    // player.setFillColor(sf::Color::Yellow);
    // player.setPosition(position.x, position.y);
    sf::Texture texture;
    texture.loadFromFile("scroll_sprite.png");
    player.setTexture(texture);
    player.setScale(sf::Vector2f(0.035f, 0.035f));
    player.setPosition(position.x, position.y);
    temp.draw(player);
}

Vector2f Character::getPosition()
{
    return position;
}

void Character::respawn()
{
    position = {160, 176};
}

sf::FloatRect Character::getGlobalBounds()
{
    return player.getGlobalBounds();
}

void Character::setPosition(int x, int y)
{
    position.x = x;
    position.y = y;
}

bool Character::wall_collision(int pos_x, int pos_y, int MAP_H, int MAP_W, const Maze &maze)
{
    bool output = false;

    // getting the exact position of pacman by dviding current pos coordinates by cell size
    float cell_x = pos_x / static_cast<float>(16);
    float cell_y = pos_y / static_cast<float>(16);

    // can intersect 4 cells at most.
    // checking all 4 cells around it to see if the 2 are interacting.
    for (int i = 0; i < 4; i++)
    {
        int x = 0;
        int y = 0;

        switch (i)
        {
        case 0: // top left cell (flooring it bcs we get the upper most/smalllest val)
        {
            x = static_cast<int>(floor(cell_x));
            y = static_cast<int>(floor(cell_y));
            break;
        }
        case 1: // top right cell (ceil the x and floor the y bcs right and top)
        {
            x = static_cast<int>(ceil(cell_x));
            y = static_cast<int>(floor(cell_y));
            break;
        }
        case 2: // bottom left cell
        {
            x = static_cast<int>(floor(cell_x));
            y = static_cast<int>(ceil(cell_y));
            break;
        }
        case 3: // bottom right cell
        {
            x = static_cast<int>(ceil(cell_x));
            y = static_cast<int>(ceil(cell_y));
            break;
        }
        }
        // if we're somewhere on the map
        if (0 <= x && 0 <= y && MAP_H > y && MAP_W > x)
        {
            // cout << "y: " << y << " x: " << x << endl;
            // if a wall # is on the same coordinates as any of our 4 cases
            if (maze.maze_sketch[y][x] == "#")
            { // there will be a collision, hence wont be able to move
                output = true;
            }
        }
    }
    return output;
}
void Pacman::increase_lives()
{
    if (lives < 3)
    {
        lives += 1;
    }
}

int Pacman::movement(Maze &maze, Health &temp_health, Poison &temp_poison, std::vector<Ghost> ghosts)
{
    int status = 0;
    // the 4 possible wallls when pacman is moving
    bool walls[4];
    walls[0] = wall_collision(speed + position.x, position.y, 21, 21, maze); // right
    walls[1] = wall_collision(position.x - speed, position.y, 21, 21, maze); // left
    walls[2] = wall_collision(position.x, position.y - speed, 21, 21, maze); // up
    walls[3] = wall_collision(position.x, speed + position.y, 21, 21, maze); // down
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        if (walls[0] == false) // moving right and theres no wall, then keep moving towards the right
        {
            direction = 0;
            // bool check = true;
        }
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        if (walls[1] == false)
        {
            direction = 1;
            // bool check = true;
        }
    }

    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        if (walls[2] == false)
        {
            direction = 2;
            // bool check = true;
        }
    }

    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        if (walls[3] == false)
        {
            direction = 3;
            // bool check = true;
        }
    }

    if (walls[direction] == false) // if theres no wall in that direction
    {
        switch (direction)
        {
        case 0: // moves to the right
        {
            position.x += speed;
            break;
        }
        case 1: // moves to the left
        {
            position.x -= speed;
            break;
        }
        case 2: // moves up
        {
            position.y -= speed;
            break;
        }
        case 3: // moves down
        {
            position.y += speed;
            break;
        }
        }
    } // maze.maze_sketch[position.y/16][position.x/16] == "."

    if (-16 > position.x)
    {
        position.x = 16 * 21 - speed; // wraps it around and makess it come out from the right
    }
    else if (16 * 21 <= position.x)
    {
        position.x = speed - 16; // wraps it around and makess it come out from the left
    }

    if (temp_health.find_in_array(position.x / 16, position.y / 16))
    { // health is there
        // maze.maze_sketch[position.y/16][position.x/16] = " ";
        score += 50;
        temp_health.remove_from_array(position.x / 16, position.y / 16);
        if (temp_health.is_empty() == true)
        {
            status = 2;
        }
    }
    if (temp_poison.find_in_array(position.x / 16, position.y / 16))
    { // poison is there
        temp_poison.remove_from_array(position.x / 16, position.y / 16);
        score -= 20;
        if (score < 0)
            status = 1;
    }
    if (ghosts[0].getGlobalBounds().intersects(player.getGlobalBounds()) ||
        ghosts[1].getGlobalBounds().intersects(player.getGlobalBounds()) ||
        ghosts[2].getGlobalBounds().intersects(player.getGlobalBounds()))
    {
        status = died();
    }
    // 16 is our cells size and 21 is the width of our maze.
    return status;
}

int Pacman::died()
{
    cout << lives;
    if (lives == 0)
    {
        // std::cout << "game over!" << std::endl;
        return 1;
    }
    else
    {
        lives -= 1;
        cout << "lives - 1: " << lives << endl;
        respawn();
        return 0;
    }
}

void Pacman::draw_data(sf::RenderWindow &temp)
{
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
    text_score.setPosition(220, 340); // 245 340
    text.setPosition(150, 340);       // 200 340
    temp.draw(text_score);
    temp.draw(text);
    for (int i = 0; i < lives + 1; i++)
    {
        sf::Texture texture;
        texture.loadFromFile("heart_1.png");
        sf::Sprite sprite;
        sprite.setTexture(texture);
        sprite.setScale(sf::Vector2f(0.05f, .05f));
        sprite.setPosition(heart_loco[i][0], heart_loco[i][1]);
        temp.draw(sprite);
    }
}

int Pacman::get_direction()
{
    return direction;
}

void Pacman::reset()
{
    score = 0;
    position.x = 160;
    position.y = 176;
    lives = 2;
}

//-------------------ghost code----------------------------------------------

Ghost::Ghost(int id)
{
    this->ghost_id = id;
    switch (id)
    {
    case 0: // Red Ghost (starting position)
        position = {180, 112};
        break;
    case 1: // Pink Ghost (starting position)
        position = {160, 38};
        break;
    case 2: // Blue Ghost (starting position)
        position = {160, 304};
        break;
    }
}

void Ghost::draw_ghost(sf::RenderWindow &i_window)
{
    sf::Texture texture;
    // sf::Sprite player;
    // colors and positions based on the ghost's ID
    switch (ghost_id)
    {
    case 0: // red Ghost Mikaal
        // ghostTop.setFillColor(sf::Color::Red);
        // ghostBottom.setFillColor(sf::Color::Red);
        texture.loadFromFile("mikaal_sprite.png");
        player.setTexture(texture);
        player.setScale(sf::Vector2f(0.03f, 0.03f));
        break;
    case 1: // pink Ghost
        // ghostTop.setFillColor(sf::Color(255, 182, 255));
        // ghostBottom.setFillColor(sf::Color(255, 182, 255));
        texture.loadFromFile("adeenah_sprite.png");
        player.setTexture(texture);
        player.setScale(sf::Vector2f(0.028f, 0.025f));
        break;
    case 2: // cyan Ghost
        // ghostTop.setFillColor(sf::Color(0, 255, 255));
        // ghostBottom.setFillColor(sf::Color(0, 255, 255));
        texture.loadFromFile("afeera_sprite.png");
        player.setTexture(texture);
        player.setScale(sf::Vector2f(0.03f, 0.03f));
        break;
    }

    player.setPosition(position.x, position.y);

    // ghostTop.setPosition(position.x, position.y);
    // ghostBottom.setPosition(position.x, position.y + 8.f);

    // Draw the ghost (both top and bottom parts)
    // i_window.draw(ghostTop);
    // i_window.draw(ghostBottom);
    i_window.draw(player);
}

// void Ghost::set_position(int pos_x, int pos_y){
//     position= {pos_x,pos_y};
// // }
// void Ghost::find_pacman(Pacman &i_pacman) //not needed
// {
//     target = i_pacman.getPosition();
// }

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

// there r a lott of debugging statements that i ade gpt put to figure out where the issue is so ignore that!
int Ghost::ghost_movement(Pacman &i_pacman, Maze &maze, int last_direction)
{

    bool walls[4];
    walls[0] = this->wall_collision(position.x + ghost_speed, position.y, 21, 21, maze); // Right
    walls[1] = this->wall_collision(position.x - ghost_speed, position.y, 21, 21, maze); // Left
    walls[2] = this->wall_collision(position.x, position.y - ghost_speed, 21, 21, maze); // Up
    walls[3] = this->wall_collision(position.x, position.y + ghost_speed, 21, 21, maze); // Down

    // bool walls_pacman[4];
    // walls_pacman[0] = i_pacman.wall_collision(position.x + i_pacman.speed, position.y, 21, 21, maze); // Right
    // walls_pacman[1] = i_pacman.wall_collision(position.x - i_pacman.speed, position.y, 21, 21, maze); // Left
    // walls_pacman[2] = i_pacman.wall_collision(position.x, position.y - i_pacman.speed, 21, 21, maze); // Up
    // walls_pacman[3] = i_pacman.wall_collision(position.x, position.y + i_pacman.speed, 21, 21, maze); // Down

    // cout << "W" << endl;
    // cout << "right: " << position.x + ghost_speed << " " << position.y << "   "  << endl;
    // cout << "left : " << position.x - ghost_speed << " " << position.y << "   "  << endl;
    // cout << "up   : " << position.x << " " << position.y - ghost_speed << "   "  << endl;
    // cout << "down : " << position.x << " " << position.y + ghost_speed << "   "  << endl;

    // find_pacman(i_pacman);
    // cout << "Target Position: " << target.x << ", " << target.y << endl;

    // calculate movement direction towards the target
    int opp_direction;
    switch (last_direction)
    {
    case 0:
        opp_direction = 1;
        break;
    case 1:
        opp_direction = 0;
        break;
    case 2:
        opp_direction = 3;
        break;
    case 3:
        opp_direction = 2;
        break;
    }

    // route based on the ghost's ID
    // target = i_pacman.getPosition();
    find_target(i_pacman);
    int dx = target.x - position.x;
    int dy = target.y - position.y;
    // cout << "dx: " << dx << ", dy: " << dy << endl;

    if (dy == 0 && dy == 0)
    {
        // i_pacman.died();
        // cout << "DEADDDDDDDDDDD";
    }

    if (std::abs(dx) >= std::abs(dy))
    {
        if (dx > 0 && !walls[0])
        { // move right
            direction = 0;
        }
        else if (dx > 0 && walls[0])
        {
            for (int i = 0; i < 4; i++)
            {
                if (!walls[i] && i != opp_direction)
                {
                    direction = i;
                    break;
                }
            }
        }

        if (dx < 0 && !walls[1])
        { // move left
            direction = 1;
        }
        else if (dx < 0 && walls[1])
        {
            for (int i = 0; i < 4; i++)
            {
                if (!walls[i] && i != opp_direction)
                {
                    direction = i;
                    break;
                }
            }
        }
    }
    else
    {
        if (dy > 0 && !walls[3])
        { // move down
            direction = 3;
        }
        else if (dy > 0 && walls[3])
        {
            for (int i = 0; i < 4; i++)
            {
                if (!walls[i] && i != opp_direction)
                {
                    direction = i;
                    break;
                }
            }
        }

        if (dy < 0 && !walls[2])
        { // move up
            direction = 2;
        }
        else if (dy < 0 && walls[2])
        {
            for (int i = 0; i < 4; i++)
            {
                if (!walls[i] && i != opp_direction)
                {
                    direction = i;
                    break;
                }
            }
        }
    }

    switch (direction)
    {
    case 0: // move right
        if (!walls[0])
        {
            position.x += ghost_speed;
            // cout << "Moving right: " << position.x << endl;
        }
        break;

    case 1: //  left
        if (!walls[1])
        {
            position.x -= ghost_speed;
            // cout << "Moving left: " << position.x << endl;
        }
        break;

    case 2: // move up
        if (!walls[2])
        {
            position.y -= ghost_speed;
            // cout << "Moving up: " << position.y << endl;
        }
        break;
    case 3: //  down
        if (!walls[3])
        {
            position.y += ghost_speed;
            // cout << "Moving down: " << position.y << endl;
        }
        break;
    }

    return direction;
}

void Ghost::find_target(Pacman &i_pacman)
{
    switch (ghost_id)
    {
    case 0:
    {                                    // red ghost will chase Pacman directly
        target = i_pacman.getPosition(); // Get Pacman's position as the target
        break;
    }
    case 1:
    {                                    // pink ghost (chase 4th cell in front of pacman)
        target = i_pacman.getPosition(); //  pacman's position first

        // get pcmans current direction to see which way to go
        switch (i_pacman.get_direction())
        {
        case 0:                 // Right
            target.x += 16 * 4; // move 4 cells to the right of pman
            break;
        case 1: // up
            target.y -= 16 * 4;
            break;
        case 2: // left
            target.x -= 16 * 4;
            break;
        case 3: // down
            target.y += 16 * 4;
            break;
        }
        break;
    }
    case 2:
    { // blue ghost (chase 2nd cell in front of pacman)
        target = i_pacman.getPosition();

        switch (i_pacman.get_direction())
        {
        case 0:                 // Right
            target.x += 16 * 2; // move 2 cells to the right
            break;
        case 1: // Up
            target.y -= 16 * 2;
        case 2: // Left
            target.x -= 16 * 2;
            break;
        case 3: // Down
            target.y += 16 * 2;
            break;
        }
        break;
    }
    }
}

//---------------COPILOT CODE---------------------------
// int Ghost::ghost_movement(Pacman& i_pacman, Maze& maze, int last_direction) {
//     bool walls[4];
//     walls[0] = this->wall_collision(position.x + ghost_speed, position.y, 21, 21, maze); // Right
//     walls[1] = this->wall_collision(position.x - ghost_speed, position.y, 21, 21, maze); // Left
//     walls[2] = this->wall_collision(position.x, position.y - ghost_speed, 21, 21, maze); // Up
//     walls[3] = this->wall_collision(position.x, position.y + ghost_speed, 21, 21, maze); // Down

//     find_pacman(i_pacman);

//     int opp_direction;
//     switch (last_direction) {
//         case 0: opp_direction = 1; break;
//         case 1: opp_direction = 0; break;
//         case 2: opp_direction = 3; break;
//         case 3: opp_direction = 2; break;
//     }

//     target = i_pacman.getPosition();
//     int dx = target.x - position.x;
//     int dy = target.y - position.y;

//     int possible_directions[4] = {0, 1, 2, 3};
//     int new_direction = last_direction;

//     if (std::abs(dx) > std::abs(dy)) {
//         if (dx > 0 && !walls[0] && last_direction != 1) new_direction = 0;
//         else if (dx < 0 && !walls[1] && last_direction != 0) new_direction = 1;
//     } else {
//         if (dy > 0 && !walls[3] && last_direction != 2) new_direction = 3;
//         else if (dy < 0 && !walls[2] && last_direction != 3) new_direction = 2;
//     }

//     if (walls[new_direction]) {
//         for (int i = 0; i < 4; i++) {
//             if (!walls[possible_directions[i]] && possible_directions[i] != opp_direction) {
//                 new_direction = possible_directions[i];
//                 break;
//             }
//         }
//     }

//     // Ensure the ghost doesn't choose an invalid direction
//     if (walls[new_direction]) {
//         new_direction = opp_direction; // fallback to the opposite direction
//     }

//     switch (new_direction) {
//         case 0: if (!walls[0]) position.x += ghost_speed; break;
//         case 1: if (!walls[1]) position.x -= ghost_speed; break;
//         case 2: if (!walls[2]) position.y -= ghost_speed; break;
//         case 3: if (!walls[3]) position.y += ghost_speed; break;
//     }

//     return new_direction;
// }
