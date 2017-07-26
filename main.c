#include <stdio.h>
#include <time.h>

#define clearScreen() printf("\033[2J")
#define setPosition(x,y) printf("\033[%d;%dH",(y)+1, (x)*2+1)  //adjustment for indexing
#define setCharColor(n) printf("\033[3%dm",(n))
#define setBackColor(n) printf("\033[4%dm",(n))
#define setAttribute(n) printf("\033[%dm",(n))
#define cursolOn() printf("\033[?25h")  // displays the cursor
#define cursolOff() printf("\033[?25l")  //hides the cursor

#define BLACK 0
#define RED 1
#define WHITE 7
#define DEFAULT 9

/* for setAttribute() */
#define NORMAL 0
#define REVERSE 7

#define WIDTH 10
#define HEIGHT 20



int wait(int msec);

int main(int argc, char *argv[])
{
    int y;
    clearScreen();
    cursolOff();
    for (y=1; y<20; y++)
    {
        setPosition(5, y);
        setCharColor(WHITE);
        setBackColor(BLACK);
        setAttribute(REVERSE);
        printf("  ");
        fflush(stdout);

        wait(250);

        setPosition(5, y);
        setCharColor(WHITE);
        setBackColor(BLACK);
        setAttribute(NORMAL);
        printf("  ");
        fflush(stdout);
    }

    setBackColor(DEFAULT);
    setCharColor(DEFAULT);
    cursolOn();
}

int wait(int msec)
{
    struct timespec r = {0, msec * 1000L * 1000L};
    return nanosleep( &r, NULL);
}
