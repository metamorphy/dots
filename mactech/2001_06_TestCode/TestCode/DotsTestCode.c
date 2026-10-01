//
// DotsTestCode.c
// Copyright © 2001 J. Robert Boonstra II
//
// NOTE: This code is provided to assist you in testing your
//       Programmer's Challenge solution.  It is provided as is.
//       There is no guarantee that this code is free of errors.
//       There is no guarantee that code which passes the test 
//       cases provided here will pass the eventual test code
//       used to evaluate the Challenge.
//
// Initial version - 24 May 01
// Revised 3 June to add SetPort before call to PlayDots
// Revised again 3 June to SetPort before call to OpponentMove and InitDots as well
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "JRBTimer.h"

#include <MacMemory.h>
#include <TextEdit.h>

#include <string.h>
#include <assert.h>

#include "ProblemName.h"
#include "Dots.h"

#include "MultiPlayer.h"
#include "TestCodeInteractive.h"
#include "globals.h"

// define MY_DEBUG to see each move made
#define MY_DEBUG 1
//#define MY_DEBUG 0

/* problem specific data declarations */
static FILE *gLogFile;
static short gNumLines,gNumCells;
static WindowPtr gPlayer1Window,gPlayer2Window;
static Rect gWindRect1 = {40,20,360,340}, gWindRect2 = {40,360,360,680};
static Boolean gQuitFlag=false,gInited=false;
static char *gLinesPlayed;
static char *gCellsWon;
extern short gBoardSize;
	/* numbering scheme for cells is to index cell by the (row,col) value at the upper left,
	   where (row,col) -> (row)*(gBoardSize-1) + col
	 */

/* forward declarations */
static void ReadInputFile(int testCaseNumber);
static Boolean ValidMove(DotLine linePlayed, int playerNumber, int *returnCellsWon);
static void ProcessEvent(EventRecord *ev);
/* utilities */
/* */

void CallSolution(int testCaseNumber, FILE *logFile,
	PlayerData *player1Data,
	PlayerData *player2Data
	) {
#pragma unused(testCaseNumber)
	unsigned long elapsedMicroseconds, totalElapsedMicroseconds = 0;
	Boolean player1ToPlay, done = false;
	DotLine *playerLines;
	EventRecord theEvent;
	short numLinesPlayed,moveCounter;
	
	gLogFile = logFile;
	
	ReadInputFile(testCaseNumber);
	
	/* Initialize problem */
	player1Data->testCaseTime=0;
	player2Data->testCaseTime=0;
	gNumLines = gBoardSize*(gBoardSize-1)*2;
	gNumCells = (gBoardSize-1)*(gBoardSize-1);
	assert (0 != (playerLines = (DotLine *)NewPtr(gNumLines*sizeof(DotLine))) );
	assert (0 != (gLinesPlayed = (char *)NewPtrClear(gNumLines*sizeof(char *))) );
	assert (0 != (gCellsWon = (char *)NewPtrClear(gNumCells*sizeof(char *))) );
	assert (nil != (gPlayer1Window = NewCWindow(nil,&gWindRect1,"\pDots 1",true,documentProc,(WindowPtr)-1,true,0)));
	assert (nil != (gPlayer2Window = NewCWindow(nil,&gWindRect2,"\pDots 2",true,documentProc,(WindowPtr)-1,true,0)));
	
	player1ToPlay = player1Data->playingFirst;

	/* Initialize solution */
	SetPort(gPlayer1Window);
	StartTimer();
	(*player1Data->initDots)(gBoardSize,player1ToPlay,gPlayer1Window);
	elapsedMicroseconds = StopTimer();
	player1Data->testCaseTime += elapsedMicroseconds;

	SetPort(gPlayer2Window);
	StartTimer();
	(*player2Data->initDots)(gBoardSize,!player1ToPlay,gPlayer2Window);
	elapsedMicroseconds = StopTimer();
	player2Data->testCaseTime += elapsedMicroseconds;
	
	moveCounter = 0;

	/* Call solution */
	while (!done) {
		PlayerData *currentPlayerData, *otherPlayerData;
		int numCellsWon;
		if (player1ToPlay) {
			currentPlayerData = player1Data;
			otherPlayerData = player2Data;
			SetPort(gPlayer1Window);
		} else {
			currentPlayerData = player2Data;
			otherPlayerData = player1Data;
			SetPort(gPlayer2Window);
		}
		StartTimer();
		numLinesPlayed = (*currentPlayerData->playDots)(playerLines);
		elapsedMicroseconds = StopTimer();
		currentPlayerData->testCaseTime += elapsedMicroseconds;
		
		if (numLinesPlayed<=0) 
			done=true;
		
		if (!done) {
			for (int i=0; i<numLinesPlayed; i++) {
				DotLine *theLine = &playerLines[i];
				/* determine if move is valid, and calculate cells won */
				Boolean moveIsValid = !ValidMove(playerLines[i], currentPlayerData->playerNumber, &numCellsWon);
				moveCounter++;
				#if MY_DEBUG
				/* log move if logging option selected */
					fprintf(logFile,"Player %d played (%d,%d)->(%d,%d), valid=%d, cellsWon=%d\n",
						currentPlayerData->playerNumber,
						theLine->dot1.row,theLine->dot1.col,theLine->dot2.row,theLine->dot2.col,
						moveIsValid,numCellsWon);
				#endif
				if ( moveIsValid ){
					currentPlayerData->testCaseCorrect = false;
					done = true;
					break;
				} else {
					/* score move */
					currentPlayerData->testCaseCells += numCellsWon;
					/* determine if player was required to move again and did not */
					if ((numCellsWon>0) && (i==numLinesPlayed-1) && (moveCounter<gNumLines) ) {
						fprintf(logFile,"Player %d captured a cell with move to (%d,%d)->(%d,%d) but did not move again\n",
							currentPlayerData->playerNumber,
							theLine->dot1.row,theLine->dot1.col,
							theLine->dot2.row,theLine->dot2.col);
						currentPlayerData->testCaseCorrect = false;
					} else if ((numCellsWon==0) && (i<numLinesPlayed-1)) {
						fprintf(logFile,"Player %d did not capture a cell with move to (%d,%d)->(%d,%d) but did move again\n",
							currentPlayerData->playerNumber,
							theLine->dot1.row,theLine->dot1.col,
							theLine->dot2.row,theLine->dot2.col);
						currentPlayerData->testCaseCorrect = false;
					}
				}
				/* inform opponent of players moves */
				if (player1ToPlay) {
					SetPort(gPlayer2Window);
				} else {
					SetPort(gPlayer1Window);
				}
				StartTimer();
				(*otherPlayerData->opponentMove)(playerLines[i]);
				elapsedMicroseconds = StopTimer();
				otherPlayerData->testCaseTime += elapsedMicroseconds;
			}
		
			player1ToPlay = !player1ToPlay;
		}
		/* look for quit or other events */
		if (WaitNextEvent(everyEvent,&theEvent,0,nil)) 
			ProcessEvent(&theEvent);
		if (gNewGameFlag) done=true;
		if (gQuitFlag) done=true;
	}
	
	/* Terminate solution */
	StartTimer();
	(*player1Data->termDots)();
	elapsedMicroseconds = StopTimer();
	player1Data->testCaseTime += elapsedMicroseconds;

	StartTimer();
	(*player2Data->termDots)();
	elapsedMicroseconds = StopTimer();
	player2Data->testCaseTime += elapsedMicroseconds;

	DisposeWindow(gPlayer2Window);
	DisposeWindow(gPlayer1Window);
	DisposePtr((Ptr)gCellsWon);
	DisposePtr((Ptr)gLinesPlayed);
	DisposePtr((Ptr)playerLines);
}

