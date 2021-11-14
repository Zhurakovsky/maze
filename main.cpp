#include <iostream>
#include <queue>

#include <cstdlib>     /* srand, rand */
#include <ctime>       /* time */

using namespace std;

namespace
{
    /* ЭТО ЕДИНАЯ ТОЧКА, где мы ЗАДАЕМ ЖЕЛАЕМЫЕ размеры лабиринта.
    Нужно помнить, что чем больше размеры, тем сложнее (реже) будет получить проходимый лабиринт, так как мы заполняем его случайными числами  */
    const int ROWS = 3; 
    const int COLS = 3;
};


enum maze_result_t
{
    SOLVED = 0,
    NOT_SOLVED,
    INVALID_PARAM
};


struct Cell
{
    int val;
    int row_index; // Индекс ряда    от 0 до [rows-1]
    int col_index; // Индекс колонки от 0 до [cols-1]
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


int check_top_neighbor(int** maze, int row, int col)
{
    /* Проверим допустимые границы */
    if (row == 0)
    {
        return 0;
    }

    return maze[row-1][col];
}


int check_left_neighbor(int** maze, int row, int col)
{
    if (col == 0)
    {
        return 0;
    }

    return maze[row][col-1];
}


int check_bottom_neighbor(int** maze, int row, int col)
{
    if (row == (ROWS - 1))
    {
        return 0;
    }

    return maze[row + 1][col];
}


int check_right_neighbor(int** maze, int row, int col)
{
    if (col == (COLS - 1))
    {
        return 0;
    }

    return maze[row][col+1];
}



maze_result_t check_maze(int** maze, int rows, int cols)
{
    maze_result_t result = NOT_SOLVED;

    if ((!maze) || (rows < 2) || (cols < 2))
    {
        return INVALID_PARAM;
    }

    /* Для проверки напечатаем лабиринт */
    print_maze(maze, rows, cols);

    /* Создаем очередь, в которую будем помещать валидные (непосещенные клетки со значением 1) объекты-клетки */
    std::queue<Cell> cells{}; // очередь пока пуста

    /* Левая верхнаяя клетка всегда валидна, поскольку мы ей принудительно ставим 1. И посещенных клеток у нас пока что нету. Так что создадим наш объект первой клетки и поместим его в очередь */

    Cell cell;
    cell.val = maze[0][0]; // Это поле по нашим условиям всегда вход и равно 1
    cell.row_index = 0;    // Первый ряд
    cell.col_index = 0;    // Первая колонка
    
    cells.push(cell);

    /*
    Теперь у нас есть очередь, в которой есть первая клетка.
    Теперь в цикле будем:
      - извлекать элементы из очереди
      - помечать в матрице нашего лабиринта клетку извлеченного элемента как посещенную (9)
      - проверять соседей этой клетки:
            -- Если сосед является выходом из лабиринта - завершаем выполнение и цикла и программы. 
            -- Если сосед не является выходом, но подходящий (1) - создадим объект клетки соседа и поместим в ту же очередь.

      В итоге - мы или найдем выход, или кончатся элементы в очереди, что означает, что лабиринт не имеет решения. Значит возвращаем соответствующий результат и завершаем программу.
    */

   while(!cells.empty())
   {
       /* Создали элемент из первого элемента очереди */
       Cell extracted_front_cell = cells.front();   
       
       /* Проверили, что данные нормально скопировались. */
       cout << "Extracted data: val = " << extracted_front_cell.val
        << ", row_index = " << extracted_front_cell.row_index 
        << ", col_index = " << extracted_front_cell.col_index << endl; 

        /* Не забыли удалить первый элемент, чтобы очередь продвинулась. */
        cells.pop();

        /* Еесли извлеченная клетка выход - успешно выходим из функции и из программы */
        if (extracted_front_cell.val == 3)
        {
            result = SOLVED;
            return result;
        }

        /* Пометим клетку в нашем лабиринте как посещенную (9). Не забываем, что помечаем клеточку в исходном лабиринте. Но данные о посещенной клетке - это данные из объекта, извлеченного из очереди. Тут важно не запутаться. */
        maze[extracted_front_cell.row_index][extracted_front_cell.col_index] = 9;

        /* Для уверенности можем напечатать наш лабиринт с внесенными изменениями */
        print_maze(maze, rows, cols);

        /* Проверим верхнего соседа. Есть он или нет - проверит функция, которая вернет значение верхней (если она есть) клетки в лабиринте. Если это выход, значит выходим из функции и из программы. В вызываемую функцию передадим координаты нашей текущей клетки. Которую извлекли и с которой работаем. */

        int top_val = check_top_neighbor(maze,
                                        extracted_front_cell.row_index,
                                        extracted_front_cell.col_index);
        switch(top_val)
        {
            case 3:
            result = SOLVED;
            return result;
            // break;  /* break тут ставить по правилам нужно, но бессмыссленно. Мы вышли из функции и дальше программа не пойдет */

            case 1:
            /* Наш случай. Нужно из верхнего соседа создать объект клетки и поместить в очередь. Насчет выхода за границы массива можно не переживать. Функция это проверила */
            Cell cell;
            cell.val = maze[extracted_front_cell.row_index - 1][extracted_front_cell.col_index];
            cell.row_index = (extracted_front_cell.row_index - 1);
            cell.col_index = extracted_front_cell.col_index;
            
            cells.push(cell);
            break;

            default:
            /* Тут любые другие значения клеток лабиринта (0 или 9). Не делаем с ними ничего */
            break;
        }

        /* 
        Дальше по аналогии. Проверяем левого, нижнего и правого соседей. Тут я проверяю соседей против часовой стрелки. Можно проверять в любом порядке.
        
        ВАЖНО: тут есть полно возможностей оптимизировать и сокращать количество кода. Я не делаю этого просто для наглядности.
        */

        int left_val = check_left_neighbor(maze,
                                        extracted_front_cell.row_index,
                                        extracted_front_cell.col_index);
        switch(left_val)
        {
            case 3:
                result = SOLVED;
                return result;
                // break;

            case 1:
                Cell cell;
                cell.val = maze[extracted_front_cell.row_index][extracted_front_cell.col_index - 1];
                cell.row_index = (extracted_front_cell.row_index);
                cell.col_index = (extracted_front_cell.col_index - 1);
                
                cells.push(cell);
                break;

            default:
                break;
        }

        int bottom_val = check_bottom_neighbor(maze,
                                        extracted_front_cell.row_index,
                                        extracted_front_cell.col_index);
        switch(bottom_val)
        {
            case 3:
                result = SOLVED;
                return result;
                //break;

            case 1:  
                Cell cell;
                cell.val = maze[extracted_front_cell.row_index + 1][extracted_front_cell.col_index];
                cell.row_index = (extracted_front_cell.row_index + 1);
                cell.col_index = (extracted_front_cell.col_index);
                
                cells.push(cell);
                break;

            default:
                break;
        }

        int right_val = check_right_neighbor(maze,
                                        extracted_front_cell.row_index,
                                        extracted_front_cell.col_index);
        switch(right_val)
        {
            case 3:
                result = SOLVED;
                return result;
                // break;

            case 1:
                Cell cell;
                cell.val = maze[extracted_front_cell.row_index][extracted_front_cell.col_index + 1];
                cell.row_index = (extracted_front_cell.row_index);
                cell.col_index = (extracted_front_cell.col_index + 1);
                
                cells.push(cell);
                break;

            default:
                break;
        }
    }

    /* В принципе - все. Мы или нашли выход и уже успешно вышли из функции где-то раньше, или пришли сюда. Это значит, что лабиринт не имеет решения и мы возвращаем значение NOT_SOLVED, как его в самом начале и установили */

    return result;
}


int main()
{
    /* initialize random seed: */
    srand (time(NULL));
    
    // Создадим лабиринт и выделим для него память
    int** maze;
    maze = new int*[ROWS];
    for (size_t row = 0; row < ROWS; row++)
    {
        maze[row] = new int[COLS];
    }

    // Testcase 1 // Проходной

    maze[0][0] = 0;
    maze[0][1] = 1;
    maze[0][2] = 1;
    maze[1][0] = 0;
    maze[1][1] = 1;
    maze[1][2] = 0;
    maze[2][0] = 0;
    maze[2][1] = 1;
    maze[2][2] = 0;
    
    /*
    0 1 1
    0 1 0
    0 1 0
    */
    
    /* Oтметим обязательные точки входа и выхода */
    set_entrance_and_exit(maze, ROWS, COLS);

    maze_result_t expected_result = SOLVED;
    maze_result_t tested_result = check_maze(maze, ROWS, COLS);

    if (expected_result == tested_result)
    {
        cout << "Test 1 passed" << endl;
    }
    else
    {
        cout << "Test 1 not passed" << endl;
    }
    
    /* End of Testcase 1 */

    /* Testcase 2. Unable to pass */
    maze[0][0] = 0;
    maze[0][1] = 1;
    maze[0][2] = 1;
    maze[1][0] = 0;
    maze[1][1] = 0;
    maze[1][2] = 0;
    maze[2][0] = 0;
    maze[2][1] = 1;
    maze[2][2] = 0;
    
    /*
    0 1 1
    0 0 0
    0 1 0
    */
    
    /* Oтметим обязательные точки входа и выхода */
    set_entrance_and_exit(maze, ROWS, COLS);

    expected_result = NOT_SOLVED;
    tested_result = check_maze(maze, ROWS, COLS);

    if (expected_result == tested_result)
    {
        cout << "Test 2 passed" << endl;
    }
    else
    {
        cout << "Test 2 not passed" << endl;
    }
    
    /* End of Testcase 2 */

    /* Testcase 3. Nullptr */
    expected_result = INVALID_PARAM;
    tested_result = check_maze(nullptr, ROWS, COLS);

    if (expected_result == tested_result)
    {
        cout << "Test 3 passed" << endl;
    }
    else
    {
        cout << "Test 3 not passed" << endl;
    }

    /* End of Testcase 3 */

    /* Testcase 4. Invalid size */

    maze[0][0] = 0;
    maze[0][1] = 1;
    maze[0][2] = 1;
    /*
    0 1 1
    */
    expected_result = INVALID_PARAM;
    tested_result = check_maze(nullptr, 1, COLS);

    if (expected_result == tested_result)
    {
        cout << "Test 4 passed" << endl;
    }
    else
    {
        cout << "Test 4 not passed" << endl;
    }
    /* End of Testcase 4 */

    /* Заполним лабиринт случайными 0 и 1 */
    fill_maze_by_random_numbers(maze, ROWS, COLS);
    
    /* Oтметим обязательные точки входа и выхода */
    // set_entrance_and_exit(maze, ROWS, COLS);

    // /* Проверим, имеет ли лабиринт решение и напишем соответствующий ответ. */
    // if (check_maze(maze, ROWS, COLS) == SOLVED)
    // {
    //     cout << "MAZE SOLVED!" << endl;
    // }
    // else
    // {
    //     cout << "MAZE NOT SOLVED!" << endl;         
    // }

    /* Мы работали с ручным выделением памяти. Не забываем очистить ее после использования */
    for (size_t row = 0; row < ROWS; row++)
    {
        delete [] maze[row];
    }

    delete [] maze;
    maze = nullptr;

    return 0;
}
