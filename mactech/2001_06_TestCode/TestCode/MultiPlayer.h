#if !defined(__MULTIPLAYER__)
#define __MULTIPLAYER__

#if defined(__cplusplus)
extern "C" {
#endif

#include <MacTypes.h>
#include <MacWindows.h>
#include "Dots.h"

typedef void (InitDotsProc) (
	short boardSize,	/* number of dots per row/col in board */
	Boolean playFirst,	/* true if you play first, false of opponent plays first */
	WindowPtr dotWindow		/* color window where you should draw game results */
);

typedef void (OpponentMoveProc) (
	const DotLine opponentLine	/* line formed by your opponent on previous move */
);

typedef short /* number of lines generated */ (PlayDotsProc) (
	DotLine yourLines[]			/* return the lines you form here */
);

typedef void (TermDotsProc)(void);		/* return any storage you allocated */


typedef struct PlayerData {
	InitDotsProc *initDots;
	OpponentMoveProc *opponentMove;
	PlayDotsProc *playDots;
	TermDotsProc *termDots;
	long playerNumber;
	unsigned long testCaseTime;
	double cumElapsedMilliseconds;
	double testCaseScore;
	double cumScore;
	long testCaseCells;
	long cumCells;
	long testCaseCorrect;
	long cumCorrect;
	Boolean playingFirst;
} PlayerData;


#define PLAYER_PROTOTYPES(N) \
\
InitDotsProc InitDots##N; \
OpponentMoveProc OpponentMove##N; \
PlayDotsProc PlayDots##N; \
TermDotsProc TermDots##N; \

#if defined(__cplusplus)
}
#endif

#endif