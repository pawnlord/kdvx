#include <termios.h>
#include <stdio.h>
#define startbuff() printf("\033[?1049h\033[H")
#define clear() printf("\033[H\033[J")
#define clearline() printf("\033[H\033[2K")
#define gotoxy(x, y) printf("\033[%d;%dH", (y), (x))
#define godownx(x) printf("\033[%dB", (x))
#define goupx(x) printf("\033[%dA", (x))
#define goleftx(x) printf("\033[%dD", (x))
#define gorightx(x) printf("\033[%dC", (x))
#define delete(x) printf("\033[%dD \033[%dD", 1, 1)
#define display() printf("\n")
#define endbuff() printf("\033[?1049l")

static struct termios old, new;

/* Initialize new terminal i/o settings */
void initTermios(int echo);

/* Restore old terminal i/o settings */
void resetTermios(void);

/* Read 1 character - echo defines echo mode */
int getch_(int echo);

/* 
Read 1 character without echo 
getch() function definition.
*/
int getch(void);

/* 
Read 1 character with echo 
getche() function definition.
*/
int getche(void);

void message(int width, int height, char* message);

/* take already defined string and add input*/
char input(char* s);