PROGRAM main (input, output);

{********************DOTSNBOXES**********************}
{* A "Dots and Boxes" game for the mac.  Copyright 1986 by Jeff Mallett	    *}
{* 							    Last revision 11/14/86							    *}
{*							  Currently 1025 lines long!							    *}
{**************************************************}

	LABEL
		1, 2;						{1 -- the computer jumps here when it has found a move}
								{2 -- the end}

	CONST
		volume = 100;			{the volume of all beeps and buzzes}
		blinks = 65;				{the number of times a new edge will flash when it appears}
		HOdiff = 8;				{Hand Out Difference: if the number of untaken boxes is less}
								{	than HOdiff, then the computer will always take a box}
								{	even if hand out possibilities exist.}
		maxx = 11;				{maximum possible number of columns}
		maxxplusone = 12;
		maxy = 11;				{maximum possible number of rows}
		maxyplusone = 12;
		boxwidth = 20;			{pixel distance between adjacent dots}
		scoreline = 290;			{distance of scores from top of screen}
		compsymb = 'C';			{computer's initial}
		markvalue = -1;			{temporary marker: when calculating a path a box}
								{	whose pathsize=markvalue is already on the path}
	TYPE
		direction = (left, up, right, down);
		who = (firstplayer, secondplayer);
		kindofplayer = (computer, human);
		pathdirection = (behind, ahead);
		Ptrtoboxrecord = ^boxrecordtype;
		pathnextarray = ARRAY[pathdirection] OF Ptrtoboxrecord;
		boxrecordtype = RECORD
				xcoord, ycoord : integer;			{location of box}
				previous, next : Ptrtoboxrecord;	{previous & next elements on edge-list}
				pathsize : integer;					{number of elements in the 2-path}
				pnext : pathnextarray;				{the two connecting 2's in the path}
			END;

	VAR
		{edge[x,y,d] is true iff box x,y has an edge in direction d}
		edge : ARRAY[0..maxxplusone, 0..maxyplusone, direction] OF boolean;
		{Ptr[x,y] is a pointer to the corresponding box-record on an edge list}
		Ptr : ARRAY[1..maxx, 1..maxy] OF Ptrtoboxrecord;
		{list[n] is a pointer to a doubly-linked list of box-records, all of which have n edges}
		list : ARRAY[0..4] OF Ptrtoboxrecord;
		{symbol[w] is the initial of player w}
		symbol : ARRAY[who] OF char;
		{kind keeps track whether or not each player is a human or a computer}
		kind : ARRAY[who] OF kindofplayer;
		{score[w] is the number of boxes player w has completed}
		score : ARRAY[who] OF integer;
		mode,						{the mode of play (see instructions), either 1,2,3 or 4}
		totalscore,				{totalscore = the sum of the two player's score}
		maxtotalscore,			{the maximum possible value of totalscore}
		boardsizex, boardsizey,	{the size of the board in squares across and down respectively}
		oldscore,					{the score of the current player before his last move}
		numofTPH,				{the number of length 2 hand out possibilities}
		numofFPH,				{the number of length 4 hand out possibilities}
		x, y, i, j, h, v			{temporary variables (for the horizontal and vertical coordinates)}
		 : integer;
		safe,						{safe is true iff there exists at least one box in list[0] or list[1]}
								{	that is not totally connected to boxes with 2 edges}
		CheckNeeded,				{CheckNeeded is true if safe needs to be re-evaluated}
		PathsComputed,			{this is true if the 2-paths have been recorded in the behind, ahead}
								{	and pathsize fields of the box-records in the edge-lists}
		badpos : boolean;			{used to evaluate whether or not the selected position is legal}
		whoseturn : who;			{whose turn it is to play}
		moverect,					{rectangle surrounding the portion of the screen that indicates}
								{	whose move it is.}
		quitbox,					{the "quit" box}
		{* r : rect; ### UNKNOWN TYPE *}
		p : char;
		k, d : direction;
		temp, temp2 : Ptrtoboxrecord;
		pdir : pathdirection;
		{* crsr : Cursor;  ### UNKNOWN TYPE *}
		{* pt : point; ### UNKNOWN TYPE *}
		{* str : STRING; ### UNKNOWN TYPE *}

{*********************************************************}
{										SCREEN PROCEDURES										 }

{Given box coordinates x,y returns coords of top-left point of box (i,j)}
	PROCEDURE ScreenLoc (x, y : integer;
									VAR i : integer;
									VAR j : integer);
	BEGIN
		i := x * boxwidth;
		j := y * boxwidth;
	END;{ScreenLoc}

{Given the coordinates of two boxes x1, y1 and x2, y2, this will draw in}
{	an edge connecting the dot in the upper left hand corner of the two boxes.}
	PROCEDURE DrawEdge (x1, y1, x2, y2 : integer);
		VAR
			i1, j1, i2, j2, f : integer;
	BEGIN {* ###
		ScreenLoc(x1, y1, i1, j1);
		ScreenLoc(x2, y2, i2, j2);
		Moveto(i1, j1);
		LineTo(i2, j2);
		IF mode <> 5 THEN
			BEGIN
				Note(250, volume, 2);
				FOR f := 1 TO blinks DO
					BEGIN
						PenMode(notpatCopy);
						Lineto(i1, j1);
						PenMode(patCopy);
						Lineto(i2, j2);
					END;
			END; *}
	END;{DrawEdge}

{Print the dots on the screen which form the playing board}
	PROCEDURE PrintDots;
		VAR
			hor, ver : integer;
		{* ###	Rectangle : rect; *}
	BEGIN
		FOR hor := 1 TO boardsizex + 1 DO
			FOR ver := 1 TO boardsizey + 1 DO
				BEGIN  {* ###
					ScreenLoc(hor, ver, i, j);
					SetRect(Rectangle, i, j, i + 1, j + 1);
					PaintOval(Rectangle); *}
				END;
	END;{PrintDots}

{Update a player's score on the drawing window}
	PROCEDURE UpdateScore (w : who);
		VAR
			n : integer;
	{* ###		Rectangle : rect; *}
	BEGIN
		IF w = firstplayer THEN
			n := 70
		ELSE
			n := 118;
{* ###		SetRect(Rectangle, n, scoreline - 9, n + 45, scoreline + 2);
		EraseRect(Rectangle);
		moveto(n, scoreline);
		writedraw(score[w]); {* CONVERSION writedraw(score[w] : 1); *}
	END;{UpdateScore}

{Update whose move it is on the drawing window}
	PROCEDURE Updatemove (w : who);
	BEGIN  {* ###
		EraseRect(moverect);
		moveto(190, scoreline);
		writedraw(symbol[w]); *}
	END;{Updatemove}

	PROCEDURE PrintInitial (x, y : integer;
									person : who);
		VAR
			hor, ver : integer;
	BEGIN
	{* ### 
		ScreenLoc(x, y, hor, ver);
		hor := hor + (boxwidth DIV 2) - 3;
		ver := ver + (boxwidth DIV 2) + 3;
		MoveTo(hor, ver);
		TextFace([bold]);
		WriteDraw(symbol[person]);
		TextFace([]);
		IF mode = 5 THEN
			Note(750, volume, 1)
		ELSE
			Note(750, volume, 2); *}
		score[person] := score[person] + 1;
		UpdateScore(person);
		totalscore := totalscore + 1;
	END;{PrintInitial}

{*********************************************************}
{										OTHER PROCEDURES										 }

{Initializes/Reinitializes the path data for a single record}
	PROCEDURE Clear (p : Ptrtoboxrecord);
	BEGIN
		p^.pathsize := 0;
		p^.pnext[ahead] := NIL;
		p^.pnext[behind] := NIL;
	END;{Clear}

{Produces a random number between 1 and n inclusive}
	FUNCTION rnd (n : integer) : integer;
	BEGIN
		rnd := trunc((random / maxint + 1) * n / 2) + 1
	END;{rnd}

{RandomDirection returns a direction chosen at random}
	FUNCTION RandomDirection : direction;
		VAR
			aRandNum : integer;
	BEGIN
		aRandNum := rnd(4);
		IF aRandNum = 1 THEN
			RandomDirection := down
		ELSE IF aRandNum = 2 THEN
			RandomDirection := right
		ELSE IF aRandNum = 3 THEN
			RandomDirection := up
		ELSE
			RandomDirection := left;
	END;{RandomDirection}

{Returns the next direction in a clockwise fashion}
	FUNCTION NextDirection (dir : direction) : direction;
	BEGIN
		IF dir = down THEN
			NextDirection := left
		ELSE
			NextDirection := succ(dir);
	END;{NextDirection}

{Given a box at x,y GetDirection will return a direction randomly such that}
{	edge[x,y,GetDirection(x,y)] = false }
	FUNCTION GetDirection (x, y : integer) : direction;
		VAR
			d, dir : direction;
	BEGIN
	{cycle through directions starting at random direction dir}
		dir := RandomDirection;
		d := NextDirection(dir);
		WHILE edge[x, y, d] = true DO
			d := NextDirection(d);
		GetDirection := d;
	END;{GetDirection}

{Returns the direction opposite to the direction passed to the function}
	FUNCTION OppositeDirection (dir : direction) : direction;
	BEGIN
		CASE dir OF
			left : 
				OppositeDirection := right;
			up : 
				OppositeDirection := down;
			right : 
				OppositeDirection := left;
			down : 
				OppositeDirection := up;
		END;
	END;{OppositeDirection}

{Given a 3-box on a 2-list, returns the path direction out of the three box}
	FUNCTION OutPathDirection (p : Ptrtoboxrecord) : pathdirection;
	BEGIN
		IF p^.pnext[ahead] <> NIL THEN
			OutPathDirection := ahead
		ELSE
			OutPathDirection := behind;
	END;{OutPathDirection}

{Swaps the pointers p1 and p2}
	PROCEDURE Swap (VAR p1, p2 : Ptrtoboxrecord);
		VAR
			temp : Ptrtoboxrecord;
	BEGIN
		temp := p1;
		p1 := p2;
		p2 := temp
	END;{Swap}

{Returns the number of edges of the box at (x,y)}
	FUNCTION NumberOfEdges (x, y : integer) : integer;
		VAR
			count : integer;
			dir : direction;
	BEGIN
		count := 0;
		FOR dir := left TO down DO
			IF edge[x, y, dir] = true THEN
				count := count + 1;
		NumberOfEdges := count;
	END;{NumberOfEdges}

{Returns true if x,y specifies a box within the boundaries.  Returns false otherwise}
	FUNCTION IsInsideBoard (x, y : integer) : boolean;
	BEGIN
		IF (x >= 1) AND (x <= boardsizex) AND (y >= 1) AND (y <= boardsizey) THEN
			IsInsideBoard := true
		ELSE
			IsInsideBoard := false;
	END;{IsInsideBoard}

{Given coordinate x, returns what the new coordinate x would be after moving in direction dir}
	FUNCTION NewXCoord (VAR x : integer;
									dir : direction) : integer;
	BEGIN
		IF dir = left THEN
			NewXCoord := x - 1
		ELSE IF dir = right THEN
			NewXCoord := x + 1
		ELSE
			NewXCoord := x;
	END;{NewXCoord}

{Given coordinate y, returns what the new coordinate y would be after moving in direction dir}
	FUNCTION NewYCoord (VAR y : integer;
									dir : direction) : integer;
	BEGIN
		IF dir = up THEN
			NewYCoord := y - 1
		ELSE IF dir = down THEN
			NewYCoord := y + 1
		ELSE
			NewYCoord := y;
	END;{NewYCoord}

{Re-evaluates safe}
	PROCEDURE CheckSafety (VAR safe : boolean);
		VAR
			temp : Ptrtoboxrecord;
			hor, ver, f : integer;
			dir : direction;
	BEGIN
		IF mode <> 3 THEN
			BEGIN
				safe := false;
				FOR f := 0 TO 1 DO
					BEGIN {Check 0&1 lists}
						temp := list[f];
						WHILE (temp <> NIL) AND (safe = false) DO
							BEGIN {Check a box on list}
								hor := temp^.xcoord;
								ver := temp^.ycoord;
								FOR dir := left TO down DO
									IF (NOT edge[hor, ver, dir]) THEN
										IF NumberOfEdges(NewXCoord(hor, dir), NewYCoord(ver, dir)) <> 2 THEN
											safe := true;
								temp := temp^.next;
							END;
					END;
			END;
	END;{CheckSafety}

{Given a pointer to a 2-box or a 3-box, computes the path from that box}
	PROCEDURE ComputeAPath (posinedgelist : Ptrtoboxrecord);
		VAR
			pathlength, n, hor, ver : integer;
			dir : direction;
			pathend, finished : boolean;
			temp, temp2 : Ptrtoboxrecord;
			pdir : pathdirection;
	BEGIN
		pathlength := -1;		{set to -1 because posinedge is counted twice}
		FOR pdir := behind TO ahead DO
			BEGIN {each iteration moves to the end of the path in the behind/ahead direction}
				temp := posinedgelist;
				REPEAT {each iteration mards one more square in the path}
					pathlength := pathlength + 1;
					temp^.pathsize := markvalue;
					pathend := true;
		{check for a continuation of the path in the pdir direction}
					finished := false;
					dir := left;
					REPEAT	{each iteration checks one direction for an adjoining 2/3-square}
						IF NOT edge[temp^.xcoord, temp^.ycoord, dir] THEN
							BEGIN
								hor := NewXCoord(temp^.xcoord, dir);
								ver := NewYCoord(temp^.ycoord, dir);
								IF IsInsideBoard(hor, ver) THEN
									BEGIN
										temp2 := Ptr[hor, ver];
										IF temp2^.pathsize <> markvalue THEN
											BEGIN {found square next to current square that hasn't been marked}
												n := NumberOfEdges(hor, ver);
												IF (n = 2) OR (n = 3) THEN
													BEGIN {include this square in path}
														temp^.pnext[pdir] := temp2;
														IF pdir = behind THEN
															temp2^.pnext[ahead] := temp
														ELSE
															temp2^.pnext[behind] := temp;
														temp := temp2;
														pathend := false
													END;
											END;
									END;
							END;{if}
						IF dir = down THEN
							finished := true
						ELSE
							dir := succ(dir);
					UNTIL finished OR (NOT pathend);
		{Either all directions have been looked at or a path continuation has been found}
				UNTIL (pathend = true);
				temp^.pnext[pdir] := NIL;
			END;{for}
	{Update pathsize for all boxes in the newly computed path}
		FOR pdir := behind TO ahead DO
			BEGIN
				temp := posinedgelist;
				REPEAT
					temp^.pathsize := pathlength;
					temp := temp^.pnext[pdir];
				UNTIL (temp = NIL) OR (temp = posinedgelist);
			END;
	END;{ComputeAPath}

{Finds all 2-paths and stores them by forming a doubly-linked list for each path}
	PROCEDURE ComputePaths;
		VAR
			posinedgelist : Ptrtoboxrecord;
	BEGIN
		posinedgelist := list[2];
		WHILE posinedgelist <> NIL DO
			BEGIN {each iteration moves one box along the edge[2] list}
				IF (posinedgelist^.pnext[ahead] = NIL) AND (posinedgelist^.pnext[behind] = NIL) THEN
					ComputeAPath(posinedgelist);			{Compute path containing this element}
				posinedgelist := posinedgelist^.next;
			END;{while}
		PathsComputed := true;
	END;{Compute Paths}

{Updates the paths that the newly changed box (at hor,ver) was in if needed}
	PROCEDURE UpdatePathFrom (hor, ver : integer);
		VAR
			count : integer;
			pdir : pathdirection;
	BEGIN
	{Update path lists}
		count := NumberOfEdges(hor, ver);
		IF (count = 2) OR (count = 3) THEN
			ComputeAPath(Ptr[hor, ver])
		ELSE IF count = 4 THEN
			Clear(Ptr[hor, ver]);
	END;{UpdatePathFrom}

{Adds an edge to box at x,y and direction d.  If the edge completes a square update score, etc.}
{If safe might need to be changed then CheckNeeded will be set to true.}
	PROCEDURE AddEdge (x, y : integer;
									d : direction;
									whoseturn : who;
									VAR CheckNeeded : boolean);
		VAR
			count : integer;
			temp : Ptrtoboxrecord;
			pdir : pathdirection;
	BEGIN
		edge[x, y, d] := true;
		IF (x > 0) AND (y > 0) AND (x <= boardsizex) AND (y <= boardsizey) THEN
			BEGIN
				count := NumberOfEdges(x, y);
				IF count = 4 THEN
					PrintInitial(x, y, whoseturn);

		{Take Ptr[x,y] off of old edge list}
				IF Ptr[x, y]^.previous <> NIL THEN
					Ptr[x, y]^.previous^.next := Ptr[x, y]^.next
				ELSE
					list[count - 1] := Ptr[x, y]^.next;
				IF Ptr[x, y]^.next <> NIL THEN
					Ptr[x, y]^.next^.previous := Ptr[x, y]^.previous;

		{Add Ptr[x,y] to the beginning of a new list}
				Ptr[x, y]^.previous := NIL;
				Ptr[x, y]^.next := list[count];
				list[count] := Ptr[x, y];
				IF Ptr[x, y]^.next <> NIL THEN
					Ptr[x, y]^.next^.previous := Ptr[x, y];

		{Update safe if necessary}
				IF (safe = true) AND (count = 2) THEN
					CheckNeeded := true;
				IF (count = 3) AND (list[2] = NIL) THEN
					safe := true;
			END;
	END;{AddEdge}

{Picks the first box it can find with n edges and returns the x and y coordinates of that box}
{	along with a direction (picked randomly) in which there is no edge. }
	PROCEDURE PlayFromList (n : integer;
									VAR x, y : integer;
									VAR d : direction);
	BEGIN
		x := list[n]^.xcoord;
		y := list[n]^.ycoord;
		d := GetDirection(x, y);
	END;{PlayFromList}

{Returns the x and y coordinates of a box in the smallest 2-path.}
{Assumes that paths have been computed!}
	PROCEDURE MinimumTwoPath (VAR x, y : integer);
		VAR
			minvalue : integer;
			temp : Ptrtoboxrecord;
	BEGIN
	{find minimum pathlength on 2-list}
		minvalue := maxx * maxy;
		temp := list[2];
		WHILE temp <> NIL DO
			BEGIN
				IF temp^.pathsize < minvalue THEN
					minvalue := temp^.pathsize;
				temp := temp^.next;
			END;
	{return x and y coordinates of minimum pathlength}
		temp := list[2];
		WHILE temp^.pathsize > minvalue DO
			temp := temp^.next;
		x := temp^.xcoord;
		y := temp^.ycoord;
	END;{MinimumTwoPath}

{Given a handout type (either 2 or 4) and  that every box on the 3-list is}
{	either a 2 or hand out, HandOut finds a hand out of the indicated type in}
{	the 3-list and sets hor, ver, and dir for the move which would do this}
{	hand out.}
	PROCEDURE HandOut (HOtype : integer;
									VAR x, y : integer;
									VAR d : direction);
		VAR
			hor, ver : integer;
			temp, temp2 : Ptrtoboxrecord;
			pdir : pathdirection;
			dir : direction;
			found : boolean;
	BEGIN
		found := false;
		temp := list[3];
		WHILE (temp <> NIL) AND (NOT found) DO
			BEGIN
				IF temp^.pathsize = HOtype THEN
					BEGIN{handout}
						found := true;
						pdir := OutPathDirection(temp);
						temp2 := temp^.pnext[pdir];
						x := temp2^.xcoord;
						y := temp2^.ycoord;
						FOR dir := left TO down DO
							IF NOT edge[x, y, dir] THEN
								BEGIN
									hor := NewXCoord(x, dir);
									ver := NewYCoord(y, dir);
									IF NOT IsInsideBoard(hor, ver) THEN
										d := dir
									ELSE IF temp <> Ptr[hor, ver] THEN
										d := dir;
								END;
					END;{if}
				temp := temp^.next;
			END;{while}
	END;{HandOut}

{Given hor,ver coordinates of a 2-box on a 2-list of length two, returns the}
{	correct direction that a new edge should be placed.}
	FUNCTION HardHeartedHandout (hor, ver : integer) : direction;
		VAR
			x, y : integer;
			dir : direction;
	BEGIN
		FOR dir := left TO down DO
			IF NOT edge[hor, ver, dir] THEN
				BEGIN
					x := NewXCoord(hor, dir);
					y := NewYCoord(ver, dir);
					IF NumberOfEdges(x, y) = 2 THEN
						HardHeartedHandout := dir;
				END;
	END;{HardHeartedHandout}


{*********************************************************}
{										MAIN PROGRAM											 }

BEGIN {MAIN PROGRAM}
{Set up}
	HideAll;
	setRect(r, 270, 20, 527, 350);
	setTextRect(r);
	ShowText;
	TextFace([]);
	TextSize(9);
	writeln('Welcome to Dots and Boxes');
	writeln('  by Jeff Mallett');
	writeln;
	writeln('Playing modes...');
	writeln(' 1   Play against the computer');
	writeln('      (you move first)');
	writeln(' 2   Play against the computer');
	writeln('      (I move first)');
	writeln(' 3   Play against someone else');
	writeln(' 4   Watch the computer play itself');
	writeln(' 5   Computer vs. itself quickly');
	writeln;
	REPEAT
		write('Which playing mode (1/2/3/4/5)? ');
		readln(mode);
	UNTIL (mode > 0) AND (mode < 6);
	REPEAT
		write('Number of dots across (up to ', maxx + 1 : 1, ')? ');
		readln(boardsizex);
		boardsizex := boardsizex - 1;
	UNTIL (boardsizex > 0) AND (boardsizex <= maxx);
	REPEAT
		write('Number of dots down (up to ', maxx + 1 : 1, ')? ');
		readln(boardsizey);
		boardsizey := boardsizey - 1;
	UNTIL (boardsizey > 0) AND (boardsizey <= maxx);
	IF mode < 3 THEN
		BEGIN
			writeln('My Initial is ', compsymb, ' for computer ... ');
			REPEAT
				write('What is your initial? ');
				readln(p)
			UNTIL p <> compsymb;
			IF mode = 2 THEN
				BEGIN
					symbol[firstplayer] := compsymb;
					symbol[secondplayer] := p;
					kind[firstplayer] := computer;
					kind[secondplayer] := human;
				END
			ELSE
				BEGIN
					symbol[firstplayer] := p;
					symbol[secondplayer] := compsymb;
					kind[firstplayer] := human;
					kind[secondplayer] := computer;
				END;
		END
	ELSE IF mode = 3 THEN
		BEGIN
			write('Enter the first player''s initial: ');
			readln(symbol[firstplayer]);
			REPEAT
				write('Enter the second player''s initial: ');
				readln(symbol[secondplayer]);
			UNTIL symbol[firstplayer] <> symbol[secondplayer];
			kind[firstplayer] := human;
			kind[secondplayer] := human;
		END
	ELSE
		BEGIN
			symbol[firstplayer] := 'A';
			symbol[secondplayer] := 'B';
			kind[firstplayer] := computer;
			kind[secondplayer] := computer;
		END;
	writeln;
	IF mode < 4 THEN
		BEGIN {brief instructions}
			writeln('To add an edge move the mouse between');
			writeln('two adjacent dots and click.  When the');
			writeln('edge completes a square, the player');
			writeln('receives one point and he may move');
			writeln('again.  When all the squares have been');
			writeln('taken the game is over and the player');
			writeln('with the highest score wins.');
			writeln('May the best man win!');
		END;

  {Initialize Cursor}
	crsr.hotSpot.h := 7;
	crsr.hotSpot.v := 7;
	FOR i := 0 TO 15 DO
		BEGIN
			crsr.data[i] := 0;
			crsr.mask[i] := 0;
		END;
	FOR i := 2 TO 5 DO
		BEGIN
			crsr.data[i] := 256;
			crsr.mask[i] := 256;
		END;
	FOR i := 9 TO 12 DO
		BEGIN
			crsr.data[i] := 256;
			crsr.mask[i] := 256;
		END;
	crsr.data[7] := 15736;
	crsr.mask[7] := 15480;
	SetCursor(crsr);

	randSeed := TickCount - 101 * boardsizex - 1001 * boardsizey + mode;
{Initialize arrays}
	FOR i := 0 TO boardsizex + 1 DO
		FOR j := 0 TO boardsizey + 1 DO
			FOR k := left TO down DO
				edge[i, j, k] := false;
{Initialize list}
	new(list[0]);
	list[0]^.previous := NIL;
	temp := list[0];
	FOR i := 1 TO 4 DO
		list[i] := NIL;
	FOR i := 1 TO boardsizex DO
		FOR j := 1 TO boardsizey DO
			BEGIN
				Ptr[i, j] := temp;
				Clear(temp);
				IF (i = boardsizex) AND (j = boardsizey) THEN
					temp^.next := NIL
				ELSE
					BEGIN
						new(temp^.next);
						temp^.next^.previous := temp;
						temp := temp^.next;
					END;
			END;
     {mix up list elements randomly}
	FOR i := 1 TO boardsizex DO
		FOR j := 1 TO boardsizey DO
			swap(Ptr[i, j], Ptr[rnd(boardsizex), rnd(boardsizey)]);
	FOR i := 1 TO boardsizex DO
		FOR j := 1 TO boardsizey DO
			BEGIN
				Ptr[i, j]^.xcoord := i;
				Ptr[i, j]^.ycoord := j;
			END;

{Initialize variables}
	safe := true;
	PathsComputed := false;
	maxtotalscore := boardsizex * boardsizey;
	totalscore := 0;
	score[firstplayer] := 0;
	score[secondplayer] := 0;

{Initialize screen}
	setRect(r, 1, 20, 270, 360);
	setDrawingRect(r);
	ShowDrawing;
	PrintDots;
	TextSize(9);
	moveto(10, scoreline - 13);
	writedraw('SCORE:         ', symbol[firstplayer], '              ', symbol[secondplayer]);
	UpdateScore(firstplayer);
	UpdateScore(secondplayer);
	setRect(moverect, 190, scoreline - 9, 200, scoreline + 3);
	moveto(160, scoreline - 13);
	writedraw('Player to move');
	setRect(quitbox, 90, 295, 155, 320);
	FrameRoundRect(quitbox, 10, 10);
	moveto(112, 310);
	writedraw('QUIT');
	UpdateMove(firstplayer);
	whoseturn := firstplayer;


{*********************************************************}
{										PLAY THE GAME											 }

	REPEAT
  {New player's turn}
		REPEAT
     {Next phase of player's turn}
			UpdateMove(whoseturn);
			oldscore := score[whoseturn];
			IF button THEN
				BEGIN
					GetMouse(pt.h, pt.v);
					IF PtInRect(pt, quitbox) THEN
						GOTO 2;
				END;

			IF kind[whoseturn] = human THEN
				BEGIN{HUMAN'S TURN}
					REPEAT{Get Legal Move}
						REPEAT{Get Legal Mouse Click}
							WHILE button DO
								;
							WHILE NOT button DO
								;
							GetMouse(h, v);
							x := h DIV boxwidth;
							y := v DIV boxwidth
						UNTIL (x >= 0) AND (y >= 0) AND (x <= boardsizex + 1) AND (y <= boardsizey + 1);
						h := h MOD boxwidth;
						v := v MOD boxwidth;
						badpos := false;
						IF (h < v) AND (h < boxwidth - v) THEN
							d := left
						ELSE IF (h > v) AND (h < boxwidth - v) THEN
							d := up
						ELSE IF (h > v) AND (h > boxwidth - v) THEN
							d := right
						ELSE IF (h < v) AND (h > boxwidth - v) THEN
							d := down
						ELSE
							badpos := true;
						IF badpos = false THEN
							IF (x = 0) AND (d <> right) THEN
								badpos := true
							ELSE IF (y = 0) AND (d <> down) THEN
								badpos := true
							ELSE IF (x = boardsizex + 1) AND (d <> left) THEN
								badpos := true
							ELSE IF (y = boardsizey + 1) AND (d <> up) THEN
								badpos := true
							ELSE IF edge[x, y, d] = true THEN
								badpos := true;
					UNTIL badpos = false;
				END{HUMAN'S TURN}

			ELSE
				BEGIN{COMPUTER'S TURN}
            {generate x, y, and d for computer's move}
					IF list[3] = NIL THEN
						IF (list[0] = NIL) AND (list[1] = NIL) THEN
							BEGIN{must pick a 2-2 square}
								IF NOT PathsComputed THEN
									ComputePaths;
								MinimumTwoPath(x, y);
								IF Ptr[x, y]^.pathsize = 2 THEN
									d := HardHeartedHandout(x, y)
								ELSE
									d := GetDirection(x, y);
								GOTO 1;
							END{if}
						ELSE {  (list[0]<>nil or list[1]<>nil) and list[3]=nil  }
							BEGIN
				{while not at end of 0-list do}
				{	if any legal adjacent element<>2 or is outside board then take it}
				{	try next element}
								temp := list[0];
								WHILE temp <> NIL DO
									BEGIN
										k := RandomDirection;
										d := k;
										REPEAT
											i := NewXCoord(temp^.xcoord, d);
											j := NewYCoord(temp^.ycoord, d);
											IF NOT IsInsideBoard(i, j) OR (NumberOfEdges(i, j) <> 2) THEN
												BEGIN
													x := temp^.xcoord;
													y := temp^.ycoord;
													GOTO 1;
												END;{if}
											d := NextDirection(d);
										UNTIL d = k;
										temp := temp^.next;
									END;{while}

				{while not at end of 1-list do}
				{	if any legal adjacent element<>2 or is outside board then take it}
				{	try next element}
								temp := list[1];
								WHILE temp <> NIL DO
									BEGIN
										k := RandomDirection;
										d := k;
										REPEAT
											IF NOT edge[temp^.xcoord, temp^.ycoord, d] THEN
												BEGIN
													i := NewXCoord(temp^.xcoord, d);
													j := NewYCoord(temp^.ycoord, d);
													IF NOT IsInsideBoard(i, j) OR (NumberOfEdges(i, j) <> 2) THEN
														BEGIN
															x := temp^.xcoord;
															y := temp^.ycoord;
															GOTO 1;
														END;{if}
												END;{if}
											d := NextDirection(d);
										UNTIL d = k;
										temp := temp^.next;
									END;{while}
				{all bad choices: all 0's and 1's are adjacent to 2's}
				{now count how many 2's in a row-- if one, take it}
				{                                                  -- if two, take it (put | between 2's)}
								IF NOT PathsComputed THEN
									ComputePaths;
								MinimumTwoPath(x, y);
								IF Ptr[x, y]^.pathsize = 2 THEN
									d := HardHeartedHandout(x, y)
								ELSE
									d := GetDirection(x, y);
								GOTO 1;
							END
					ELSE { list[3]<>nil }
						BEGIN
							IF (safe) OR (list[2] = NIL) OR (maxtotalscore - totalscore < HOdiff) THEN
								BEGIN
									PlayFromList(3, x, y, d);
									GOTO 1;
								END;
			  {unsafe & list[2]<>nil & list[3]<>nil}
				{while not at end of 3-list do}
				{	if the box connected to the 3-box is not a 2-box (or is outside the board)}
				{		then take it }
							temp := list[3];
							WHILE temp <> NIL DO
								BEGIN
									d := GetDirection(temp^.xcoord, temp^.ycoord);
									i := NewXCoord(temp^.xcoord, d);
									j := NewYCoord(temp^.ycoord, d);
									IF (NumberOfEdges(i, j) <> 2) THEN
										BEGIN
											x := temp^.xcoord;
											y := temp^.ycoord;
											GOTO 1;
										END;{if}
									temp := temp^.next;
								END;{while}
  			{unsafe & list[3]<>nil & all 3-boxes are part of 2-lists}
			{Take all 3's that will not ruin handout possibilities}
							IF NOT PathsComputed THEN
								ComputePaths;
							temp := list[3];
							WHILE temp <> NIL DO
								BEGIN
				{if connecting 2-path is not of length 2 or 4 then take it}
									IF (temp^.pathsize <> 2) AND (temp^.pathsize <> 4) THEN
										BEGIN
											x := temp^.xcoord;
											y := temp^.ycoord;
											d := GetDirection(x, y);
											GOTO 1;
										END;{if}
				{if 2-path looks like 3-2-2-2 then take it}
									IF temp^.pathsize = 4 THEN
										BEGIN
											pdir := OutPathDirection(temp);
											temp2 := temp^.pnext[pdir]^.pnext[pdir]^.pnext[pdir];
											IF NumberOfEdges(temp2^.xcoord, temp2^.ycoord) = 2 THEN
												BEGIN
													x := temp^.xcoord;
													y := temp^.ycoord;
													d := GetDirection(x, y);
													GOTO 1;
												END;
										END;
									temp := temp^.next;
								END;{while}
			{Now all 3-paths look like 3-2 or 3-2-2-3.  These both are	}
			{	are hand out possibilities...									}
			{									 __ __		 __ __				}
			{		2-path		    				|__     | -> |__ __ |			}
			{							__ __ __ __		__ __ __ __		}
			{		4-path			    |	__ __ __ __ | -> |__ __|__ __ |		}
			{It is possible to have more than one handout available so	}
			{	 count the number of each kind of handout possibility.		}
							numofTPH := 0;
							numofFPH := 0;
							temp := list[3];
							WHILE temp <> NIL DO
								BEGIN
									IF temp^.pathsize = 2 THEN
										numofTPH := numofTPH + 1
									ELSE
										numofFPH := numofFPH + 1;
									temp := temp^.next;
								END;{while}
							numofFPH := numofFPH DIV 2;
							temp := list[3];
							IF numofTPH + numofFPH = 1 THEN
								BEGIN{only one possibility}
									IF numofTPH = 0 THEN
										BEGIN{do a FPH}
											HandOut(4, x, y, d);
											GOTO 1;
										END
									ELSE IF numofFPH = 0 THEN
										BEGIN{do aTPH}
											HandOut(2, x, y, d);
											GOTO 1;
										END;
								END;
			{More than one hand outs available so grab another square first}
							IF numofFPH <> 0 THEN
								BEGIN{take the square from a length 4 2-list}
									temp := list[3];
									WHILE temp^.pathsize <> 4 DO
										temp := temp^.next;
									x := temp^.xcoord;
									y := temp^.ycoord;
									d := GetDirection(x, y);
									GOTO 1;
								END
							ELSE{more than one length two hand outs available so grab from there}
								BEGIN
									PlayFromList(3, x, y, d);
									GOTO 1;
								END;
						END;{else}
				END;{COMPUTER'S TURN}
1 :		  {computer has determined his move (x,y,d)}

	{Take care of the player's move}
			CASE d OF
				left : 
					DrawEdge(x, y, x, y + 1);
				up : 
					DrawEdge(x, y, x + 1, y);
				right : 
					DrawEdge(x + 1, y, x + 1, y + 1);
				down : 
					DrawEdge(x, y + 1, x + 1, y + 1);
			END;
			CheckNeeded := false;
			AddEdge(x, y, d, whoseturn, CheckNeeded);
			AddEdge(NewXCoord(x, d), NewYCoord(y, d), OppositeDirection(d), whoseturn, CheckNeeded);
			IF PathsComputed THEN
				BEGIN
					UpdatePathFrom(x, y);
					UpdatePathFrom(NewXCoord(x, d), NewYCoord(y, d));
				END;
			IF CheckNeeded THEN
				Checksafety(safe);

		UNTIL (totalscore = maxtotalscore) OR (oldscore = score[whoseturn]);
		IF whoseturn = firstplayer THEN
			whoseturn := secondplayer
		ELSE
			whoseturn := firstplayer;
	UNTIL totalscore = maxtotalscore;
	writeln('GAME OVER');
	IF score[firstplayer] = score[secondplayer] THEN
		writeln('It''s a tie!')
	ELSE IF score[firstplayer] > score[secondplayer] THEN
		writeln('The first player wins!')
	ELSE
		writeln('The second player wins!');
	writeln('Select the QUIT box to exit...');
	REPEAT
		GetMouse(pt);
	UNTIL PtInRect(pt, quitbox) AND button;
2 :{the end!}
	InvertRoundRect(quitbox, 10, 10);
END.