#ifndef GAME_HPP
#define GAME_HPP
#include <SFML/Graphics.hpp>
#include "character.hpp"
#include "Maze.hpp"
#include "food_poison.hpp"
#include <chrono>
#pragma once

class Game{
    private:
        
        const unsigned char CELL_SIZE = 16;
        const int MAP_H = 21;
        const int MAP_W = 21;
        const unsigned char SCREEN_RESIZE = 2;
        const unsigned short FRAME = 16667;
        Pacman man;
        Health healths;
        Poison poisons;
        Maze pacman_maze;
        
    public:
        Game(); 
        ~Game(); 
        void show_start();
        void show_start(sf::RenderWindow &window);
        void StartScreen(sf::RenderWindow &window);
        bool screen_clicked(sf::Vector2i mousePos);
        void init(sf::RenderWindow &window); //this should be the main window.open loop thigy
        void update(sf::RenderWindow& window); //redraws the maze. this will be called in init after any event
        //we need to call update cause lets say smth gets removed from the food array so we need to 
        //re draw teh maze everytime so that change is reflected on screen 
        //IF IN THE FUTURE WE WANT SOUNDS ETC TO BE PLAYED. WE WILL MAKE FUCNTIONS HERE TO ADD THOSE
        void show_win();
        void WinScreen(sf::RenderWindow& window);
        bool restart_button(sf::Vector2i mousePos);

        void show_lose();
        void LoseScreen(sf::RenderWindow& window);
        void reset();
        
        
        // bool screen_clicked_lose(sf::Vector2i mousePos);

};

#endif
