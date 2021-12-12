#include "clio.h"

/* Initialize new terminal i/o settings */
void initTermios(int echo) {
  tcgetattr(0, &old); //grab old terminal i/o settings
  new = old; //make new settings same as old settings
  new.c_lflag &= ~ICANON; //disable buffered i/o
  new.c_lflag &= echo ? ECHO : ~ECHO; //set echo mode
  tcsetattr(0, TCSANOW, &new); //apply terminal io settings
}

/* Restore old terminal i/o settings */
void resetTermios(void) {
  tcsetattr(0, TCSANOW, &old);
}

/* Read 1 character - echo defines echo mode */
int getch_(int echo) {
  int ch;
  initTermios(echo);
  ch = getchar();
  resetTermios();
  return ch;
}

/* 
Read 1 character without echo 
getch() function definition.
*/
int getch(void) {
  return getch_(0);
}

/* 
Read 1 character with echo 
getche() function definition.
*/
int getche(void) {
  return getch_(1);
}

void message(int width, int height, char* message) {
	int length = 0;
	while(message[length+1] !=0)
		length++;
	gotoxy(0, height);
//	int i = 0;
//	while(i < width)
//	{
//		printf(" ");
//		++i;
//	}
	gotoxy(width/2-(length/2), height-1);
	printf("%s", message);
}

char input(char* s){
	char ch = getch();
	int i = 0;	
	switch(ch){
		case 8:
		case 127:
		for(; s[i] != 0; i++);
		if(i != 0){
			s[i-1] = 0;		
		}
		break;
		default:
		for(; s[i] != 0; i++);
		s[i] = ch;
		s[i+1] = 0;
		break;	
	}
	return ch;
}
