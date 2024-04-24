#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#define  BOARD '.'
#define EMPTY ' '
#define CREATURE '@'
#define GAME_PAIR 1
#define CREATURE_PAIR 4
#define EMPTY_PAIR 10
#define ROWS 160
#define COLS 160
typedef struct
{
    int x, y;
} Creature;
struct cell

{
    int current;

    int future;
};

void inital_creatures(struct cell board[ROWS][COLS]);
void update_game(struct cell board[ROWS][COLS]);
int count_Neighbors(struct cell board[ROWS][COLS], int row, int col);

int main(void)
{
    int y, x;
    int ch;
    struct cell board[ROWS][COLS] = {0, 0};

    /* initialize curses */
    initscr();
    keypad(stdscr, TRUE);
    cbreak();
    noecho();

    /* initialize colors */
    if (has_colors() == FALSE)
    {
        endwin();
        printf("Your terminal does not support color\n");
        exit(1);
        
    }

    start_color();
    // board color

    // creature color
    init_pair(CREATURE_PAIR, COLOR_WHITE, COLOR_GREEN);
    
    clear();

    inital_creatures(board);
    do
    {
        update_game(board);

        refresh();
        //  Delay for 500 milliseconds
        Sleep(1000);
        // ch = getch();

    } while ((ch != 'q') && (ch != 'Q'));

    endwin();

    exit(0);
}

void inital_creatures(struct cell board[LINES][COLS])
{
    int i, j;
    int start_y = 0;
    // starting y-coordinate of the desert area
    int start_x = 0;
    // starting x-coordinate of the desert area

    for (i = start_y; i < LINES; i++)
    {
        for (j = start_x; j < COLS; j++)
        {
            board[i][j].current = rand() % 2;
        }
    }
}
// update dessert function
void update_game(struct cell board[LINES][COLS])
{
    int i, j;
    int neighbors;
    int start_y = 0;
    // starting x-coordinate of the desert area
    int start_x = 0;
    // starting y-coordinate of the desert area

    for (i = start_y; i < LINES; i++)
    {
        for (j = start_x; j < COLS; j++)
        {
            neighbors = count_Neighbors(board, i, j);

            // Loneliness
            if ((board[i][j].current == 1) && (neighbors < 2))
            {
                board[i][j].future = 0; // cell dies
            }
            // Overpopulation
            else if ((board[i][j].current == 1) && (neighbors > 3))
            {
                board[i][j].future = 0; // cell dies
            }
            // Reproduction
            else if ((board[i][j].current == 0) && (neighbors == 3))
            {
                board[i][j].future = 1; // cell becomes alive
            }
            // else back to current
            else
            {
                board[i][j].future = board[i][j].current;
            }
        }
    }

    // Copy future to current
    for (i = start_y; i < LINES; i++)
    {
        for (j = start_x; j < COLS; j++)
        {
            board[i][j].current = board[i][j].future;
        }
    }

    // Draw the board on the screen
    for (i = start_y; i < LINES; i++)
    {
        for (j = start_x; j < COLS; j++)
        {
            if (board[i][j].current == 1)
            {
                // Draw a creature
                attron(COLOR_PAIR(CREATURE_PAIR));
                mvaddch(i, j, CREATURE);
                attroff(COLOR_PAIR(CREATURE_PAIR));
            }
            else
            {
                // Draw desert
                attron(COLOR_PAIR(GAME_PAIR));
                mvaddch(i, j, BOARD);
                attroff(COLOR_PAIR(GAME_PAIR));
            }
        }
    }

    
}
int count_Neighbors(struct cell board[ROWS][COLS], int row, int col)
{
    int i, j;
    int count = 0;

    // count live neighbors
    for (i = row - 1; i <= row + 1; i++)
    {
        for (j = col - 1; j <= col + 1; j++)
        {
            if (i == row && j == col)
            {
                // skipping  the current cell
                continue;
            }

            if (i < 0 || j < 0 || i >= ROWS || j >= COLS)
            {
                // skip cells that are outside the board
                continue;
            }
            // wrap edges
            int x = (i + ROWS) % ROWS;
            int y = (j + COLS) % COLS;

            // count number of neighbors
            count += board[x][y].current;
        }
    }

    return count;
}