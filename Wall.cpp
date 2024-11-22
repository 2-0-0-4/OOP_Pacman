#include <array>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <chrono>
#include "character.hpp"
#include "food_poison.hpp"

class Wall
{
protected:
    sf::RectangleShape wall;
    sf::Vector2f position;

public:
    void draw(sf::RenderWindow &temp, int x_coord, int y_coord)
    {
        sf::RectangleShape wall(sf::Vector2f(16, 16));
        wall.setFillColor(sf::Color(32, 81, 190)); // Light Blue
        wall.setPosition(x_coord * 16, y_coord * 16);
        temp.draw(wall);
    }
};
