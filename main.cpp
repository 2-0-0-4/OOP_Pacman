#include "Game.hpp"
//g++ *.cpp -I"C:\mingw_dev_lib\include" -L"C:\mingw_dev_lib\lib" -lsfml-graphics -lsfml-window -lsfml-system -o main.exe

int main(){
    Game game;
    game.init();
    return 0;
}