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

    return mousePos.x >= 270 && mousePos.x <= 560 &&
           mousePos.y >= 260 && mousePos.y <= 630;
}

void Game::StartScreen(sf::RenderWindow &window)
{

    window.clear();
    sf::Texture start_screen;

    // Try to load the image
    if (!start_screen.loadFromFile("new_start.jpg"))
    {
        std::cerr << "Error: Could not load start screen image!" << std::endl;
        return;
    }
    else
    {
        std::cout << "Image loaded successfully!" << std::endl;
    }

    sf::Sprite start(start_screen);
    start.setScale(sf::Vector2f(0.4f, 0.4f));

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
                    window.clear();
                    init(window); // Transition to the next screen
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

bool Game::restart_button(sf::Vector2i mousePos)
{
    return mousePos.x >= 270 && mousePos.x <= 560 && // might need to change based on buton loco
           mousePos.y >= 600 && mousePos.y <= 730;
}

void Game::WinScreen(sf::RenderWindow &window)
{
    sf::Texture start_screen;

    // Try to load the image
    if (!start_screen.loadFromFile("win_screen.jpg")) // change to win screen
    {
        std::cerr << "Error: Could not load start screen image!" << std::endl;
        return;
    }
    else
    {
        std::cout << "Image loaded successfully!" << std::endl;
    }

    sf::Sprite start(start_screen);
    start.setScale(sf::Vector2f(0.4f, 0.4f));
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
                bool check = restart_button(sf::Mouse::getPosition(window));
                if (check) //
                {
                    man.reset();
                    StartScreen(window); // Transition to the next screen
                    return;
                }
            }
        }

        // Clear the window, draw the image, and display
        window.clear();     // Clear the window before drawing the new frame
        window.draw(start); // Draw the sprite (image)
        man.draw_score(window);
        window.display(); // Display the window contents
    }
}

void Game::LoseScreen(sf::RenderWindow &window)
{
    sf::Texture start_screen;

    // Try to load the image
    if (!start_screen.loadFromFile("lose_screen.jpg")) // change to lose screen
    {
        std::cerr << "Error: Could not load start screen image!" << std::endl;
        return;
    }
    else
    {
        std::cout << "Image loaded successfully!" << std::endl;
    }

    sf::Sprite start(start_screen);
    start.setScale(sf::Vector2f(0.4f, 0.4f));
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
                if (restart_button(sf::Mouse::getPosition(window))) //
                {
                    man.reset();
                    StartScreen(window);
                    // StartScreen(window); // Transition to the next screen
                    return;
                }
            }
        }

        // Clear the window, draw the image, and display
        window.clear();     // Clear the window before drawing the new frame
        window.draw(start); // Draw the sprite (image)
        man.draw_score(window);
        window.display(); // Display the window contents
    }
}

void Game::init(sf::RenderWindow &window)
{
    // similar to lag, used to make the game framerate-independent.
    std::chrono::time_point<std::chrono::steady_clock> previous_time;

    srand(static_cast<unsigned>(time(0)));

    // get the current time and store it in a variable.
    previous_time = std::chrono::steady_clock::now();
    std::vector<Ghost> ghosts;
    ghosts.push_back(Ghost(0)); // red Ghost
    ghosts.push_back(Ghost(1)); // pink Ghost
    ghosts.push_back(Ghost(2)); // blue Ghost
    window.setFramerateLimit(100);
    pacman_maze.draw_maze(MAP_H, MAP_W, healths, poisons, window, true); // Draw the map
    int last_direction = 5;
    int status = 0;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            break;
        }

        for (size_t i = 0; i < ghosts.size(); i++)
        {
            ghosts[i].draw_ghost(window); // Call the draw method of each ghost
        }
        for (size_t i = 0; i < ghosts.size(); i++)
        {
            last_direction = ghosts[i].ghost_movement(man, pacman_maze, last_direction); // call ghost_movement for each ghost
        }
        man.draw(window);
        // man.setPosition(160, 176);
        pacman_maze.draw_maze(MAP_H, MAP_W, healths, poisons, window, false); // Draw the map
        status = man.movement(pacman_maze, healths, poisons, ghosts);
        if (status == 1)
        {
            LoseScreen(window);
            status = 0;
        }
        else if (status == 2)
        {
            WinScreen(window);
            status = 0;
        }

        window.display();
        window.clear();
        update(window);
    }
}

void Game::update(sf::RenderWindow &window)
{
    healths.draw(window, sf::Color::Green);
    poisons.draw(window);
    man.draw_data(window);
}
