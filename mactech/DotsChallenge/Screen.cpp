// Screen.cpp
// Copyright 2001 Jeff Mallett.  All rights reserved.


#include <stdio.h>
#include <string.h>

// ---------------------- DRAWING CONSTANTS

// The full board can't actually fit on the window.  I get less than 20x20
// boxes showing in the test code window.  Therefore only draw
// the upper-left portion of grid if it's that big.
// The maximum dimensions to draw can be adjusted here if the window
// size is increased:
const int MAX_X_DRAW = 25;
const int MAX_Y_DRAW = 25;

const int BOX_WIDTH = 15;			/*pixel distance between adjacent dots*/
const int SCORE_TITLE_Y = 20;
const int SCORE_Y = 38;				/*distance of scores from top of screen*/
const int GRID_Y = 50;
const int SCORE_X[2] = {40, 130};
const char PLAYER_INITIAL[2] = { '1', '2' };/*PLAYER_INITIAL[w] is the initial of player w*/


// ---------------------- EXTERNS
// (These should have been put in a header file...)

// COPIED
enum direction {left=0, up, right, down};
enum who {firstplayer=0, secondplayer};

// EXTERNED
extern int gBoardSizeX;
extern int gBoardSizeY;


// ---------------------- PROTOTYPES

void ScreenLoc (int x, int y, int& i, int& j);
void DrawEdge (int x1, int y1, int x2, int y2);
void DrawDots();
void DrawScore (who w);
void DrawInitial (int x, int y, who person);
void DrawMove(int x, int y, direction d);
void DrawScoreTitle(who person);
void InitialDrawing();


////////////////////////////////////////////////////////////////////////


// *********************************************************************
//
// DRAWING PROCEDURES
//
// *********************************************************************

/*Given box coordinates x,y returns coords of top-left point of box (i,j)*/
void ScreenLoc (int x, int y, int& i, int& j) {
	
	i = x * BOX_WIDTH;
	j = y * BOX_WIDTH + GRID_Y;
}   /*ScreenLoc*/

/*Given the coordinates of two boxes x1, y1 and x2, y2, this will draw in*/
/*	an edge connecting the dot in the upper left hand corner of the two boxes.*/
void DrawEdge (int x1, int y1, int x2, int y2) {
	
	int i1, j1, i2, j2;
	ScreenLoc(x1, y1, i1, j1);
	ScreenLoc(x2, y2, i2, j2);
	MoveTo(i1, j1);
	LineTo(i2, j2);
}   /*DrawEdge*/

/*Print the dots on the screen which form the playing board*/
void DrawDots() {
	
	int hor, ver, i, j;
	Rect rect;
	int maxh = gBoardSizeX;
	int maxv = gBoardSizeY;
	
	if (maxh > MAX_X_DRAW)
		maxh = MAX_X_DRAW;
	if (maxv > MAX_Y_DRAW)
		maxv = MAX_Y_DRAW;
	++maxh;
	++maxv;
	
	for( hor = 1; hor <= maxh; hor++)
		for( ver = 1; ver <= maxv; ver++) {
			ScreenLoc(hor, ver, i, j);
			SetRect(&rect, i, j, i + 1, j + 1);
			PaintOval(&rect);
		}
}   /*DrawDots*/

/*Update a player's score on the drawing window*/
void DrawScore (who w) {
	
	extern int gScore[];
	int n;
	Rect rect;
	
	n = SCORE_X[w];
	SetRect(&rect, n, SCORE_Y - 11, n + 40, SCORE_Y + 2);
	EraseRect(&rect);

	char s[256];
	sprintf(s, "%1d", gScore[w]);
	
	MoveTo(n, SCORE_Y);
	DrawText(s, 0, strlen(s));
}   /*DrawScore*/

void DrawInitial (int x, int y, who person) {

	if (x <= MAX_X_DRAW && y <= MAX_Y_DRAW)
	{
		int hor, ver;
		
		ScreenLoc(x, y, hor, ver);
		hor += (BOX_WIDTH / 2) - 3;
	
		ver += (BOX_WIDTH / 2) + 6;
		//TextFace(bold);
		MoveTo(hor, ver);
		DrawText(&PLAYER_INITIAL[person], 0, 1);
		//TextFace(normal);
	}
}   /*DrawInitial*/


void DrawMove(int x, int y, direction d) {

	if (x <= MAX_X_DRAW && y <= MAX_Y_DRAW)
	{
		switch (d) {
		case left : 
			DrawEdge(x, y, x, y + 1);
			break;
		case up : 
			DrawEdge(x, y, x + 1, y);
			break;
		case right : 
			DrawEdge(x + 1, y, x + 1, y + 1);
			break;
		case down : 
			DrawEdge(x, y + 1, x + 1, y + 1);
			break;
		}
	}
}

void DrawScoreTitle(who person) {

	Rect rect;
	int n = SCORE_X[person];
	SetRect(&rect, n, SCORE_TITLE_Y - 11, n + 40, SCORE_TITLE_Y + 2);
	EraseRect(&rect);

	MoveTo(n, SCORE_TITLE_Y);
	DrawText(&PLAYER_INITIAL[person], 0, 1);
}

void InitialDrawing() {

	DrawScoreTitle(firstplayer);
	DrawScoreTitle(secondplayer);

	int x = SCORE_X[firstplayer]-10;
	int y = SCORE_TITLE_Y+4;
	int x2 = SCORE_X[secondplayer]+20;
	MoveTo(x, y);
	LineTo(x2, y);
	MoveTo((x+x2)/2, SCORE_TITLE_Y-15);
	LineTo((x+x2)/2, SCORE_Y+15);
	
	DrawDots();
}



