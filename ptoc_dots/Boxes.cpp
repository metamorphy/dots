#include "ptoc.h"



/*DOTSNBOXES**********************/
/* A "Dots and Boxes" game for the mac.  Copyright 1986 by Jeff Mallett	    */
/* 							    Last revision 11/14/86							    */
/**/

/*1 -- the computer jumps here when it has found a move*/
/*2 -- the end*/


const integer hodiff = 8;				/*Hand Out Difference: if the number of untaken boxes is less*/
								/*	than HOdiff, then the computer will always take a box*/
								/*	even if hand out possibilities exist.*/
const integer maxx = 11;				/*maximum possible number of columns*/
const integer maxxplusone = 12;
const integer maxy = 11;				/*maximum possible number of rows*/
const integer maxyplusone = 12;
const integer boxwidth = 20;			/*pixel distance between adjacent dots*/
const integer scoreline = 290;			/*distance of scores from top of screen*/
const integer markvalue = -1;			/*temporary marker: when calculating a path a box*/
/*	whose pathsize=markvalue is already on the path*/

enum direction {left, up, right, down, last_direction};
enum who {firstplayer, secondplayer, last_who};
enum kindofplayer {computer, human, last_kindofplayer};
enum pathdirection {behind, ahead, last_pathdirection};

typedef struct boxrecordtype* ptrtoboxrecord;
typedef array<0,last_pathdirection,ptrtoboxrecord> pathnextarray;
struct boxrecordtype {
	integer xcoord, ycoord;			        /*location of box*/
	ptrtoboxrecord previous, next;	        /*previous & next elements on edge-list*/
	integer pathsize;					/*number of elements in the 2-path*/
	pathnextarray pnext;				/*the two connecting 2's in the path*/
};


/*edge[x,y,d] is true iff box x,y has an edge in direction d*/
matrix<0,maxxplusone, 0,maxyplusone, 0,last_direction,boolean> edge;

/*Ptr[x,y] is a pointer to the corresponding box-record on an edge list*/
matrix<1,maxx, 1,maxy,ptrtoboxrecord> ptr;

/*list[n] is a pointer to a doubly-linked list of box-records, all of which have n edges*/
array<0,4,ptrtoboxrecord> list;

/*symbol[w] is the initial of player w*/
array<0,last_who,char> symbol;

/*kind keeps track whether or not each player is a human or a computer*/
array<0,last_who,kindofplayer> kind;

/*score[w] is the number of boxes player w has completed*/
array<0,last_who,integer> score;

integer mode,						/*the mode of play (see instructions), either 1,2,3 or 4*/
	totalscore,				/*totalscore = the sum of the two player's score*/
	maxtotalscore,			/*the maximum possible value of totalscore*/
	boardsizex, boardsizey,	/*the size of the board in squares across and down respectively*/
	oldscore,					/*the score of the current player before his last move*/
	numoftph,				/*the number of length 2 hand out possibilities*/
	numoffph,				/*the number of length 4 hand out possibilities*/
	x, y, i, j, h, v                        /*temporary variables (for the horizontal and vertical coordinates)*/;
boolean safe,						/*safe is true iff there exists at least one box in list[0] or list[1]*/
	/*	that is not totally connected to boxes with 2 edges*/
	checkneeded,				/*CheckNeeded is true if safe needs to be re-evaluated*/
	pathscomputed,			/*this is true if the 2-paths have been recorded in the behind, ahead*/
	/*	and pathsize fields of the box-records in the edge-lists*/
	badpos;			                /*used to evaluate whether or not the selected position is legal*/
who whoseturn;			        /*whose turn it is to play*/
direction k, d;
ptrtoboxrecord temp, temp2;
pathdirection pdir;


/**/
/*										OTHER PROCEDURES										 */

/*Initializes/Reinitializes the path data for a single record*/
void Clear (ptrtoboxrecord p) {
	
	p->pathsize = 0;
	p->pnext[ahead] = nil;
	p->pnext[behind] = nil;
}   /*Clear*/

//###
/*Produces a random number between 1 and n inclusive*/
integer rnd (integer n) {
	
	integer rnd_result;
	rnd_result = trunc((random / maxint + 1) * n / 2) + 1;
	return rnd_result;
}   /*rnd*/

