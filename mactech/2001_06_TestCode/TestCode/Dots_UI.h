#include "Dots.h"

void InitDotsUserGame(short boardSize, Boolean playfirst, WindowPtr wdw) ;
void TermDotsUserGame(void);
void DrawDotsUserOpponentMove (DotLine opponentMove);
short ProcessDotsUserEvent(EventRecord *ev, DotLine *yourLines, short *cellsWon, Boolean *validMove);


