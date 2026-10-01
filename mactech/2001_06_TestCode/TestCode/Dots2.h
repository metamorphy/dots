#include "Dots.h"

#if !defined(__DOTS2__)
#define __DOTS2__

#if defined(__cplusplus)
extern "C" {
#endif

#include <MacTypes.h>
#include <MacWindows.h>

void InitDots02(
	short boardSize,	/* number of dots per row/col in board */
	Boolean playFirst,	/* true if you play first, false of opponent plays first */
	WindowPtr dotWindow		/* color window where you should draw game results */
);

void OpponentMove02(
	const DotLine opponentLine	/* line formed by your opponent on previous move */
);

short /* number of lines generated */ PlayDots02(
	DotLine yourLines[]			/* return the lines you form here */
);

void TermDots02(void);		/* return any storage you allocated */

#if defined(__cplusplus)
}
#endif

#endif