/*RandomDirection returns a direction chosen at random*/
direction RandomDirection() {
	
	integer arandnum;
	
	direction randomdirection_result;
	arandnum = rnd(4);
	if (arandnum == 1) 
		randomdirection_result = down;
	else if (arandnum == 2) 
		randomdirection_result = right;
	else if (arandnum == 3) 
		randomdirection_result = up;
	else
		randomdirection_result = left;
	return randomdirection_result;
}   /*RandomDirection*/

/*Returns the next direction in a clockwise fashion*/
direction NextDirection (direction dir) {
	
	direction NextDirection_result;
	if (dir == down) 
		NextDirection_result = left;
	else
		NextDirection_result = succ(direction,dir);
	return NextDirection_result;
}   /*NextDirection*/

/*Given a box at x,y GetDirection will return a direction randomly such that*/
/*	edge[x,y,GetDirection(x,y)] = false */
direction GetDirection (integer x, integer y) {
	
	direction d, dir;
	
	/*cycle through directions starting at random direction dir*/
	direction getdirection_result;
	dir = randomdirection();
	d = NextDirection(dir);
	while (edge[x][y][d]) 
		d = NextDirection(d);
	getdirection_result = d;
	return getdirection_result;
}   /*GetDirection*/

/*Returns the direction opposite to the direction passed to the function*/
direction OppositeDirection (direction dir) {
	
	direction oppositedirection_result;
	switch (dir) {
	case left : 
		oppositedirection_result = right;
		break;
	case up : 
		oppositedirection_result = down;
		break;
	case right : 
		oppositedirection_result = left;
		break;
	case down : 
		oppositedirection_result = up;
		break;
	}
	return oppositedirection_result;
}   /*OppositeDirection*/

/*Given a 3-box on a 2-list, returns the path direction out of the three box*/
pathdirection OutPathDirection (ptrtoboxrecord p) {
	
	pathdirection outpathdirection_result;
	if (p->pnext[ahead] != nil) 
		outpathdirection_result = ahead;
	else
		outpathdirection_result = behind;
	return outpathdirection_result;
}   /*OutPathDirection*/

/*Swaps the pointers p1 and p2*/
void Swap (ptrtoboxrecord& p1, ptrtoboxrecord& p2) {
	
	ptrtoboxrecord temp;
	
	temp = p1;
	p1 = p2;
	p2 = temp;
}   /*Swap*/

/*Returns the number of edges of the box at (x,y)*/
integer NumberOfEdges (integer x, integer y) {
	
	integer count;
	direction dir;
	
	integer NumberOfEdges_result;
	count = 0;
	for( dir = left; dir <= down; dir = succ(direction,dir))
		if (edge[x][y][dir]) 
			count++;
		NumberOfEdges_result = count;
		return NumberOfEdges_result;
}   /*NumberOfEdges*/

/*Returns true if x,y specifies a box within the boundaries.  Returns false otherwise*/
boolean IsInsideBoard (integer x, integer y) {
	
	boolean IsInsideBoard_result;
	if ((x >= 1) && (x <= boardsizex) && (y >= 1) && (y <= boardsizey)) 
		IsInsideBoard_result = true;
	else
		IsInsideBoard_result = false;
	return IsInsideBoard_result;
}   /*IsInsideBoard*/

/*Given coordinate x, returns what the new coordinate x would be after moving in direction dir*/
integer NewXCoord (integer& x, direction dir) {
	
	integer NewXCoord_result;
	if (dir == left) 
		NewXCoord_result = x - 1;
	else if (dir == right) 
		NewXCoord_result = x + 1;
	else
		NewXCoord_result = x;
	return NewXCoord_result;
}   /*NewXCoord*/

/*Given coordinate y, returns what the new coordinate y would be after moving in direction dir*/
integer NewYCoord (integer& y, direction dir) {

	integer NewYCoord_result;
	if (dir == up) 
		NewYCoord_result = y - 1;
	else if (dir == down) 
		NewYCoord_result = y + 1;
	else
		NewYCoord_result = y;
	return NewYCoord_result;
}   /*NewYCoord*/

