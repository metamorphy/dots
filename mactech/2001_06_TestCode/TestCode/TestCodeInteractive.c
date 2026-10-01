//
// TestCode.c
// Copyright © 2001 J. Robert Boonstra II
//
// NOTE: This code is provided to assist you in testing your
//       Programmer's Challenge solution.  It is provided as is.
//       There is no guarantee that this code is free of errors.
//       There is no guarantee that code which passes the test 
//       cases provided here will pass the eventual test code
//       used to evaluate the Challenge.
//
//

#include <stdio.h>
#include <stdlib.h>

#include <MacMemory.h>
#include <string.h>
#include <assert.h>

#include "ProblemName.h"
#include "Dots.h"
#include "Dots2.h"
#include "globals.h"

#define kNumPlayers 2

#include "MultiPlayer.h"
#include "TestCodeInteractive.h"
#include "ProcessEvents.h"

PLAYER_PROTOTYPES()
PLAYER_PROTOTYPES(02)

PlayerData playerInfo[kNumPlayers] = {
	{InitDots  ,OpponentMove  ,PlayDots  ,TermDots  ,0,0,0,0,0,true,true},
	{InitDots02,OpponentMove02,PlayDots02,TermDots02,0,0,0,0,0,true,true}
};

/* problem-specific includes */
/* */

/* print formats */
#define kFormat1 "Test case %d CORRECT, %.3f milliseconds, %ld points\n\n"
#define kFormat2 "Test case %d INCORRECT, %.3f milliseconds, %ld points\n\n"
#define kFormat3 "Player %d cells %d time %.2f score %.2f correct %d\n"

FILE *inFile,*logFile;

static Boolean gGameInProgress=false, gDone=false;

int main(void)
{
	char fileName[256];
	long testCaseCounter;
	EventRecord theEvent;
	Boolean cumCorrect=true;
	
	sprintf(fileName,"%s.log",kProblemName);
	assert(0!=(logFile = fopen(fileName,"w")));

	testCaseCounter = 0;
	
	for (int player=0; player<=kNumPlayers; player++) {
		playerInfo[player].playerNumber = player;
		playerInfo[player].cumScore = 0;
		playerInfo[player].cumCells = 0;
		playerInfo[player].cumCorrect = true;
	}
	
	InitToolbox();

	/* iterate on test cases */
	while (!gDone) {
		int player1,player2;
		PlayerData *thePlayer;
		while (!gGameInProgress & !gNewGameFlag & !gQuitFlag) {
			if (WaitNextEvent(everyEvent,&theEvent,0,nil)) 
				ProcessEvent(&theEvent);
		}
		if (gQuitFlag) {gDone=true; continue;}
		if (gNewGameFlag) {
			char problemName[256]=kProblemName;
			gNewGameFlag = false;
			if (gHumanPlaysFirst) {
				player1=1;	player2=0;
			} else {
				player1=0;	player2=1;
			}
			playerInfo[player1].testCaseTime = 0;
			playerInfo[player1].testCaseScore = 0;
			playerInfo[player1].testCaseCells = 0;
			playerInfo[player1].testCaseCorrect = true;
			playerInfo[player1].playingFirst = true;

			playerInfo[player2].testCaseTime = 0;
			playerInfo[player2].testCaseScore = 0;
			playerInfo[player2].testCaseCells = 0;
			playerInfo[player2].testCaseCorrect = true;
			playerInfo[player2].playingFirst = false;

			fprintf(logFile,"%s evaluation, test case %d, Player %d vs player %d\n",
				kProblemName,testCaseCounter, player1,player2);
			fprintf(logFile,"\n");
		}
		
		/* execute and time solution */
		CallSolution(testCaseCounter, logFile,
				&playerInfo[player1],
				&playerInfo[player2]);

		/* accumulate statistics */
		
		playerInfo[player1].testCaseScore = playerInfo[player2].testCaseCells *
												(1.0 + .01*(playerInfo[player1].testCaseTime / 1000.0));
		playerInfo[player2].testCaseScore = playerInfo[player1].testCaseCells *
												(1.0 + .01*(playerInfo[player2].testCaseTime / 1000.0));
		
		fprintf(logFile,"\nScores for test case %d\n",testCaseCounter);
		for (int player=0; player<2; player++) {
			thePlayer = &playerInfo[player];
			double timeInMilliseconds = thePlayer->testCaseTime / 1000.0;
			thePlayer->cumCorrect &= thePlayer->testCaseCorrect;
			thePlayer->cumCells += thePlayer->testCaseCells;
			thePlayer->cumScore += thePlayer->testCaseScore;
			thePlayer->cumElapsedMilliseconds += timeInMilliseconds;
		
			fprintf(logFile,kFormat3,
				thePlayer->playerNumber,
				thePlayer->testCaseCells,thePlayer->testCaseTime/1000.0,
				thePlayer->testCaseScore,thePlayer->testCaseCorrect);
				
		}
		fprintf(logFile,"\n");
		if (gQuitFlag) gDone=true;
		testCaseCounter++;
	}

	fprintf(logFile,"\nCumulative scores:\n");
	for (int player=0; player<kNumPlayers; player++) {
		PlayerData *thePlayer = &playerInfo[player];
		fprintf(logFile,kFormat3,
			thePlayer->playerNumber,
				thePlayer->cumCells,thePlayer->cumElapsedMilliseconds,
				thePlayer->cumScore,thePlayer->cumCorrect);
	}

	fclose(inFile);
	fclose(logFile);

	return 0;
}

void InitToolbox()
{
	Handle mBar=0;

	InitGraf( &qd.thePort );
	InitFonts();
	InitWindows();
	InitCursor();
	TEInit();
	InitDialogs(nil);
	InitCursor();

	FlushEvents( everyEvent, 0 ) ;
	
	mBar = GetNewMBar(128);
	if (0==mBar) DebugStr("\p mBar zero");
	SetMenuBar(mBar);
	DrawMenuBar();
	DisposeHandle((Handle)mBar);
}
