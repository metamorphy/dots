//----------------------------------------------------------------------------------------
// Defines.h
//----------------------------------------------------------------------------------------
#define __UDEFINES__

#define kBorder			25		// pixel offset for playing grid
#define kBoxWidth		20		// pixel distance between adjacent dots
#define kNumBlinks		 5		// the number of times a new edge will flash when it appears
#define kMaxX			11		// maximum possible number of columns
#define kMaxXPlusOne	kMaxX+1
#define kMaxY			11		// maximum possible number of rows
#define kMaxYPlusOne	kMaxY+1

enum {kLeft = 0, kUp, kRight, kDown} Direction;


void ScreenLoc(int x, int y, int *i, int *j);