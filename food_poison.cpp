#include <SFML/Graphics.hpp>
using namespace sf;
#include "food_poison.hpp"
#include <iostream>

void Food::add_to_array(int x, int y)
{
    locations.push_back({x, y});
}

void Food::remove_from_array(int x, int y)
{
    // Create the target vector to find
    std::vector<int> target = {x, y};

    // Find the target vector in the locations array
    auto it = std::find(locations.begin(), locations.end(), target);

    // If found, remove it
    if (it != locations.end())
    {
        locations.erase(it);
    }
    else
    {
        std::cout << "Element {" << x << ", " << y << "} not found in locations.\n";
    }
}

void Food::draw(sf::RenderWindow &temp, sf::Color circle_color)
{ // draws all food circles at the given locations stored in the array locations
    for (std::vector<int> location : locations)
    {                                      // gets location of each food
        sf::CircleShape food_circle(2.5f); // orginal size was 3.0
        food_circle.setFillColor(circle_color);
        food_circle.setPosition((location[0] * 16) + 5, (location[1] * 16) + 5);
        temp.draw(food_circle);
    }
}

bool Food::is_empty()
{
    return locations.empty();
};

Food::~Food() {};

bool Food::find_in_array(int x, int y)
{
    std::vector<int> target = {x, y};

    // Find the target vector in the locations array
    auto it = std::find(locations.begin(), locations.end(), target);
    return it != locations.end();
}

void Poison::draw(sf::RenderWindow &temp)
{
    for (std::vector<int> location : locations)
    { // gets location of each food
        sf::Texture poison_texture;
        poison_texture.loadFromFile("poison_sprite.png");
        sf::Sprite p_sprite;
        p_sprite.setTexture(poison_texture);
        p_sprite.setScale(sf::Vector2f(0.035f, .035f));
        p_sprite.setPosition((location[0] * 16), (location[1] * 16));
        temp.draw(p_sprite);
    }
}
