//g++ character.cpp -I"C:\mingw_dev_lib\include" -L"C:\mingw_dev_lib\lib" -lsfml-graphics -lsfml-window -lsfml-system -o main.exe
#include <SFML/Graphics.hpp>
#include <iostream>


class Character{
    protected:
        sf::RectangleShape player;
        sf::Vector2f position = {160,128};
        int speed = 16;

    public:
        void draw(sf::RenderWindow &temp){
            sf::RectangleShape player(sf::Vector2f(16, 16));
            player.setFillColor(sf::Color::Blue);
            player.setPosition(position.x, position.y);
            temp.draw(player);

        }
        void respawn(){
                //position = {200,150};
            }
} ;


class Pacman : public Character{
    private:
        int lives = 3;
    public:
        // void increase_speed(){
        //     speed = 3;
        // }
        void increase_lives(){
            if (lives < 3){
                lives += 1;
            }
        }

        void movement(){
            sf::Vector2f index;
            index.x = position.x/16;
            index.y = position.y/16; 
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                position.x -= speed; // Move left
                std::cout << "left " << position.x << ", " << position.y <<  std::endl;
                std::cout << "left " << index.x << ", " << index.y <<  std::endl;
                
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                position.x += speed; // Move right
                player.move(speed,0.f);
                std::cout << "right " << position.x << ", " << position.y <<  std::endl;
                std::cout << "left " << index.x << ", " << index.y <<  std::endl;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                position.y -= speed; // up
                std::cout << "up " << position.x << ", " << position.y <<  std::endl;
                std::cout << "left " << index.x << ", " << index.y <<  std::endl;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                position.y += speed; // down
                std::cout << "down " << position.x << ", " << position.y <<  std::endl;
                std::cout << "left " << index.x << ", " << index.y <<  std::endl;
            }
            
        }

        void died(){
            if (lives == 0){
                std::cout << "game over!" << std::endl;
            }
            else{
                lives -= 1;
                respawn();
            }
        }   
};





// int main(){
//     sf::RenderWindow window(sf::VideoMode(400, 300), "Hit or Miss Ball");
//     Character man;
//     window.setFramerateLimit(60);
    
//     while (window.isOpen()) {
//         sf::Event event;
//         while (window.pollEvent(event)) {
//             if (event.type == sf::Event::Closed)
//             window.close();
//         }
//         man.draw(window);
//         man.movement();
//         man.respawn();
//         // Display the contents
//         window.display();
//         window.clear();
//     }
// }

