#include <stdio.h>
#include <time.h>

#define clearScreen() printf("\033[2J")
#define setPosition(x,y) printf("\033[%d;%dH",(y)+1, (x)*2+1)  //adjustment for indexing
#define cursolOn() printf("\033[?25h")  // displays the cursor
#define cursolOff() printf("\033[?25l")  //hides the cursor
#define WIDTH 10
#define HEIGHT 20

#define setCharColor(n) printf("\033[3%dm",(n))
#define setBackColor(n) printf("\033[4%dm",(n))
#define BLACK 0
#define RED 1
#define GREEN 2
#define YELLOW 3
#define BLUE 4
#define MAGENTA 5
#define CYAN 6
#define WHITE 7
#define DEFAULT 9

#define setAttribute(n) printf("\033[%dm",(n))
#define NORMAL 0
#define BLIGHT 1
#define DIM 2
#define UNDERBAR 4
#define BLINK 5
#define REVERSE 7
#define HIDE 8
#define STRIKE 9

typedef struct cell {
    char c;
    int charcolor;
    int backcolor;
    int attribute;
} Cell;

#define BLOCK_SIZE 4
#define BLOCK_NUM 7
Cell block_type[BLOCK_NUM][BLOCK_SIZE][BLOCK_SIZE] = {
  '\0', RED, BLACK, NORMAL,
  '\0', RED, BLACK, NORMAL,
  ' ', RED, RED, REVERSE,
  '\0', RED, BLACK, NORMAL,
  '\0', RED, BLACK, NORMAL,
  ' ', RED, RED, REVERSE,
  ' ', RED, RED, REVERSE,
  '\0', RED, BLACK, NORMAL,
  '\0', RED, BLACK, NORMAL,
  ' ', RED, RED, REVERSE,
  '\0', RED, BLACK, NORMAL,
  '\0', RED, BLACK, NORMAL,
  '\0', RED, BLACK, NORMAL,
  '\0', RED, BLACK, NORMAL,
  '\0', RED, BLACK, NORMAL,
  '\0', RED, BLACK, NORMAL,

  '\0', BLUE, BLACK, NORMAL,
  ' ', BLUE, BLACK, REVERSE,
  ' ', BLUE, BLACK, REVERSE,
  '\0', BLUE, BLACK, NORMAL,
  '\0', BLUE, BLACK, NORMAL,
  ' ', BLUE, BLACK, REVERSE,
  '\0', BLUE, BLACK, NORMAL,
  '\0', BLUE, BLACK, NORMAL,
  '\0', BLUE, BLACK, NORMAL,
  ' ', BLUE, BLACK, REVERSE,
  '\0', BLUE, BLACK, NORMAL,
  '\0', BLUE, BLACK, NORMAL,
  '\0', BLUE, BLACK, NORMAL,
  '\0', BLUE, BLACK, NORMAL,
  '\0', BLUE, BLACK, NORMAL,
  '\0', BLUE, BLACK, NORMAL,

  '\0', GREEN, BLACK, NORMAL,
  '\0', GREEN, BLACK, NORMAL,
  '\0', GREEN, BLACK, NORMAL,
  '\0', GREEN, BLACK, NORMAL,
  '\0', GREEN, BLACK, NORMAL,
  ' ', GREEN, BLACK, REVERSE,
  ' ', GREEN, BLACK, REVERSE,
  '\0', GREEN, BLACK, NORMAL,
  ' ', GREEN, BLACK, REVERSE,
  ' ', GREEN, BLACK, REVERSE,
  '\0', GREEN, BLACK, NORMAL,
  '\0', GREEN, BLACK, NORMAL,
  '\0', GREEN, BLACK, NORMAL,
  '\0', GREEN, BLACK, NORMAL,
  '\0', GREEN, BLACK, NORMAL,
  '\0', GREEN, BLACK, NORMAL,

  '\0', YELLOW, BLACK, NORMAL,
  '\0', YELLOW, BLACK, NORMAL,
  '\0', YELLOW, BLACK, NORMAL,
  '\0', YELLOW, BLACK, NORMAL,
  '\0', YELLOW, BLACK, NORMAL,
  ' ', YELLOW, BLACK, REVERSE,
  ' ', YELLOW, BLACK, REVERSE,
  '\0', YELLOW, BLACK, NORMAL,
  '\0', YELLOW, BLACK, NORMAL,
  ' ', YELLOW, BLACK, REVERSE,
  ' ', YELLOW, BLACK, REVERSE,
  '\0', YELLOW, BLACK, NORMAL,
  '\0', YELLOW, BLACK, NORMAL,
  '\0', YELLOW, BLACK, NORMAL,
  '\0', YELLOW, BLACK, NORMAL,
  '\0', YELLOW, BLACK, NORMAL,

  '\0', CYAN, BLACK, NORMAL,
  '\0', CYAN, BLACK, NORMAL,
  '\0', CYAN, BLACK, NORMAL,
  '\0', CYAN, BLACK, NORMAL,
  ' ', CYAN, BLACK, REVERSE,
  ' ', CYAN, BLACK, REVERSE,
  ' ', CYAN, BLACK, REVERSE,
  ' ', CYAN, BLACK, REVERSE,
  '\0', CYAN, BLACK, NORMAL,
  '\0', CYAN, BLACK, NORMAL,
  '\0', CYAN, BLACK, NORMAL,
  '\0', CYAN, BLACK, NORMAL,
  '\0', CYAN, BLACK, NORMAL,
  '\0', CYAN, BLACK, NORMAL,
  '\0', CYAN, BLACK, NORMAL,
  '\0', CYAN, BLACK, NORMAL,

  '\0', MAGENTA, BLACK, NORMAL,
  '\0', MAGENTA, BLACK, NORMAL,
  '\0', MAGENTA, BLACK, NORMAL,
  '\0', MAGENTA, BLACK, NORMAL,
  ' ', MAGENTA, BLACK, REVERSE,
  ' ', MAGENTA, BLACK, REVERSE,
  ' ', MAGENTA, BLACK, REVERSE,
  '\0', MAGENTA, BLACK, NORMAL,
  '\0', MAGENTA, BLACK, NORMAL,
  ' ', MAGENTA, BLACK, REVERSE,
  '\0', MAGENTA, BLACK, NORMAL,
  '\0', MAGENTA, BLACK, NORMAL,
  '\0', MAGENTA, BLACK, NORMAL,
  '\0', MAGENTA, BLACK, NORMAL,
  '\0', MAGENTA, BLACK, NORMAL,
  '\0', MAGENTA, BLACK, NORMAL,

  '\0', WHITE, BLACK, NORMAL,
  ' ', WHITE, BLACK, REVERSE,
  '\0', WHITE, BLACK, NORMAL,
  '\0', WHITE, BLACK, NORMAL,
  '\0', WHITE, BLACK, NORMAL,
  ' ', WHITE, BLACK, REVERSE,
  '\0', WHITE, BLACK, NORMAL,
  '\0', WHITE, BLACK, NORMAL,
  '\0', WHITE, BLACK, NORMAL,
  ' ', WHITE, BLACK, REVERSE,
  ' ', WHITE, BLACK, REVERSE,
  '\0', WHITE, BLACK, NORMAL,
  '\0', WHITE, BLACK, NORMAL,
  '\0', WHITE, BLACK, NORMAL,
  '\0', WHITE, BLACK, NORMAL,
  '\0', WHITE, BLACK, NORMAL,
};