/*Re-evaluates safe*/
void CheckSafety (boolean& safe) {
	
	ptrtoboxrecord temp;
	integer hor, ver, f;
	direction dir;
	
	safe = false;
	for(f = 0; f <= 1; f++) //########
	{     /*Check 0&1 lists*/
		temp = list[f];
		while ((temp != nil) && !safe) 
		{     /*Check a box on list*/
			hor = temp->xcoord;
			ver = temp->ycoord;
			for( dir = left; dir <= down; dir = succ(direction,dir))
				if (! edge[hor][ver][dir]) 
					if (NumberOfEdges(NewXCoord(hor, dir), NewYCoord(ver, dir)) != 2) 
						safe = true;
					temp = temp->next;
		}
	}
}   /*CheckSafety*/

/*Given a pointer to a 2-box or a 3-box, computes the path from that box*/
void ComputeAPath (ptrtoboxrecord posinedgelist) {
	
	integer pathlength, n, hor, ver;
	direction dir;
	boolean pathend, finished;
	ptrtoboxrecord temp, temp2;
	pathdirection pdir;
	
	pathlength = -1;		/*set to -1 because posinedge is counted twice*/
	for( pdir = behind; pdir <= ahead; pdir = succ(pathdirection,pdir))
	{     /*each iteration moves to the end of the path in the behind/ahead direction*/
		temp = posinedgelist;
		do {   /*each iteration mards one more square in the path*/
			pathlength++;
			temp->pathsize = markvalue;
			pathend = true;
			/*check for a continuation of the path in the pdir direction*/
			finished = false;
			dir = left;
			do {	/*each iteration checks one direction for an adjoining 2/3-square*/
				if (! edge[temp->xcoord][temp->ycoord][dir]) 
				{
					hor = NewXCoord(temp->xcoord, dir);
					ver = NewYCoord(temp->ycoord, dir);
					if (IsInsideBoard(hor, ver)) 
					{
						temp2 = ptr[hor][ver];
						if (temp2->pathsize != markvalue) 
						{     /*found square next to current square that hasn't been marked*/
							n = NumberOfEdges(hor, ver);
							if ((n == 2) || (n == 3)) 
							{     /*include this square in path*/
								temp->pnext[pdir] = temp2;
								if (pdir == behind) 
									temp2->pnext[ahead] = temp;
								else
									temp2->pnext[behind] = temp;
								temp = temp2;
								pathend = false;
							}
						}
					}
				}   /*if*/
				if (dir == down) 
					finished = true;
				else
					dir = succ(direction,dir);
			} while (!(finished || (! pathend)));
			/*Either all directions have been looked at or a path continuation has been found*/
		} while (!pathend);
		temp->pnext[pdir] = nil;
	}   /*for*/
	/*Update pathsize for all boxes in the newly computed path*/
	for( pdir = behind; pdir <= ahead; pdir = succ(pathdirection,pdir))
	{
		temp = posinedgelist;
		do {
			temp->pathsize = pathlength;
			temp = temp->pnext[pdir];
		} while (!((temp == nil) || (temp == posinedgelist)));
	}
}   /*ComputeAPath*/

/*Finds all 2-paths and stores them by forming a doubly-linked list for each path*/
void ComputePaths() {
	
	ptrtoboxrecord posinedgelist;
	
	posinedgelist = list[2];
	while (posinedgelist != nil) 
	{     /*each iteration moves one box along the edge[2] list*/
		if ((posinedgelist->pnext[ahead] == nil) && (posinedgelist->pnext[behind] == nil)) 
			ComputeAPath(posinedgelist);			/*Compute path containing this element*/
		posinedgelist = posinedgelist->next;
	}   /*while*/
	pathscomputed = true;
}   /*ComputePaths*/

/*Updates the paths that the newly changed box (at hor,ver) was in if needed*/
void UpdatePathFrom (integer hor, integer ver) {
	
	integer count;
	pathdirection pdir;
	
	/*Update path lists*/
	count = NumberOfEdges(hor, ver);
	if ((count == 2) || (count == 3)) 
		ComputeAPath(ptr[hor][ver]);
	else if (count == 4) 
		clear(ptr[hor][ver]);
}   /*UpdatePathFrom*/

