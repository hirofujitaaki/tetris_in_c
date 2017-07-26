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
#define WHITE 7
#define DEFAULT 9


#define setAttribute(n) printf("\033[%dm",(n))
#define NORMAL 0
#define REVERSE 7

typedef struct _square {
    char c;
    int charcolor;
    int backcolor;
    int attribute;
} Square;

int wait(int msec);
void initialize(void);
void reset(void);
int checkRange(Square squ, int x, int y);
int printSquare(Square squ, int x, int y);
int clearSquare(Square squ, int x, int y);

int main(int argc, char *argv[])
{
    int y;
    Square squ = {' ', WHITE, BLACK, REVERSE};
    initialize();

    for (y=1; y<HEIGHT; y++)
    {
        printSquare(squ, 5, y);
        wait(250);
        clearSquare(squ, 5, y);

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

int checkRange(Square squ, int x, int y)
{
    if(squ.c=='\0' || x<0 || y<0 || x>=WIDTH || y>=HEIGHT )
        return -1;  // False
    else
        return 0; // True
}

int printSquare(Square squ, int x, int y)
{
    if(checkRange(squ, x, y) == -1)
        return -1;
    setPosition(x, y);
    setAttribute(squ.attribute);
    setBackColor(squ.backcolor);
    setCharColor(squ.charcolor);
    printf("%c%c", squ.c, squ.c);
    fflush(stdout);
    return 0;
}

int clearSquare(Square c, int x, int y)
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