int wait(int msec);
void initialize(void);
void reset(void);
int checkRange(Cell a, int x, int y);
int printCell(Cell c, int x, int y);
int clearCell(Cell c, int x, int y);
void copyBlock(Cell src[BLOCK_SIZE][BLOCK_SIZE], Cell dst[BLOCK_SIZE][BLOCK_SIZE]);
int printBlock(Cell block[BLOCK_SIZE][BLOCK_SIZE], int x, int y);
int clearBlock(Cell block[BLOCK_SIZE][BLOCK_SIZE], int x, int y);

int main(int argc, char *argv[])
{
    int i;
    Cell block[BLOCK_SIZE][BLOCK_SIZE];
    copyBlock(block_type[1], block);
    initialize();

    for (i=0; i<HEIGHT; i++)
    {
        printBlock(block, 5, i);
        wait(250);
        clearBlock(block, 5, i);

    }
    reset();
}


int wait(int msec)
{
    struct timespec r = {0, msec * 1000L * 1000l};
    return nanosleep( &r, NULL);
}

void initialize(void)
{
    setAttribute(NORMAL);
    setBackColor(BLACK);
    setCharColor(WHITE);
    clearScreen();
    cursolOff();
}

void reset(void)
{
    setBackColor(BLACK);
    setCharColor(WHITE);
    setAttribute(NORMAL);
    clearScreen();
    cursolOn();
}

int checkRange(Cell a, int x, int y)
{
    if(a.c=='\0' || x<0 || y<0 || x>=WIDTH || y>=HEIGHT )
        return -1;  // False
    else
        return 0; // True
}

int printCell(Cell c, int x, int y)
{
    if(checkRange(c, x, y) == -1)
        return -1;
    setPosition(x, y);
    setAttribute(c.attribute);
    setBackColor(c.backcolor);
    setCharColor(c.charcolor);
    printf("%c%c", c.c, c.c);
    fflush(stdout);
    return 0;
}

int clearCell(Cell c, int x, int y)
{
    if(checkRange(c, x, y) == -1)
        return -1;
    setPosition(x, y);
    setBackColor(BLACK);
    setCharColor(WHITE);
    setAttribute(NORMAL);
    printf("  ");
    fflush(stdout);
    return 0;
}


void copyBlock(Cell src[BLOCK_SIZE][BLOCK_SIZE], Cell dst[BLOCK_SIZE][BLOCK_SIZE])
{
    int i, j;
    for (j=0; j<BLOCK_NUM; j++)
        for (i=0; i<BLOCK_NUM; i++)
            dst[j][i]=src[j][i];
}

int printBlock(Cell block[BLOCK_SIZE][BLOCK_SIZE], int x, int y)
{
    int i, j;
    for (j = 0; j < BLOCK_SIZE; j++)
        for (i = 0; i < BLOCK_SIZE; i++)
            printCell(block[j][i], i+x, j+y);
    return 0;
}

int clearBlock(Cell block[BLOCK_SIZE][BLOCK_SIZE], int x, int y)
{
    int i, j;
    for (j = 0; j <BLOCK_SIZE; j++)
        for (i = 0; i < BLOCK_SIZE; i++)
            clearCell(block[j][i], i+x, j+y);
    return 0;
}
