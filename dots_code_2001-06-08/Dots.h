#if !defined(__DOTS__)
#define __DOTS__

#if defined(__cplusplus)
extern "C" {
#endif

#ifndef USING_WINDOWS
	#include <MacTypes.h>
	#include <MacWindows.h>
#endif

typedef struct Dot {
	short row;		/* row number of dot, 0..boardSize-1 */
	short col;		/* column number of dot, 0..boardSize-1 */
} Dot;

typedef struct DotLine {
	Dot dot1;		/* first dot of a line */
	Dot dot2;		/* second dot of a line */
	/*	legal lines are formed by dots in the same row, in adjacent columns, 
		or in the same column in adjacent rows */
} DotLine;			

void InitDots(
	short boardSize,	/* number of dots per row/col in board */
	Boolean playFirst,	/* true if you play first, false of opponent plays first */
	WindowPtr dotWindow		/* color window where you should draw game results */
);

void OpponentMove(
	const DotLine opponentLine	/* line formed by your opponent on previous move */
);

short /* number of lines generated */ PlayDots(
	DotLine yourLines[]			/* return the lines you form here */
);

void TermDots(void);		/* return any storage you allocated */

#if defined(__cplusplus)
}
#endif

#endif