#include <SFML/Graphics.hpp>
using namespace sf;
#include "food_poison.hpp"
#include <iostream>

void Food::add_to_array(int x, int y){
    locations.push_back({x, y});
}

void Food::remove_from_array(int x, int y){
       // Create the target vector to find
    std::vector<int> target = {x, y};

    // Find the target vector in the locations array
    auto it = std::find(locations.begin(), locations.end(), target);

    // If found, remove it
    if (it != locations.end()) {
        locations.erase(it);
    } else {
        std::cout << "Element {" << x << ", " << y << "} not found in locations.\n";
    }
}