/*Adds an edge to box at x,y and direction d.  If the edge completes a square update score, etc.*/
/*If safe might need to be changed then CheckNeeded will be set to true.*/
void AddEdge (integer x, integer y,
					direction d,
					who whoseturn,
					boolean& checkneeded) {
	
	integer count;
	ptrtoboxrecord temp;
	pathdirection pdir;
	
	edge[x][y][d] = true;
	if ((x > 0) && (y > 0) && (x <= boardsizex) && (y <= boardsizey)) 
	{
		count = NumberOfEdges(x, y);
		if (count == 4) 
			PrintInitial(x, y, whoseturn);
		
		/*Take Ptr[x,y] off of old edge list*/
		if (ptr[x][y]->previous != nil) 
			ptr[x][y]->previous->next = ptr[x][y]->next;
		else
			list[count - 1] = ptr[x][y]->next;
		if (ptr[x][y]->next != nil) 
			ptr[x][y]->next->previous = ptr[x][y]->previous;
		
		/*Add Ptr[x,y] to the beginning of a new list*/
		ptr[x][y]->previous = nil;
		ptr[x][y]->next = list[count];
		list[count] = ptr[x][y];
		if (ptr[x][y]->next != nil) 
			ptr[x][y]->next->previous = ptr[x][y];
		
		/*Update safe if necessary*/
		if (safe && (count == 2)) 
			checkneeded = true;
		if ((count == 3) && (list[2] == nil)) 
			safe = true;
	}
}   /*AddEdge*/

/*Picks the first box it can find with n edges and returns the x and y coordinates of that box*/
/*	along with a direction (picked randomly) in which there is no edge. */
void PlayFromList (integer n,
						 integer& x, integer& y,
						 direction& d) {
	
	x = list[n]->xcoord;
	y = list[n]->ycoord;
	d = GetDirection(x, y);
}   /*PlayFromList*/

/*Returns the x and y coordinates of a box in the smallest 2-path.*/
/*Assumes that paths have been computed!*/
void MinimumTwoPath (integer& x, integer& y) {
	
	integer minvalue;
	ptrtoboxrecord temp;
	
	/*find minimum pathlength on 2-list*/
	minvalue = maxx * maxy;
	temp = list[2];
	while (temp != nil) 
	{
		if (temp->pathsize < minvalue) 
			minvalue = temp->pathsize;
		temp = temp->next;
	}
	/*return x and y coordinates of minimum pathlength*/
	temp = list[2];
	while (temp->pathsize > minvalue) 
		temp = temp->next;
	x = temp->xcoord;
	y = temp->ycoord;
}   /*MinimumTwoPath*/

/*Given a HandOut type (either 2 or 4) and  that every box on the 3-list is*/
/*	either a 2 or hand out, HandOut finds a hand out of the indicated type in*/
/*	the 3-list and sets hor, ver, and dir for the move which would do this*/
/*	hand out.*/
void HandOut (integer hotype,
					integer& x, integer& y,
					direction& d) {
	
	integer hor, ver;
	ptrtoboxrecord temp, temp2;
	pathdirection pdir;
	direction dir;
	boolean found;
	
	found = false;
	temp = list[3];
	while (temp != nil && !found) 
	{
		if (temp->pathsize == hotype) 
		{    /*HandOut*/
			found = true;
			pdir = outpathdirection(temp);
			temp2 = temp->pnext[pdir];
			x = temp2->xcoord;
			y = temp2->ycoord;
			for( dir = left; dir <= down; dir = succ(direction,dir))
				if (! edge[x][y][dir]) 
				{
					hor = NewXCoord(x, dir);
					ver = NewYCoord(y, dir);
					if (! IsInsideBoard(hor, ver)) 
						d = dir;
					else if (temp != ptr[hor][ver]) 
						d = dir;
				}
		}   /*if*/
		temp = temp->next;
	}   /*while*/
}   /*HandOut*/

/*Given hor,ver coordinates of a 2-box on a 2-list of length two, returns the*/
/*	correct direction that a new edge should be placed.*/
direction HardHeartedHandout (integer hor, integer ver) {
	
	integer x, y;
	direction dir;
	
	direction hardheartedhandout_result;
	for( dir = left; dir <= down; dir = succ(direction,dir))
		if (! edge[hor][ver][dir]) 
		{
			x = NewXCoord(hor, dir);
			y = NewYCoord(ver, dir);
			if (NumberOfEdges(x, y) == 2) 
				hardheartedhandout_result = dir;
		}
		return hardheartedhandout_result;
}   /*HardHeartedHandout*/