/* read input data, sort input records */
static void ReadInputFile(int testCaseNumber) {
#pragma unused(testCaseNumber)
/* no input to read for this test code */
}

#define VERTICAL_LINE_OFFSET (gBoardSize * (gBoardSize-1))
#define HORIZ_LINE(row,col) ((row)*(gBoardSize-1) + (col))
#define VERT_LINE(row,col) (VERTICAL_LINE_OFFSET + (row)*(gBoardSize) + (col))
#define CELL_NUMBER(row,col) ((row)*(gBoardSize-1) + (col))

static Boolean ValidMove(DotLine linePlayed, int playerNumber, int *returnCellsWon) {
	int rowNumber, colNumber, lineNumber;
	int cellsWon = 0;
	Boolean horizontal;
	*returnCellsWon = 0;
	if ((linePlayed.dot1.row < 0) || (linePlayed.dot1.row>=gBoardSize)) {
		fprintf(gLogFile,"Player %d illegal row %d in move (%d,%d)-(%d,%d)\n",
			playerNumber,linePlayed.dot1.row,
			linePlayed.dot1.row,linePlayed.dot1.col,linePlayed.dot2.row,linePlayed.dot2.col);
		return false;
	}
	if ((linePlayed.dot1.col < 0) || (linePlayed.dot1.col>=gBoardSize)) {
		fprintf(gLogFile,"Player %d illegal col %d in move (%d,%d)-(%d,%d)\n",
			playerNumber,linePlayed.dot1.col,
			linePlayed.dot1.row,linePlayed.dot1.col,linePlayed.dot2.row,linePlayed.dot2.col);
		return false;
	}
	if ((linePlayed.dot2.row < 0) || (linePlayed.dot2.row>=gBoardSize)) {
		fprintf(gLogFile,"Player %d illegal row %d in move (%d,%d)-(%d,%d)\n",
			playerNumber,linePlayed.dot2.row,
			linePlayed.dot1.row,linePlayed.dot1.col,linePlayed.dot2.row,linePlayed.dot2.col);
		return false;
	}
	if ((linePlayed.dot2.col < 0) || (linePlayed.dot2.col>=gBoardSize)) {
		fprintf(gLogFile,"Player %d illegal row %d in move (%d,%d)-(%d,%d)\n",
			playerNumber,linePlayed.dot2.col,
			linePlayed.dot1.row,linePlayed.dot1.col,linePlayed.dot2.row,linePlayed.dot2.col);
		return false;
	}
	horizontal = (linePlayed.dot1.row == linePlayed.dot2.row);
	if (horizontal) {
		int colDiff = linePlayed.dot1.col - linePlayed.dot2.col;
		if ((colDiff!=1) && (colDiff!=-1)) {
			fprintf(gLogFile,"Player %d illegal line in move (%d,%d)-(%d,%d)\n",
				playerNumber,
				linePlayed.dot1.row,linePlayed.dot1.col,linePlayed.dot2.row,linePlayed.dot2.col);
			return false;
		}
	} else if (linePlayed.dot1.col == linePlayed.dot2.col) {
		int rowDiff = linePlayed.dot1.row - linePlayed.dot2.row;
		if ((rowDiff!=1) && (rowDiff!=-1)) {
			fprintf(gLogFile,"Player %d illegal line in move (%d,%d)-(%d,%d)\n",
				playerNumber,
				linePlayed.dot1.row,linePlayed.dot1.col,linePlayed.dot2.row,linePlayed.dot2.col);
			return false;
		}
	} else return false;
	rowNumber = (linePlayed.dot1.row < linePlayed.dot2.row) ? linePlayed.dot1.row : linePlayed.dot2.row;
	colNumber = (linePlayed.dot1.col < linePlayed.dot2.col) ? linePlayed.dot1.col : linePlayed.dot2.col;
	if (horizontal) {
		lineNumber = HORIZ_LINE(rowNumber,colNumber);
	} else {
		lineNumber = VERT_LINE(rowNumber,colNumber);
	}
	if (gLinesPlayed[lineNumber] != 0) {
		fprintf(gLogFile,"Player %d played existing line in move (%d,%d)-(%d,%d)\n",
			playerNumber,
			linePlayed.dot1.row,linePlayed.dot1.col,linePlayed.dot2.row,linePlayed.dot2.col);
		return false;
	}
	
	/* record results of move */
	gLinesPlayed[lineNumber] = 1;
	if (horizontal) {
		/* check cell above this line */
		if ( (rowNumber>0) && (colNumber<gBoardSize-1) &&
			 (0 != gLinesPlayed[HORIZ_LINE(rowNumber-1,colNumber)]) &&
			 (0 != gLinesPlayed[VERT_LINE(rowNumber-1,colNumber  )]) &&
			 (0 != gLinesPlayed[VERT_LINE(rowNumber-1,colNumber+1)]) ) {
			 	gCellsWon[CELL_NUMBER(rowNumber-1,colNumber)] = playerNumber;
			 	++cellsWon;
		}
		/* check cell below this line */
		if ( (rowNumber<gBoardSize-1) && (colNumber<gBoardSize-1) &&
			 (0 != gLinesPlayed[HORIZ_LINE(rowNumber+1,colNumber)]) &&
			 (0 != gLinesPlayed[VERT_LINE(rowNumber,colNumber  )]) &&
			 (0 != gLinesPlayed[VERT_LINE(rowNumber,colNumber+1)]) ) {
			 	gCellsWon[CELL_NUMBER(rowNumber,colNumber)] = playerNumber;
			 	++cellsWon;
		}
	} else {
		/* check cell left of this line */
		if ( (colNumber>0) &&  (rowNumber<gBoardSize-1) &&
			 (0 != gLinesPlayed[VERT_LINE(rowNumber,colNumber-1)]) &&
			 (0 != gLinesPlayed[HORIZ_LINE(rowNumber  ,colNumber-1)]) &&
			 (0 != gLinesPlayed[HORIZ_LINE(rowNumber+1,colNumber-1)]) ) {
			 	gCellsWon[CELL_NUMBER(rowNumber,colNumber-1)] = playerNumber;
			 	++cellsWon;
		}
		/* check cell right of this line */
		if ( (colNumber<gBoardSize-1) && (rowNumber<gBoardSize-1) &&
			 (0 != gLinesPlayed[VERT_LINE(rowNumber,colNumber+1)]) &&
			 (0 != gLinesPlayed[HORIZ_LINE(rowNumber  ,colNumber  )]) &&
			 (0 != gLinesPlayed[HORIZ_LINE(rowNumber+1,colNumber  )]) ) {
			 	gCellsWon[CELL_NUMBER(rowNumber,colNumber)] = playerNumber;
			 	++cellsWon;
		}
	}
	*returnCellsWon = cellsWon;
	return true;
}
