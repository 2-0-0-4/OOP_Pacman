#include "Game.hpp"
#include <iostream>
// g++ *.cpp -I"C:\mingw_dev_lib\include" -L"C:\mingw_dev_lib\lib" -lsfml-graphics -lsfml-window -lsfml-system -o main.exe
//  const int CELL_SIZE= 16;
//  const int MAP_W= 21;
//  const int MAP_H=21;
//  const int SCREEN_RESIZE=2;

Game::Game() {};
Game::~Game() {};
void Game::show_start()
{
    sf::RenderWindow window(sf::VideoMode(CELL_SIZE * MAP_W * SCREEN_RESIZE, (40 + CELL_SIZE * MAP_H) * SCREEN_RESIZE), "Pac-Man", sf::Style::Close);
    window.setView(sf::View(sf::FloatRect(0, 0, CELL_SIZE * MAP_W, 40 + CELL_SIZE * MAP_H)));

    // Show the start screen
    StartScreen(window);
}
bool Game::screen_clicked(sf::Vector2i mousePos)
{

    return mousePos.x >= 300 && mousePos.x <= 700 &&
           mousePos.y >= 500 && mousePos.y <= 800;
}

void Game::StartScreen(sf::RenderWindow &window)
{
    sf::Texture start_screen;

    // Try to load the image
    if (!start_screen.loadFromFile("pacman_start.jpg"))
    {
        std::cerr << "Error: Could not load start screen image!" << std::endl;
        return;
    }
    else
    {
        std::cout << "Image loaded successfully!" << std::endl;
    }

    sf::Sprite start(start_screen);

    // start.setScale(window.getSize().x / float(start.getLocalBounds().width),
    //                window.getSize().y / float(start.getLocalBounds().height));

    // position the image  in the center of the window
    //  start.setPosition((window.getSize().x - start.getLocalBounds().width) / 2,
    //                    (window.getSize().y - start.getLocalBounds().height) / 2);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            // if mouse clicked, transition to the game
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (screen_clicked(sf::Mouse::getPosition(window)))
                {
                    init(); // Transition to the next screen
                    return;
                }
            }
        }

        // Clear the window, draw the image, and display
        window.clear();     // Clear the window before drawing the new frame
        window.draw(start); // Draw the sprite (image)
        window.display();   // Display the window contents
    }
}
void Game::init()
{

    std::vector<Ghost> ghosts;
        ghosts.push_back(Ghost(0));  // red Ghost
        ghosts.push_back(Ghost(1));  // pink Ghost
        ghosts.push_back(Ghost(2));  // blue Ghost
    // similar to lag, used to make the game framerate-independent.
    std::chrono::time_point<std::chrono::steady_clock> previous_time;
    // SFML thing. Stores events, I think.
    sf::Event event;

    sf::RenderWindow window(sf::VideoMode(CELL_SIZE * MAP_W * SCREEN_RESIZE, (40 + CELL_SIZE * MAP_H) * SCREEN_RESIZE), "Pac-Man", sf::Style::Close);
    // making the window to fit the maze
    window.setView(sf::View(sf::FloatRect(0, 0, CELL_SIZE * MAP_W, 40 + CELL_SIZE * MAP_H)));

    // generating a random seed.
    srand(static_cast<unsigned>(time(0)));

    // get the current time and store it in a variable.
    previous_time = std::chrono::steady_clock::now();

    // Pacman man;
    // Ghost ghost;
    window.setFramerateLimit(100);
    pacman_maze.draw_maze(MAP_H, MAP_W, healths, poisons, window, true); // Draw the map
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            break;
        }
   
        for (size_t i = 0; i < ghosts.size(); i++) {
        ghosts[i].draw_ghost(window);  // Call the draw method of each ghost
    }
        for (size_t i = 0; i < ghosts.size(); i++) {
            ghosts[i].ghost_movement(man, pacman_maze);  // call ghost_movement for each ghost
                       
        }
        man.draw(window);
        pacman_maze.draw_maze(MAP_H, MAP_W, healths, poisons, window, false); // Draw the map
        man.movement(pacman_maze, healths, poisons);
        // ghost.ghost_movement( man,pacman_maze);
        // healths.draw(window,sf::Color::Green);
        // poisons.draw(window,sf::Color::Red);
        // healths.print();
        // poisons.print();
        window.display();
        window.clear();
        update(window);
    }
}

void Game::update(sf::RenderWindow &window)
{
    // pacman_maze.draw_maze(MAP_H,MAP_W,healths,poisons,window);  // Draw the map
    // std::cout<<"update"<<std::endl;
    healths.draw(window, sf::Color::Green);
    poisons.draw(window);
    man.draw_data(window);
}

