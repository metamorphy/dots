void CountOnesGroup(PtrToBoxRecord box, int &countones, 
					int &count2paths, PtrToTwoPathRecord &shortest2path);
void FindConnecting1Box(PtrToBoxRecord A, PtrToBoxRecord &B, PtrToTwoPathRecord &pathAB);
void FindAnother2Path(PtrToBoxRecord A, PtrToTwoPathRecord &path);
int MoveToEndpoint(PtrToBoxRecord cameFrom, PtrToBoxRecord current, int remainingDepth,
					PtrToBoxRecord &A, PtrToBoxRecord &B, PtrToTwoPathRecord &pathAB);
PtrToTwoPathRecord FindMidPath(PtrToBoxRecord inGroup, int totalBoxes, int totalPaths);
PtrToTwoPathRecord IsInZugzwang();

// ---------------------- CONSTANTS

//  0  1  2  3  4  5  6  7
static const short PARITY[6][8] =
{
  { 0, 1,-1,-1,-1,-1,-1,-1}, // 0
  { 0, 1, 1, 1,-1,-1,-1,-1}, // 1
  {-1, 1, 1, 2, 2, 2,-1,-1}, // 2
  {-1,-1, 1, 2, 0, 0, 2, 2}, // 3
  {-1,-1,-1, 2, 0, 2, 2, 2}, // 4
  {-1,-1,-1,-1, 0, 2, 1, 1}  // 5
};



		gUseZugzwang = (gCurrentPlayer == firstplayer); //#########

// Moves a box on the list to the front of the list
template<class T> void MoveToFrontofList(T element, T &list) {
	if (element->previous)
	{
		RemoveFromList(element, list);
		AddToList(element, list);
	}
}



// Counts the 1-boxes and tiny 2-paths of an unsafe 1-group
void CountOnesGroup(PtrToBoxRecord box, int &countones, 
					int &count2paths, PtrToTwoPathRecord &shortest2path) {

	count2paths = 0;
	countones = 1; // count this 1-box
	box->flag = MARK_VALUE;

	// Include 1's down adjacent two lists
	FOR_EACH_OPEN_DIRECTION(box)
	{
		if (IsOnTiny2Path(f_box)) // Note: all paths will be inside board
		{
			++count2paths;
			if (!shortest2path || f_box->twopath->pathsize < shortest2path->pathsize)
				shortest2path = f_box->twopath;

			PtrToBoxRecord box2 = GoForward(box, f_box, f_box->twopath->pathsize); // box after path
			if (NumberOfEdges(box2) == 1)
			{
				if (!box2->flag)
				{ // Add this box to the group
					int dAdjacentOnes, dAdjacent2Paths;
					CountOnesGroup(box2, dAdjacentOnes, dAdjacent2Paths, shortest2path);
					countones += dAdjacentOnes;
					count2paths += dAdjacent2Paths;
				}
				else if (IsInsideBoard(box2))
					// Either
					// 1) We've already counted this 2-path, or
					// 2) It's a second way to get to the 1-box, e.g.
					//  _  
					// |   |
					// |  _|
					// 
					// Either way, don't count it
					--count2paths;
			}
		}
	} END_FOR_EACH_OPEN_DIRECTION;
}   // CountOnesGroup


// A is an endpoint 1-box in a 1-group.
// Sets B (an connected 1-box) and pathAB (the path from A to B)
void FindConnecting1Box(PtrToBoxRecord A, PtrToBoxRecord &B, PtrToTwoPathRecord &pathAB) {

	int count = 0;
	FOR_EACH_OPEN_DIRECTION(A)
	{
		if (IsOnTiny2Path(f_box)) // Note: all paths will be inside board
		{
			PtrToBoxRecord box = GoForward(A, f_box, f_box->twopath->pathsize); // box after path
			if (IsInsideBoard(box) && NumberOfEdges(box) == 1)
			{
				B = box;
				pathAB = f_box->twopath;
				return;
			}
		}
	} END_FOR_EACH_OPEN_DIRECTION;

	ASSERT(false);
}   // FindConnecting1Box


// A is a 1-box in a 1-group.
// Search 2-paths from A.  If a path other than "path" is found,
// return it in "path".
void FindAnother2Path(PtrToBoxRecord A, PtrToTwoPathRecord &path) {

	int count = 0;
	FOR_EACH_OPEN_DIRECTION(A)
	{
		if (IsOnTiny2Path(f_box) && f_box->twopath != path) // Note: all paths will be inside board
		{
			path = f_box->twopath;
			return;
		}
	} END_FOR_EACH_OPEN_DIRECTION;

	ASSERT(false);
}   // FindAnother2Path


