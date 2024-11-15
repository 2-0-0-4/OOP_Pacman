//g++ maze+main.cpp -I"C:\mingw_dev_lib\include" -L"C:\mingw_dev_lib\lib" -lsfml-graphics -lsfml-window -lsfml-system -o main.exe

#pragma once
#include <array>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <chrono>
#include "character.cpp"

constexpr unsigned char CELL_SIZE=16;
constexpr unsigned char MAP_H=21;
constexpr unsigned char MAP_W=21;
constexpr unsigned char SCREEN_RESIZE=2;

constexpr unsigned short FRAME=16667;

class Wall{
    protected:
    sf::RectangleShape wall;
    sf::Vector2f position ;

public:
    void draw(sf::RenderWindow &temp, int x_coord, int y_coord){
        sf::RectangleShape wall(sf::Vector2f(16, 16));
        wall.setFillColor(sf::Color::White);
        wall.setPosition(x_coord*16, y_coord*16);
        temp.draw(wall);
    }
};



void draw_maze(const std::array<std::array<std::string, MAP_H>, MAP_W>& i_map, sf::RenderWindow& i_window){
    Wall temp_wall;

        for (unsigned char i= 0; i< MAP_H; i++){
            for (unsigned char j=0; j<MAP_W; j++){
                if(i_map[j][i] == "#"){
                    temp_wall.draw(i_window,i,j);
                }
                // else if(){
                //     //food comes here ??
                // }
			}
                
        }
}




int main()
{
	 unsigned lag = 0;

    // similar to lag, used to make the game framerate-independent.
    std::chrono::time_point<std::chrono::steady_clock> previous_time;

    // making the maze
    #include <array>
#include <string>


std::array<std::array<std::string, MAP_W>, MAP_H> maze_sketch = {{ 
{" ", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", " "}, 
{" ", "#", " ", " ", " ", " ", " ", " ", " ", " ", "#", " ", " ", " ", " ", " ", " ", " ", " ", "#", " "}, 
{" ", "#", " ", "#", "#", " ", "#", "#", "#", "#", " ", " ", "#", "#", "#", " ", "#", "#", " ", "#", " "}, 
{" ", "#", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "#", " "}, 
{" ", "#", " ", "#", "#", " ", "#", " ", "#", "#", "#", "#", "#", " ", "#", " ", "#", "#", " ", "#", " "}, 
{" ", "#", " ", " ", " ", " ", "#", " ", " ", " ", "#", " ", " ", " ", "#", " ", " ", " ", "#", " ", " "}, 
{" ", "#", "#", "#", "#", " ", "#", "#", "#", " ", "#", " ", "#", "#", "#", " ", "#", "#", "#", "#", " "}, 
{" ", " ", " ", " ", "#", " ", "#", " ", " ", " ", " ", " ", " ", " ", "#", " ", "#", " ", " ", " ", " "}, 
{"#", "#", "#", "#", "#", " ", "#", " ", "#", "#", " ", "#", "#", " ", "#", " ", "#", "#", "#", "#", "#"}, 
{" ", " ", " ", " ", " ", " ", " ", "#", " ", " ", " ", " ", " ", " ", " ", "#", " ", " ", " ", " ", " "}, 
{"#", "#", "#", "#", "#", " ", "#", " ", "#", "#", "#", "#", "#", " ", "#", " ", "#", "#", "#", "#", "#"}, 
{" ", " ", " ", " ", "#", " ", "#", " ", " ", " ", " ", " ", "#", " ", "#", " ", " ", " ", " ", "#", " "}, 
{" ", "#", "#", "#", "#", " ", "#", " ", "#", "#", "#", "#", "#", " ", "#", " ", "#", "#", "#", "#", " "}, 
{" ", "#", " ", " ", " ", " ", " ", " ", " ", " ", "#", " ", " ", " ", " ", " ", " ", " ", " ", "#", " "}, 
{" ", "#", " ", "#", "#", " ", "#", "#", "#", " ", "#", " ", "#", "#", "#", " ", "#", "#", " ", "#", " "}, 
{" ", "#", " ", " ", "#", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "#", " ", " ", " ", "#", " "}, 
{" ", "#", "#", " ", "#", " ", "#", " ", "#", "#", "#", "#", "#", " ", "#", " ", "#", "#", " ", "#", " "}, 
{" ", "#", " ", " ", "#", " ", " ", " ", "#", " ", " ", " ", "#", " ", " ", " ", "#", " ", " ", "#", " "}, 
{" ", "#", " ", "#", "#", "#", "#", "#", "#", " ", "#", " ", "#", "#", "#", "#", "#", "#", "#", "#", " "}, 
{" ", "#", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "#", " "}, 
{" ", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", " ", " "}
}};


    // SFML thing. Stores events, I think.
    sf::Event event;

    sf::RenderWindow window(sf::VideoMode(CELL_SIZE * MAP_W * SCREEN_RESIZE, (CELL_SIZE * MAP_H) * SCREEN_RESIZE), "Pac-Man", sf::Style::Close);
    // making the window to fit the maze
    window.setView(sf::View(sf::FloatRect(0, 0, CELL_SIZE * MAP_W, CELL_SIZE * MAP_H)));

    // generating a random seed.
    srand(static_cast<unsigned>(time(0)));


    // get the current time and store it in a variable.
    previous_time = std::chrono::steady_clock::now();
    
    
    Pacman man;
    window.setFramerateLimit(11);
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
            window.close();
            break;

        }
        // we're calculating the lag here
        unsigned delta_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - previous_time).count();
        lag += delta_time;

        previous_time += std::chrono::microseconds(delta_time);

        // while the lag exceeds the maximum allowed frame duration.
        while (lag >= FRAME) {
            // we decrease the lag.
            lag -= FRAME;

            while (window.pollEvent(event)) {
                switch (event.type) {
                case sf::Event::Closed:
                    
                    window.close();
                    break;
                }
            }

            // add the rest of the stuff here

        }
            man.draw(window);
            man.movement();
            draw_maze(maze_sketch, window);  // Draw the map
            window.display();
            window.clear();
    }

    return 0;
}

