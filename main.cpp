#include <iostream>
#include <queue>

#include <cstdlib>     /* srand, rand */
#include <ctime>       /* time */

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

void set_entrance_and_exit(int** maze, int rows, int cols)
{
    /* Пусть левая верхняя клетка будет всегда 1. Это будет наш вход. */
    maze[0][0] = 1;

    /* Пусть правая нижняя клетка будет всегда 3. Это будет наш выход. */
    maze[rows-1][cols-1] = 3;
}

void fill_maze_by_random_numbers(int** maze, int rows, int cols)
{
    for (auto row = 0; row < 3; row++)
    {
        for (auto col = 0; col < 3; col++)
        {
            maze[row][col] = rand() % 2; // fill cell by 0 or 1
        }
    }



}

void print_maze(int** maze, int rows, int cols)
{
    for (auto row = 0; row < rows; row++)
    {
        for (auto col = 0; col < cols; col++)
        {
            cout << maze[row][col] << " ";
        }
        cout << endl;
    }
}


maze_result_t check_maze(int** maze, int rows, int cols)
{
    maze_result_t result = NOT_SOLVED;

    /* Для проверки напечатаем лабиринт */
    print_maze(maze, rows, cols);

    //  change value

    return result;
}


int main()
{
    /* initialize random seed: */
    srand (time(NULL));
    
    // Создадим лабиринт и выделим для него память
    int rows = 3; 
    int cols = 3;
    int** maze;

    maze = new int*[rows];
    for (size_t row = 0; row < rows; row++)
    {
        maze[row] = new int[cols];
    }

    /* Заполним лабиринт случайными 0 и 1 */
    fill_maze_by_random_numbers(maze, rows, cols);
    
    /* Oтметим обязательные точки входа и выхода */
    set_entrance_and_exit(maze, rows, cols);

    /* Передадим лабиринт в функцию проверки */
    check_maze(maze, rows, cols);
    
    return 0;
}

// Алгоритм Дейкстры
// Как передать двумерный массив в функцию
