#include <iostream>
#include "minesweeper.h"
using namespace std;

int main(int argc,char **argv)
{
    srand(time(NULL));
    Minesweeper game;
    game.play();
    return 0;
};