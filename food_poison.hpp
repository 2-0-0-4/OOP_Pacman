#ifndef FOOD_POISON_HPP
#define FOOD_POISON_HPP
#include <SFML/Graphics.hpp>
using namespace sf;

class Food {
    protected:
        std::vector<std::vector<int>> locations{};
    public:
        void draw(sf::RenderWindow &temp, sf::Color circle_color);
        void add_to_array(int x, int y);
        void remove_from_array(int x, int y);
        virtual ~Food() = 0;

};

class Health:public Food{
    private:
        int points;
    public: 
        void consume();
};

class Poison:public Food{
    private:
        int points;
    public:
        void consume();
};

  
#endif