// g++ character.cpp -I"C:\mingw_dev_lib\include" -L"C:\mingw_dev_lib\lib" -lsfml-graphics -lsfml-window -lsfml-system -o main.exe
#include <SFML/Graphics.hpp>
#include <iostream>
#include "character.hpp"
#include <cstring>
#include <cmath>
using namespace std;

void Character::draw(sf::RenderWindow &temp)
{

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
    float reductionFactor = 0.9f; // Adjust this factor as needed to allow more overlap
    sf::FloatRect bounds = player.getGlobalBounds();
    float widthReduction = bounds.width * reductionFactor;
    float heightReduction = bounds.height * reductionFactor;

    return sf::FloatRect(bounds.left + widthReduction / 2, bounds.top + heightReduction / 2,
                         bounds.width - widthReduction, bounds.height - heightReduction);
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
        }
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        if (walls[1] == false)
        {
            direction = 1;
        }
    }

    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        if (walls[2] == false)
        {
            direction = 2;
        }
    }

    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        if (walls[3] == false)
        {
            direction = 3;
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

void Pacman::draw_score(sf::RenderWindow &window)
{
    sf::Font font;
    font.loadFromFile("ka1.ttf");
    sf::Text text_score;
    sf::Text text;
    text_score.setFont(font);
    text_score.setFillColor(sf::Color::White);
    text_score.setString(std::to_string(score));
    text_score.setCharacterSize(18);
    text_score.setPosition(180, 240);

    //  the "Score:" label
    text.setFont(font);
    text.setFillColor(sf::Color::White);
    text.setString("Score:");
    text.setCharacterSize(18);
    text.setPosition(95, 240);

    // Draw the text
    window.draw(text);
    window.draw(text_score);
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
    switch (ghost_id)
    {
    case 0: // red Ghost Mikaal
        texture.loadFromFile("mikaal_sprite.png");
        player.setTexture(texture);
        player.setScale(sf::Vector2f(0.03f, 0.03f));
        break;
    case 1: // pink Ghost
        texture.loadFromFile("adeenah_sprite.png");
        player.setTexture(texture);
        player.setScale(sf::Vector2f(0.028f, 0.025f));
        break;
    case 2: // cyan Ghost
        texture.loadFromFile("afeera_sprite.png");
        player.setTexture(texture);
        player.setScale(sf::Vector2f(0.03f, 0.03f));
        break;
    }

    player.setPosition(position.x, position.y);
    i_window.draw(player);
}

//___________________________________________________________________________________________________________________
// INITIAL CODE
// int Ghost::ghost_movement(Pacman &i_pacman, Maze &maze, int last_direction)
// {
//     bool walls[4];
//     walls[0] = this->wall_collision(position.x + ghost_speed, position.y, 21, 21, maze); // Right
//     walls[1] = this->wall_collision(position.x - ghost_speed, position.y, 21, 21, maze); // Left
//     walls[2] = this->wall_collision(position.x, position.y - ghost_speed, 21, 21, maze); // Up
//     walls[3] = this->wall_collision(position.x, position.y + ghost_speed, 21, 21, maze); // Down

//     // calculate movement direction towards the target
//     int opp_direction;
//     switch (last_direction)
//     {
//     case 0:
//         opp_direction = 1;
//         break;
//     case 1:
//         opp_direction = 0;
//         break;
//     case 2:
//         opp_direction = 3;
//         break;
//     case 3:
//         opp_direction = 2;
//         break;
//     }

//     // route based on the ghost's ID
//     find_target(i_pacman);
//     int dx = target.x - position.x;
//     int dy = target.y - position.y;

//     if (std::abs(dx) >= std::abs(dy))
//     {
//         if (dx > 0 && !walls[0])
//         { // move right
//             direction = 0;
//         }
//         else if (dx > 0 && walls[0])
//         {
//             for (int i = 0; i < 4; i++)
//             {
//                 if (!walls[i] && i != opp_direction)
//                 {
//                     direction = i;
//                     break;
//                 }
//             }
//         }

//         if (dx < 0 && !walls[1])
//         { // move left
//             direction = 1;
//         }
//         else if (dx < 0 && walls[1])
//         {
//             for (int i = 0; i < 4; i++)
//             {
//                 if (!walls[i] && i != opp_direction)
//                 {
//                     direction = i;
//                     break;
//                 }
//             }
//         }
//     }
//     else
//     {
//         if (dy > 0 && !walls[3])
//         { // move down
//             direction = 3;
//         }
//         else if (dy > 0 && walls[3])
//         {
//             for (int i = 0; i < 4; i++)
//             {
//                 if (!walls[i] && i != opp_direction)
//                 {
//                     direction = i;
//                     break;
//                 }
//             }
//         }

//         if (dy < 0 && !walls[2])
//         { // move up
//             direction = 2;
//         }
//         else if (dy < 0 && walls[2])
//         {
//             for (int i = 0; i < 4; i++)
//             {
//                 if (!walls[i] && i != opp_direction)
//                 {
//                     direction = i;
//                     break;
//                 }
//             }
//         }
//     }

//     switch (direction)
//     {
//     case 0: // move right
//         if (!walls[0])
//         {
//             position.x += ghost_speed;
//             // cout << "Moving right: " << position.x << endl;
//         }
//         break;

//     case 1: //  left
//         if (!walls[1])
//         {
//             position.x -= ghost_speed;
//             // cout << "Moving left: " << position.x << endl;
//         }
//         break;

//     case 2: // move up
//         if (!walls[2])
//         {
//             position.y -= ghost_speed;
//             // cout << "Moving up: " << position.y << endl;
//         }
//         break;
//     case 3: //  down
//         if (!walls[3])
//         {
//             position.y += ghost_speed;
//             // cout << "Moving down: " << position.y << endl;
//         }
//         break;
//     }

//     return direction;
// }

int Ghost::ghost_movement(Pacman &i_pacman, Maze &maze, int last_direction)
{
    static int directionChangeCooldown = 0;

    bool walls[4];
    walls[0] = this->wall_collision(position.x + ghost_speed, position.y, 21, 21, maze); // Right
    walls[1] = this->wall_collision(position.x - ghost_speed, position.y, 21, 21, maze); // Left
    walls[2] = this->wall_collision(position.x, position.y - ghost_speed, 21, 21, maze); // Up
    walls[3] = this->wall_collision(position.x, position.y + ghost_speed, 21, 21, maze); // Down

    if (directionChangeCooldown > 0)
    {
        directionChangeCooldown--;
    }
    else
    {
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

        // Route based on the ghost's ID
        find_target(i_pacman);
        int dx = target.x - position.x;
        int dy = target.y - position.y;

        if (std::abs(dx) >= std::abs(dy))
        {
            if (dx > 0 && !walls[0])
            { // Move right
                direction = 0;
            }
            else if (dx < 0 && !walls[1])
            { // Move left
                direction = 1;
            }
        }
        else
        {
            if (dy > 0 && !walls[3])
            { // Move down
                direction = 3;
            }
            else if (dy < 0 && !walls[2])
            { // Move up
                direction = 2;
            }
        }

        if (walls[direction] || direction == opp_direction)
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
        directionChangeCooldown = 3; // Adjust this value as needed for smoother changes
    }

    switch (direction)
    {
    case 0: // Move right
        if (!walls[0])
        {
            position.x += ghost_speed;
        }
        break;

    case 1: // Move left
        if (!walls[1])
        {
            position.x -= ghost_speed;
        }
        break;

    case 2: // Move up
        if (!walls[2])
        {
            position.y -= ghost_speed;
        }
        break;

    case 3: // Move down
        if (!walls[3])
        {
            position.y += ghost_speed;
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

// THIRD CODE:

// int Ghost::ghost_movement(Pacman &i_pacman, Maze &maze, int last_direction)
// {
//     static int directionChangeCooldown = 0;
//     // const int cellSize = 16;
//     // const int mazeSize = 21 * cellSize; // 21 cells * 16 pixels per cell

//     bool walls[4];
//     walls[0] = this->wall_collision(position.x + ghost_speed, position.y, 21, 21, maze); // Right
//     walls[1] = this->wall_collision(position.x - ghost_speed, position.y, 21, 21, maze); // Left
//     walls[2] = this->wall_collision(position.x, position.y - ghost_speed, 21, 21, maze); // Up
//     walls[3] = this->wall_collision(position.x, position.y + ghost_speed, 21, 21, maze); // Down

//     if (directionChangeCooldown > 0)
//     {
//         directionChangeCooldown--;
//     }
//     else
//     {
//             int opp_direction;
//             switch (last_direction)
//             {
//             case 0:
//                 opp_direction = 1;
//                 break;
//             case 1:
//                 opp_direction = 0;
//                 break;
//             case 2:
//                 opp_direction = 3;
//                 break;
//             case 3:
//                 opp_direction = 2;
//                 break;
//             }

//         // Route based on the ghost's ID
//         find_target(i_pacman);
//         int dx = target.x - position.x;
//         int dy = target.y - position.y;

//         if (std::abs(dx) >= std::abs(dy))
//         {
//             if (dx > 0 && !walls[0])
//             { // Move right
//                 direction = 0;
//             }
//             else if (dx < 0 && !walls[1])
//             { // Move left
//                 direction = 1;
//             }
//         }
//         else
//         {
//             if (dy > 0 && !walls[3])
//             { // Move down
//                 direction = 3;
//             }
//             else if (dy < 0 && !walls[2])
//             { // Move up
//                 direction = 2;
//             }
//         }

//         if (walls[direction] || direction == opp_direction)
//         {
//             for (int i = 0; i < 4; i++)
//             {
//                 if (!walls[i] && i != opp_direction)
//                 {
//                     direction = i;
//                     break;
//                 }
//             }
//         }

//         directionChangeCooldown = 3; // Adjust this value as needed for smoother changes
//     }

//     switch (direction)
//     {
//     case 0: // Move right
//         if (!walls[0])
//         {
//             position.x += ghost_speed;
//         }
//         break;

//     case 1: // Move left
//         if (!walls[1])
//         {
//             position.x -= ghost_speed;
//         }
//         break;

//     case 2: // Move up
//         if (!walls[2])
//         {
//             position.y -= ghost_speed;
//         }
//         break;

//     case 3: // Move down
//         if (!walls[3])
//         {
//             position.y += ghost_speed;
//         }
//         break;
//     }

//     if (-16 > position.x)
//     {
//         position.x = 16 * 21 - ghost_speed; // wraps it around and makess it come out from the right
//     }
//     else if (16 * 21 <= position.x)
//     {
//         position.x = ghost_speed - 16; // wraps it around and makess it come out from the left
//     }

//     // // Wrap-around condition
//     // if (position.x >= mazeSize)
//     // {
//     //     position.x = 0;
//     // }
//     // else if (position.x < 0)
//     // {
//     //     position.x = mazeSize - cellSize;
//     // }

//     // if (position.y >= mazeSize)
//     // {
//     //     position.y = 0;
//     // }
//     // else if (position.y < 0)
//     // {
//     //     position.y = mazeSize - cellSize;
//     // }

//     return direction;
// }
