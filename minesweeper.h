#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <stdlib.h>
#include <time.h>
#include <cstdlib>

using namespace std;

class Minesweeper
{
public:
    Minesweeper();
    void displayBoards();
    void play();

private:

    void initializeBoards(int size_input);
    void initializeBombBoard(int size_input);

    void incrementAdj(int i, int j);
    bool checkAdjCell(int i, int j);
    bool checkBombCell(int i, int j);

    void depthFirstSearch(int i, int j, int deltaRow[], int deltaCol[]);
    void floodFill(int i, int j);
    void updateBoard(int i, int j);
    void revealAdjEdge(int i, int j);

    void displayWelcome();
    void displayGameOver();
    void displayPlayerBoard();
    void displayBombBoard();
    void displayAdjBoard();
    void displayCurrBoard();
    void displayFinalBoard();
    

    // This Board will be visible to the player
    vector<vector<char>> playerBoard;
    // This Board will hold where da bombs at
    vector<vector<bool>> bombBoard;
    // This board will hold each squares adjacent bomb count
    vector<vector<int>> adjBoard;
    // This is for flood fill, keeping track of where we recursion has touched
    vector<vector<bool>> visitedBoard;
    int size;
};