// A is a 1-box in a 1-group connected to 1 or more other 1-boxes through 2-paths.
// Move through 1-group until an endpoint (a 1-box only connected to one other 1-box)
// is reached.  Set and return this in A, the adjacent box in B, and the path A-B in
// pathAB.  If current is an endpoint itself, return false to indicate that values
// could not be set.
// Note that the 1-group may be circular, so limit search to given depth.
int MoveToEndpoint(PtrToBoxRecord cameFrom, PtrToBoxRecord current, int remainingDepth,
					PtrToBoxRecord &A, PtrToBoxRecord &B, PtrToTwoPathRecord &pathAB) {

	if (remainingDepth)
	{
		FOR_EACH_OPEN_DIRECTION(current)
		{
			if (f_box != cameFrom && IsOnTiny2Path(f_box)) // Note: all paths will be inside board
			{
				PtrToBoxRecord box = GoForward(current, f_box, f_box->twopath->pathsize); // box after path
				if (IsInsideBoard(box) && NumberOfEdges(box) == 1)
				{
					if (!MoveToEndpoint(current, box, remainingDepth-1, A, B, pathAB))
					{
						A = box;
						B = current;
						pathAB = f_box->twopath;
					}
					return true; // info is set
				}
			}
		} END_FOR_EACH_OPEN_DIRECTION;
	}
	return false; // info isn't set
}   // MoveToEndpoint


/*
Parity 1 algorithm   Works for 23A,24,25,33A,33B,36,43,37
    * Find box A only connected to 1 other 1-box B
    * countBoxes = 1
    X if (2*countBoxes >= totalBoxes)
        return path A-B
    * countPaths = 2-paths out of A
    * if (2*countPaths >= totalPaths)
         return path A-B
    * If countPaths >= (totalPaths-1)/2 //If 2*countPaths+1 == totalPaths 
        [In 33b do after 1 out of 3.  In 36 do after 2 out of 6]
         find path out of B that doesn't connect to a 1-box
             if one was found, play it
    * countPaths += 2-paths out of B - 1
    * Find path out of B that leads to a 1-box C where C!=A
    * A = B, B = C, ++countBoxes
    * Goto X 

Parity 0 algorithm   Works for 23A,24,25,33A,33B,36,43,37
    * Find path from Parity 1
    * For each 1 on path
        If has another path, return that
*/
void FindMidPath(PtrToBoxRecord inGroup, int totalBoxes, int totalPaths,
					PtrToBoxRecord &A, PtrToTwoPathRecord &pathAB) {
	int countPaths;
	int countBoxes = 1;
	PtrToBoxRecord B, C;

	// From A in group, find box A only connected to 1 other 1-box B
	// Sets A, B, pathAB
	if (!MoveToEndpoint(NULL, inGroup, totalBoxes, A, B, pathAB))
	{
		A = inGroup;
		FindConnecting1Box(A, B, pathAB);
	}

	do {
		if (2*countBoxes >= totalBoxes)
			return;

		countPaths = CountSurrounding2Paths(A);

		if (2*countPaths >= totalPaths)
			return;

		if (countPaths >= (totalPaths-1)/2) //If 2*countPaths+1 == totalPaths 
		{
			// Find path out of B that doesn't connect to a 1-box
			FOR_EACH_OPEN_DIRECTION(B)
			{
				if (IsOnTiny2Path(f_box)) // Note: all paths will be inside board
				{
					C = GoForward(B, f_box, f_box->twopath->pathsize); // box after path
					if (IsInsideBoard(C) && NumberOfEdges(C) != 1)
					{
						A = B;
						pathAB = f_box->twopath;
						return; // One was found, so play it
					}
				}
			} END_FOR_EACH_OPEN_DIRECTION;
		}

		countPaths += CountSurrounding2Paths(B) - 1;

		// Find path out of B that leads to a 1-box C where C!=A
		FOR_EACH_OPEN_DIRECTION(B)
		{
			if (IsOnTiny2Path(f_box)) // Note: all paths will be inside board
			{
				C = GoForward(B, f_box, f_box->twopath->pathsize); // box after path
				if (C != A && NumberOfEdges(C) == 1) // found new path to another 1-box
				{
					A = B;
					B = C;
					pathAB = f_box->twopath;
					break;
				}
			}
		} END_FOR_EACH_OPEN_DIRECTION;

		++countBoxes;
    } while (true); //###
}   // FindMidPath


