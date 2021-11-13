#include <iostream>
#include <queue>

using namespace std;

enum maze_result_t
{
    SOLVED = 0,
    NOT_SOLVED
} ;

struct Cell_t
{
    int val;
    int row;
    int col;
};


maze_result_t check_maze(int maze[3][3], int rows, int cols)
{
    maze_result_t result = NOT_SOLVED;

    //print array

    //  change value

    return result;
}

void hello()
{
    cout << "Hello" << endl;
}

int main()
{

    //hello();

    int  maze[3][3] = {1,1,1,1,1,1,1,1,1}; 

    // Создадим лабиринт
    // Передадим лабиринт в функцию

    check_maze(maze, 3, 3);
    //cout << "Hello World" << endl;
    return 0;
}

// Алгоритм Дейкстры
// Как передать двумерный массив в функцию