/*Sets up variables.*/
void Initialize()
{
	for( i = 0; i <= boardsizex + 1; i ++)
		for( j = 0; j <= boardsizey + 1; j ++)
			for( k = left; k <= down; k = succ(direction,k))
				edge[i][j][k] = false;

	/*Initialize list*/
	list[0] = new boxrecordtype;
	list[0]->previous = nil;
	temp = list[0];
	for( i = 1; i <= 4; i ++)
		list[i] = nil;
	for( i = 1; i <= boardsizex; i ++)
		for( j = 1; j <= boardsizey; j ++) {
			ptr[i][j] = temp;
			clear(temp);
			if ((i == boardsizex) && (j == boardsizey)) 
				temp->next = nil;
			else
			{
				temp->next = new boxrecordtype;
				temp->next->previous = temp;
				temp = temp->next;
			}
		}
		
	/*mix up list elements randomly*/
    for( i = 1; i <= boardsizex; i ++)
		for( j = 1; j <= boardsizey; j ++)
			swap(ptr[i][j], ptr[rnd(boardsizex)][rnd(boardsizey)]);
	for( i = 1; i <= boardsizex; i ++)
		for( j = 1; j <= boardsizey; j ++)
		{
			ptr[i][j]->xcoord = i;
			ptr[i][j]->ycoord = j;
		}
						
	/*Initialize variables*/
	safe = true;
	pathscomputed = false;
	maxtotalscore = boardsizex * boardsizey;
	totalscore = 0;
	score[firstplayer] = 0;
	score[secondplayer] = 0;
}

void ComputerTurn(int &x, int &y, int &d)
{
	/*generate x, y, and d for computer's move*/
	if (list[3] == nil) 
	{
		if ((list[0] == nil) && (list[1] == nil)) 
		{    /*must pick a 2-2 square*/
			if (! pathscomputed) 
				ComputePaths();
			MinimumTwoPath(x, y);
			if (ptr[x][y]->pathsize == 2) 
				d = HardHeartedHandout(x, y);
			else
				d = GetDirection(x, y);
			return;
		}  /*if*/
		
		/*  (list[0]<>nil or list[1]<>nil) and list[3]=nil  */

		/*while not at end of 0-list do*/
		/*	if any legal adjacent element<>2 or is outside board then take it*/
		/*	try next element*/
		for (temp = list[0]; temp != nil; temp = temp->next)
		{
			k = RandomDirection();
			d = k;
			do {
				i = NewXCoord(temp->xcoord, d);
				j = NewYCoord(temp->ycoord, d);
				if (! IsInsideBoard(i, j) || (NumberOfEdges(i, j) != 2)) 
				{
					x = temp->xcoord;
					y = temp->ycoord;
					return;
				}   /*if*/
				d = NextDirection(d);
			} while (d != k);
		}
		
		/*while not at end of 1-list do*/
		/*	if any legal adjacent element<>2 or is outside board then take it*/
		/*	try next element*/
		for (temp = list[1]; temp != nil; temp = temp->next)
		{
			k = randomdirection();
			d = k;
			do {
				if (! edge[temp->xcoord][temp->ycoord][d]) 
				{
					i = NewXCoord(temp->xcoord, d);
					j = NewYCoord(temp->ycoord, d);
					if (! IsInsideBoard(i, j) || (NumberOfEdges(i, j) != 2)) 
					{
						x = temp->xcoord;
						y = temp->ycoord;
						return;
					}   /*if*/
				}   /*if*/
				d = NextDirection(d);
			} while (d != k);
		}

		/*all bad choices: all 0's and 1's are adjacent to 2's*/
		/*now count how many 2's in a row-- if one, take it*/
		/*                                                  -- if two, take it (put | between 2's)*/
		if (! pathscomputed) 
			ComputePaths();
		MinimumTwoPath(x, y);
		if (ptr[x][y]->pathsize == 2) 
			d = HardHeartedHandout(x, y);
		else
			d = GetDirection(x, y);
		return;
	}
	
	/* list[3]<>nil */

	if (safe || (list[2] == nil) || (maxtotalscore - totalscore < hodiff)) 
	{
		PlayFromList(3, x, y, d);
		return;
	}

	/*unsafe & list[2]<>nil & list[3]<>nil*/
	/*while not at end of 3-list do*/
	/*	if the box connected to the 3-box is not a 2-box (or is outside the board)*/
	/*		then take it */
	for (temp = list[3]; temp != nil; temp = temp->next)
	{
		d = GetDirection(temp->xcoord, temp->ycoord);
		i = NewXCoord(temp->xcoord, d);
		j = NewYCoord(temp->ycoord, d);
		if (NumberOfEdges(i, j) != 2) 
		{
			x = temp->xcoord;
			y = temp->ycoord;
			return;
		}
	}

	/*unsafe & list[3]<>nil & all 3-boxes are part of 2-lists*/
	/*Take all 3's that will not ruin HandOut possibilities*/
	if (! pathscomputed) 
		ComputePaths();
	for (temp = list[3]; temp != nil; temp = temp->next)
	{
		/*if connecting 2-path is not of length 2 or 4 then take it*/
		if ((temp->pathsize != 2) && (temp->pathsize != 4)) 
		{
			x = temp->xcoord;
			y = temp->ycoord;
			d = GetDirection(x, y);
			return;
		}   /*if*/
		/*if 2-path looks like 3-2-2-2 then take it*/
		if (temp->pathsize == 4) 
		{
			pdir = outpathdirection(temp);
			temp2 = temp->pnext[pdir]->pnext[pdir]->pnext[pdir];
			if (NumberOfEdges(temp2->xcoord, temp2->ycoord) == 2) 
			{
				x = temp->xcoord;
				y = temp->ycoord;
				d = GetDirection(x, y);
				return;
			}
		}
	} 

	/*Now all 3-paths look like 3-2 or 3-2-2-3.  These both are	     */
	/*	are hand out possibilities...							     */
	/*									 __ __		 __ __		     */
	/*		2-path		    				|__     | -> |__ __ |    */
	/*							__ __ __ __		__ __ __ __          */
	/*		4-path			    |	__ __ __ __ | -> |__ __|__ __ |  */
	/*It is possible to have more than one HandOut available so      */
	/*	 count the number of each kind of HandOut possibility.       */
	numoftph = 0;
	numoffph = 0;
	for (temp = list[3]; temp != nil; temp = temp->next)
		if (temp->pathsize == 2) 
			numoftph++;
		else
			numoffph++;
	numoffph /= 2;
	temp = list[3];
	if (numoftph + numoffph == 1) 
	{    /*only one possibility*/
		if (numoftph == 0) 
		{    /*do a FPH*/
			HandOut(4, x, y, d);
			return;
		}
		if (numoffph == 0) 
		{    /*do a TPH*/
			HandOut(2, x, y, d);
			return;
		}
	}

	/*More than one hand outs available so grab another square first*/
	if (numoffph != 0) 
	{    /*take the square from a length 4 2-list*/
		temp = list[3];
		while (temp->pathsize != 4) 
			temp = temp->next;
		x = temp->xcoord;
		y = temp->ycoord;
		d = GetDirection(x, y);
		return;
	}
	
	/*more than one length two hand outs available so grab from there*/
	PlayFromList(3, x, y, d);
}  /*ComputerTurn*/



