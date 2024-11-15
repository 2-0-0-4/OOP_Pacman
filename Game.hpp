#include <SFML/Graphics.hpp>
#include "character.hpp"
class Game{
    private:
        //initlaise character, maze, ghosts EVETRTHING here
    public:
        Game(); 
        ~Game(); 
        void init(); //this should be the main window.open loop thigy
        void update(); //redraws the maze. this will be called in init after any event
        //we need to call update cause lets say smth gets removed from the food array so we need to 
        //re draw teh maze everytime so that change is reflected on screen 
        //IF IN THE FUTURE WE WANT SOUNDS ETC TO BE PLAYED. WE WILL MAKE FUCNTIONS HERE TO ADD THOSE

};