// Checks zugzwang.
// Also may return a path we can move-to to change a crucial 1-box.  If
//    we are not in zugzwang, then we may have to
//    play this box to stay out of zugzwang.
PtrToTwoPathRecord IsInZugzwang()
{
	ASSERT(!gList[0] && gList[1] && !gList[3] && !gSafe[1]);
	ASSERT(gPathsComputed);

	int count = 0;
	PtrToBoxRecord temp;
	PtrToTwoPathRecord playPath = NULL;

	// Go through tiny list counting length 1 or 2 paths
	PtrToTwoPathRecord p;
	for (p = gTwoPaths[1]; p; p = p->next)
		++count;
	for (p = gTwoPaths[2]; p; p = p->next)
		++count;

	TRACE("IsInZugzwang: Tiny list = %d\n", count);

	// Clear flags in 1's list
	FOR_LIST(temp, 1) 
	{
		temp->flag = NO_VALUE;
	}

	// Go through 1's list looking for adjacent tiny lists
	PtrToTwoPathRecord bestOptionGroup, bestNonOptionGroup;
	PtrToBoxRecord bestOption1Box;
	int saveCountOnes, saveCount2Paths;
	saveCountOnes = saveCount2Paths = 0;
	bestOptionGroup = bestNonOptionGroup = NULL;
	int options = 0;
	FOR_LIST(temp, 1) 
	{
		if (!temp->flag)
		{
			int countones, count2paths, parity;
			PtrToTwoPathRecord shortest = NULL;
			CountOnesGroup(temp, countones, count2paths, shortest);
			if (count2paths >= 2 && countones < 6 && count2paths < 8)
			{
				parity = PARITY[countones][count2paths];
				/*TRACE(" 1-group {%d,%d} with %d 1-boxes, %d paths, and parity=%d\n",
						GetX(temp), GetY(temp), countones, count2paths, parity);*/
				if (parity != -1) //###
				{
					// Special case: If 3 shorts around a single 1-box, then Parity 1
					if (parity == 2 && countones == 2 && count2paths == 3 && CountSurrounding2Paths(temp) != 2)
						parity = 1;

					if (parity == 2) // either parity
					{
						TRACE(" 1-group {%d,%d} with %d 1-boxes, %d paths, and parity=%d\n",
								GetX(temp), GetY(temp), countones, count2paths, parity);

						++options;
						if (!bestOptionGroup || shortest->pathsize < bestOptionGroup->pathsize)
						{
							bestOptionGroup = shortest;
							bestOption1Box = temp;
							saveCountOnes = countones;
							saveCount2Paths = count2paths;
						}
						parity = 0; // To update count so that 2-paths in this group
									// aren't counted
					} else {
						if (!bestNonOptionGroup || shortest->pathsize < bestNonOptionGroup->pathsize)
							bestNonOptionGroup = shortest;
					}

					if (parity != (count2paths & 1))
						++count; // adjust count for correct parity of the group
				}
			}
		}
	}

	Boolean inZugzwang = EVEN(count); // if count is even we have a problem

	if ((options & 1) != 0) // Odd number of options
	{
		// Play appropriate option based on inZugzwang

		// Find middle of 1-group
		FindMidPath(bestOption1Box, saveCountOnes, saveCount2Paths, temp, playPath);
		
		// If need a parity of 1, play in the middle, otherwise play next to the middle
		if (!inZugzwang)
			// We need this group to have a parity of 0, so the initiative doesn't switch
			// Play next to the middle
			FindAnother2Path(temp, playPath);
	
	} else if (bestNonOptionGroup && bestNonOptionGroup->pathsize == 1)
	{   // Play small one from 1-group
		playPath = bestNonOptionGroup;
	}

	// ### If in zugzwang avoid ones where one could make a mistake, since opponent might! 

	TRACE(" Total = %d  so we are %sin zugswang\n", count, EVEN(count) ? "" : "not ");

	return playPath;
}   // IsInZugzwang

	

	direction d;
	PtrToTwoPathRecord bestPath;


		// No 0-boxes can be safely taken.  Take a tiny 2-path next to 
		// a 0-box if possible, since we don't know how to evaluate 0-boxes well!
		if (gList[0])
		{
			bestPath = NULL;
			FOR_LIST(temp, 0)
			{
				direction k = RandomDirection();
				d = k;
				do {
					PtrToBoxRecord box2 = Go(temp, d);
					if (!bestPath || box2->twopath->pathsize < bestPath->pathsize)
					{
						bestPath = box2->twopath;
						if (bestPath->pathsize == 1)
							break;
					}
					d = NextDirection(d);
				} while (d != k);
			}
			if (bestPath && bestPath->pathsize <= 2)
			{
				//TRACE("4--Play on a 2-path connected to an unsafe 0-box\n");
				PlayMoveOnPath(bestPath, move);
				return;
			}
		}


		else if (gList[1])
		{
			PtrToTwoPathRecord bestPath = IsInZugzwang();
			if (bestPath)
			{   //  We have to play a move on a 2's list next to an unsafe 1-box
				//  to stay out of zugzwang.
				PlayMoveOnPath(bestPath, move);
				//TRACE("5--Move on a 2-list next to an unsafe 1-box to avoid zugzwang\n");
				return;
			}
		}





