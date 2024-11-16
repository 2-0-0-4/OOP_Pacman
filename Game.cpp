#include "Game.hpp"
//g++ *.cpp -I"C:\mingw_dev_lib\include" -L"C:\mingw_dev_lib\lib" -lsfml-graphics -lsfml-window -lsfml-system -o main.exe

Game::Game(){};
Game::~Game(){};

void Game::init(){
     unsigned lag = 0;

    // similar to lag, used to make the game framerate-independent.
    std::chrono::time_point<std::chrono::steady_clock> previous_time;
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
            pacman_maze.draw_maze(MAP_H,MAP_W,window);  // Draw the map
            window.display();
            window.clear();
    }

}