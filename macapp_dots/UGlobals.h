//----------------------------------------------------------------------------------------
// UGlobals.h
//----------------------------------------------------------------------------------------

#ifndef __UGLOBALS__
#define __UGLOBALS__

#ifndef __UDEFINES__
#include "UDefines.h"
#endif

// gEdge[x,y,d] is true iff box x,y has an edge in direction d
extern Boolean gEdge[kMaxXPlusOne][kMaxYPlusOne][4];

extern int gBoardSizeX;
extern int gBoardSizeY;
#endif