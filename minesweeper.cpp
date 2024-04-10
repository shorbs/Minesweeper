#include "minesweeper.h"

using namespace std;
/*
    General ToDo's:
    -Less ambiguity between i and j as iterator variables and accessor variables
        *Maybe use row and col for accessor and i and j exclusivly for iterating
    -Flesh out the play() function to look better
        *Maybe add a more friendly and descriptive prompt for difficulty
    -Use the deltaRow and deltaCol for the incrementAdj function
        *Maybe this will improve readability? It will definitley increase consistency
    -Need to change all variable and function names to Benji Standard
        *functions = CamelCase
        *variables = whatever_this_is
*/
Minesweeper::Minesweeper()
{
    
    int difficulty;
    displayWelcome();
    cout << "===== Difficulty MENU =====" << endl;
    cout << "Level 1: 9x9 with 10 bombs" << endl;
    cout << "Level 2: 16x16 with 40 bombs" << endl;
    cout << "Level 3: 24x24 with 115 bombs" << endl;
    cout << "Please enter difficulty 1, 2 or 3: ";
    cin >> difficulty;

    while (difficulty < 1 || difficulty > 3)
    {
        cout << "Please enter valid difficulty 1, 2 or 3: ";
        cin >> difficulty;
    }

    switch (difficulty)
    {
    case 1:
        initializeBoards(9);
        break;
    case 2:
        initializeBoards(16);
        break;
    case 3:
        initializeBoards(24);
        break;
    }
}
void Minesweeper::play()
{
    int column;
    int row;
    bool game = true;
    while (game)
    {
        system("cls");
        displayCurrBoard();
        cout << "Enter row: " << endl;
        cin >> row;
        cout << "Enter column: " << endl;
        cin >> column;
        while ((row >= size || row < 0) || (column >= size || column < 0))
        {
            cout << "Enter valid row: " << endl;
            cin >> row;
            cout << "Enter valid column: " << endl;
            cin >> column;
        }
        if (checkBombCell(row, column))
        {
            system("cls");
            displayGameOver();
            displayFinalBoard();
            return;
        }
        updateBoard(row, column);
    }
}

void Minesweeper::initializeBoards(int size_input)
{
    playerBoard.resize(size_input);
    bombBoard.resize(size_input);
    adjBoard.resize(size_input);
    visitedBoard.resize(size_input);
    for (int i = 0; i < size_input; i++)
    {
        for (int j = 0; j < size_input; j++)
        {
            playerBoard.at(i).push_back((char)254);
            bombBoard.at(i).push_back(false);
            adjBoard.at(i).push_back(0);
            visitedBoard.at(i).push_back(false);
        }
    }
    initializeBombBoard(size_input);
}
void Minesweeper::initializeBombBoard(int size_input)
{
    this->size = size_input;

    int bomb_count;
    switch (size_input)
    {
    case 9:
        bomb_count = 10;
        break;
    case 16:
        bomb_count = 40;
        break;
    case 24:
        // 115 bombs so that there is a 1/5 chance for a bomb to be on a tile
        bomb_count = 115;
        break;
    }

    while (bomb_count > 0)
    {
        int i = rand() % size_input;
        int j = rand() % size_input;
        // cout << "(" << i << " , " << j << ")" << endl;
        while (bombBoard[i][j] == true)
        {
            i = rand() % size_input;
            j = rand() % size_input;
        }
        bombBoard[i][j] = true;
        adjBoard[i][j] = size_input;
        incrementAdj(i, j);
        bomb_count--;
    }
}
bool Minesweeper::checkAdjCell(int i, int j)
{
    // If the input coordinates are out of range
    if ((i >= size || i < 0) || (j >= size || j < 0))
        return false;
    return true;
}
bool Minesweeper::checkBombCell(int i, int j)
{
    return bombBoard[i][j];
}

