//----------------------------------------------------------------------------------------
// UGlobals.cp
//----------------------------------------------------------------------------------------

#ifndef __UDEFINES__
#include "UDefines.h"
#endif

// gEdge[x,y,d] is true iff box x,y has an edge in direction d
Boolean gEdge[kMaxXPlusOne][kMaxYPlusOne][4];

int	gBoardSizeX = 10;
int gBoardSizeY = 10;

void ScreenLoc(int x, int y, int *i, int *j)
// Given box coordinates x,y returns coords of top-left point of box (i,j)
{
	*i = kBorder + x * kBoxWidth;
	*j = kBorder + y * kBoxWidth;
}

