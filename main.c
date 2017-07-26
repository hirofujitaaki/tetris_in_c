#include <stdio.h>
#include <time.h>

#define clearScreen() printf("\033[2J")
#define setPosition(x,y) printf("\033[%d;%dH",(y),(x))
#define setCharColor(n) printf("\033[3%dm",(n))
#define setBackColor(n) printf("\033[4%dm",(n))
#define setAttribute(n) printf("\033[%dm",(n))

#define BLACK 0
#define RED 1
#define WHITE 7
#define DEFAULT 9

#define NORMAL 0
#define REVERSE 7



int wait(int msec);

int main(int argc, char *argv[])
{
    int y;
    clearScreen();
    for (y=1; y<20; y++)
    {
        setPosition(5, y);
        setCharColor(WHITE);
        setBackColor(BLACK);
        setAttribute(REVERSE);
        printf("  ");
        fflush(stdout);

        wait(500);

        setPosition(5, y);
        setCharColor(WHITE);
        setBackColor(BLACK);
        setAttribute(NORMAL);
        printf("  ");
        fflush(stdout);
    }

    setBackColor(DEFAULT);
    setCharColor(DEFAULT);
}

int wait(int msec)
{
    struct timespec r = {0, msec * 1000L * 1000L};
    return nanosleep( &r, NULL);
}
