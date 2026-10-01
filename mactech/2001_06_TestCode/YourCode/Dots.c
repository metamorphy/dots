#include "Dots.h"
#include <LowMem.h>

#include <assert.h>
#include <stdlib.h>

/* Replace this file with your code */

/*
 *  This player selects a random unplayed line.
 *  It does not make another move when a square has been filled.
 */

static short gBoardSize;
static WindowPtr gWindow;

static short moveCounter;
static char *gLinePlayed;

#define VERTICAL_LINE_OFFSET (gBoardSize * (gBoardSize-1))
#define HORIZ_LINE(row,col) ((row)*(gBoardSize-1) + (col))
#define VERT_LINE(row,col) (VERTICAL_LINE_OFFSET + (row)*(gBoardSize) + (col))

void InitDots(
	short boardSize,	/* number of dots per row/col in board */
	Boolean playFirst,
	WindowPtr wdw		/* color window where you should draw game results */
) {
#pragma unused (playFirst)
	gBoardSize = boardSize;
	gWindow = wdw;
	moveCounter = 0;
	
	assert (0 != (gLinePlayed = NewPtrClear(gBoardSize*(gBoardSize-1)*2)));
	
	srand(LMGetTicks());
}

void OpponentMove(
	DotLine opponentLine
) {
	DotLine dotline = opponentLine;
	if (opponentLine.dot1.row == opponentLine.dot2.row) {
		gLinePlayed[HORIZ_LINE(opponentLine.dot1.row,opponentLine.dot1.col)]=1;
	} else {
		gLinePlayed[VERT_LINE(opponentLine.dot1.row,opponentLine.dot1.col)]=1;
	}
	moveCounter++;
}

short PlayDots(
	DotLine yourLines[]
) {
	short numLines = 0;
	DotLine *line;

	if (moveCounter>=gBoardSize*(gBoardSize-1)*2) return 0;

	Boolean foundMove=false;
	line = &yourLines[numLines];
	do {
		short row = rand()%(gBoardSize);
		short col = rand()%(gBoardSize);
		
		assert(row<gBoardSize);
		assert(col<gBoardSize);
		
		short vert = rand() & 0x0008;
		line->dot1.row = row;
		line->dot1.col = col;
		line->dot2 = line->dot1;
		if (vert==0) {
			if ((line->dot1.col<gBoardSize-1) && (0==gLinePlayed[HORIZ_LINE(row,col)])) {
				line->dot2.col++;
				gLinePlayed[HORIZ_LINE(row,col)]=1;
				foundMove = true;
			}
		} else {
			if ((line->dot1.row<gBoardSize-1) && (0==gLinePlayed[VERT_LINE(row,col)])) {
				line->dot2.row++;
				gLinePlayed[VERT_LINE(row,col)]=1;
				foundMove = true;
			}
		}
	} while (!foundMove);

	moveCounter++;
	numLines++;
	
	return numLines;
}

void TermDots(void) {
	DisposePtr((Ptr)gLinePlayed);
}
