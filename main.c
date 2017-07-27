#include <stdio.h>
#include <time.h>
#include <sys/time.h>

#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>

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


extern int errno;
struct termios otty,ntty;

int wait_(int msec);
void initialize(void);
void reset(void);
int checkRange(Cell a, int x, int y);
int printCell(Cell c, int x, int y);
int clearCell(Cell c, int x, int y);
void copyBlock(Cell src[BLOCK_SIZE][BLOCK_SIZE], Cell dst[BLOCK_SIZE][BLOCK_SIZE]);
int printBlock(Cell block[BLOCK_SIZE][BLOCK_SIZE], int x, int y);
int clearBlock(Cell block[BLOCK_SIZE][BLOCK_SIZE], int x, int y);
void rotateBlock(Cell src[BLOCK_SIZE][BLOCK_SIZE], Cell dst[BLOCK_SIZE][BLOCK_SIZE]);

int kbhit(void);
int getch(void);
int tinit(void);


int main(int argc, char *argv[])
{
    int x, y, c, prex, prey;
    Cell block[BLOCK_SIZE][BLOCK_SIZE], block_tmp[BLOCK_SIZE][BLOCK_SIZE];
    struct timeval start_time, now_time, pre_time;
    double duration, thold;

    copyBlock(block_type[1], block);
    initialize();
    x=5;
    y=0;
    thold=0.5;
    printBlock(block, x, y);
    gettimeofday(&start_time, NULL);
    pre_time = start_time;

    for(y=0; y<HEIGHT ; )
    {
        prex = x;
        prey = y;

        if(kbhit())
        {
            clearBlock(block, x, y);
            c = getch();
            if(c==0x1b)
            {
                c = getch();
                if(c==0x5b)
                {
                    c = getch();
                    switch(c)
                    {
                        case 0x41:  //UP
                            rotateBlock(block, block_tmp);
                            clearBlock(block, x, y);  //clear the previous
                            printBlock(block_tmp, x, y);  //display the rotated block
                            copyBlock(block_tmp, block);  //over-wirte the previous position
                            break;
                        case 0x42:  //DOWN
                            break;
                        case 0x43:  //RIGHT
                            x++;
                            break;
                        case 0x44:  //LEFT
                            x--;
                            break;
                    }
                }
            }
            else
            {
                reset();
                exit(1);
            }
        }
        gettimeofday(&now_time, NULL);
        duration = now_time.tv_sec - pre_time.tv_sec
            + (now_time.tv_usec - pre_time.tv_usec)/1000000.0;
        if (duration > thold)
        {
            pre_time = now_time;
            y++;
        }
        if (prex != x || prey != y)
        {
            clearBlock(block, prex, prey);
            printBlock(block, x ,y);
        }
    }
    reset();
}

int wait_(int msec)
{
    struct timespec r = {0, msec * 1000L * 1000l};
    return nanosleep( &r, NULL);
}

void initialize(void)
{
    tinit();
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
    tcsetattr(1, TCSADRAIN, &otty);
    write(1, "\n", 1);
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

int kbhit(void)
{
  int ret;
  fd_set rfd;
  struct timeval timeout = {0,0};
  FD_ZERO(&rfd);
  FD_SET(0, &rfd);
  ret = select(1, &rfd, NULL, NULL, &timeout);
  if (ret == 1)
    return 1;
  else
    return 0;
}

int getch(void)
{
  unsigned char c;
  int n;
  while ((n = read(0, &c, 1)) < 0 && errno == EINTR) ;
  if (n == 0)
    return -1;
  else
    return (int)c;
}

static void onsignal(int sig)
{
  signal(sig, SIG_IGN);
  switch(sig){
    case SIGINT:
    case SIGQUIT:
    case SIGTERM:
    case SIGHUP:
     exit(1);
     break;
  }
}

int tinit(void)
{
  if (tcgetattr(1, &otty) < 0)
    return -1;
  ntty = otty;
  ntty.c_iflag &= ~(INLCR|ICRNL|IXON|IXOFF|ISTRIP);
  ntty.c_oflag &= ~OPOST;
  ntty.c_lflag &= ~(ICANON|ECHO);
  ntty.c_cc[VMIN] = 1;
  ntty.c_cc[VTIME] = 0;
  tcsetattr(1, TCSADRAIN, &ntty);
  signal(SIGINT, onsignal);
  signal(SIGQUIT, onsignal);
  signal(SIGTERM, onsignal);
  signal(SIGHUP, onsignal);
  return 0;
}

void rotateBlock(Cell src[BLOCK_SIZE][BLOCK_SIZE], Cell dst[BLOCK_SIZE][BLOCK_SIZE])
{
    int i, j;
    for (j=0; j<BLOCK_SIZE; j++)
        for (i=0; i<BLOCK_SIZE; i++)
            dst[i][BLOCK_SIZE-1 - j] = src[j][i];
}
