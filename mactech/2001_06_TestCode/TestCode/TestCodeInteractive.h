#include <stdio.h>
#include "MultiPlayer.h"

#define kInputDirectory "::InputData:"

void CallSolution(int testCaseNumber, FILE *logFile,
	PlayerData *playerData1,
	PlayerData *playerData2);
void InitToolbox();