#if 0
typedef struct Dot {
  short row;  /* row number of dot, 0..boardSize-1 */
  short col;  /* column number of dot, 0..boardSize-1 */
} Dot;
typedef struct DotLine {
  Dot dot1;  /* first dot of a line */
  Dot dot2;  /* second dot of a line */
    /* legal lines are formed by dots in the same row, in adjacent columns,
       or in the same column in adjacent rows */
} DotLine;                      
void InitDots(
  short boardSize,    /* number of dots per row/col in board */
  Boolean playFirst,
    /* true if you play first, false of opponent plays first */
  WindowPtr dotWindow
    /* color window where you should draw game results */
);
void OpponentMove(
  const DotLine opponentLine
    /* line formed by your opponent on previous move */
);
short /* number of lines generated */ PlayDots(
  DotLine yourLines[]  /* return the lines you form here */
);
void TermDots(void);  /* return any storage you allocated */




void InitDots(
  short boardSize,    /* number of dots per row/col in board */
  Boolean playFirst,
    /* true if you play first, false of opponent plays first */
  WindowPtr dotWindow
    /* color window where you should draw game results */
)
{
	boardsizex = boardsizey = boardSize - 1;
	Initialize();
}

void OpponentMove(
  const DotLine opponentLine
    /* line formed by your opponent on previous move */
)
{
}

short /* number of lines generated */ PlayDots(
  DotLine yourLines[]  /* return the lines you form here */
)
{
}

void TermDots()  /* return any storage you allocated */
{
}
#endif