void Minesweeper::depthFirstSearch(int row, int col, int deltaRow[], int deltaCol[])
{
    // Reveal current cell
    playerBoard[row][col] = adjBoard[row][col] + 48;
    visitedBoard[row][col] = true;
    revealAdjEdge(row, col);
    // Touch all directly adjacent (nondiagonal) cells
    for (int i = 0; i < 4; i++)
    {
        // neihboring row
        int n_row = row + deltaRow[i];
        // neihboring column
        int n_col = col + deltaCol[i];
        // check for valid cell
        if ((n_row < size && n_row >= 0) && (n_col < size && n_col >= 0) && adjBoard[n_row][n_col] == 0 && visitedBoard[n_row][n_col] == false)
        {
            depthFirstSearch(n_row, n_col, deltaRow, deltaCol);
        }
    }
}
void Minesweeper::floodFill(int i, int j)
{
    int deltaRow[] = {-1, 0, +1, 0};
    int deltaCol[] = {0, +1, 0, -1};
    depthFirstSearch(i, j, deltaRow, deltaCol);
}
void Minesweeper::updateBoard(int i, int j)
{
    if (adjBoard[i][j] == 0)
        floodFill(i, j);
    else
        playerBoard[i][j] = adjBoard[i][j] + 48;
}
void Minesweeper::revealAdjEdge(int row, int col)
{
    int deltaRow[] = {-1, +1, 0, 0, -1, -1, +1, +1};
    int deltaCol[] = {0, 0, -1, +1, +1, -1, +1, -1};
    for (int i = 0; i < 8; i++)
    {
        // neihboring row
        int n_row = row + deltaRow[i];
        // neihboring column
        int n_col = col + deltaCol[i];
        // check for valid cell
        if ((n_row < size && n_row >= 0) && (n_col < size && n_col >= 0) && adjBoard[n_row][n_col] != 0 && visitedBoard[n_row][n_col] == false)
        {
            playerBoard[n_row][n_col] = adjBoard[n_row][n_col] + 48;
        }
    }
}
void Minesweeper::incrementAdj(int i, int j)
{
    if (checkAdjCell(i - 1, j))
        adjBoard[i - 1][j]++;
    if (checkAdjCell(i + 1, j))
        adjBoard[i + 1][j]++;
    if (checkAdjCell(i, j - 1))
        adjBoard[i][j - 1]++;
    if (checkAdjCell(i, j + 1))
        adjBoard[i][j + 1]++;

    if (checkAdjCell(i - 1, j + 1))
        adjBoard[i - 1][j + 1]++;
    if (checkAdjCell(i - 1, j - 1))
        adjBoard[i - 1][j - 1]++;
    if (checkAdjCell(i + 1, j + 1))
        adjBoard[i + 1][j + 1]++;
    if (checkAdjCell(i + 1, j - 1))
        adjBoard[i + 1][j - 1]++;
}

void Minesweeper::displayWelcome(){
    cout << "  ______________________________      \\  | / | \\" << endl;
    cout << " /        Welcome To          / \\     \\ \\ / /.   ." << endl;
    cout << "|        MINESWEEPER         | ==========  - -" << endl;
    cout << "\\____________________________\\_/     / / \\ \\    /" << endl;
    cout << "                                       / |\\  | /  ." << endl;
}
void Minesweeper::displayGameOver(){
    cout << "      _.-^^---....,,--       " << endl;
    cout << "  _--                  --_  " << endl;
    cout << "(< Thank you for playing >)" << endl;
    cout << "|       MINESWEEPER       | " << endl;
    cout << " \\._                   _./  " << endl;
    cout << "    ```--. . , ; .--'''       " << endl;
    cout << "          | |   |             " << endl;
    cout << "       .-=||  | |=-.   " << endl;
    cout << "       `-=#$%&%$#=-'   " << endl;
    cout << "          | ;  :|     " << endl;
    cout << " _____.,-#%&$@%#&#~,._____" << endl;
}
void Minesweeper::displayBoards()
{
    displayPlayerBoard();
    displayBombBoard();
    displayAdjBoard();
}
void Minesweeper::displayPlayerBoard()
{
    cout << "Player Board" << endl;
    for (int i = 0; i < playerBoard.size(); i++)
    {
        for (int j = 0; j < playerBoard.size(); j++)
        {
            cout << setw(3) << playerBoard[i][j] << " ";
        }
        cout << endl;
    }
}
void Minesweeper::displayBombBoard()
{
    cout << "Bomb Board" << endl;
    for (int i = 0; i < bombBoard.size(); i++)
    {
        for (int j = 0; j < bombBoard.size(); j++)
        {
            cout << setw(3) << bombBoard[i][j] << " ";
        }
        cout << endl;
    }
}
void Minesweeper::displayAdjBoard()
{
    cout << "Adjacency Board" << endl;
    for (int i = 0; i < adjBoard.size(); i++)
    {
        for (int j = 0; j < adjBoard.size(); j++)
        {
            cout << setw(3) << adjBoard[i][j] << " ";
        }
        cout << endl;
    }
}
void Minesweeper::displayCurrBoard()
{
    for (int i = 0; i < playerBoard.size(); i++)
    {
        for (int j = 0; j < playerBoard.size(); j++)
        {
            cout << setw(3) << playerBoard[i][j] << " ";
        }
        cout << endl;
    }
}
void Minesweeper::displayFinalBoard()
{
    for (int i = 0; i < adjBoard.size(); i++)
    {
        for (int j = 0; j < adjBoard.size(); j++)
        {
            if (bombBoard[i][j])
                cout << setw(3) << (char)254 << " ";
            else
                cout << setw(3) << adjBoard[i][j] << " ";
        }
        cout << endl;
    }
}