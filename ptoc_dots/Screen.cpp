// Screen.cpp

const integer volume = 100;			/*the volume of all beeps and buzzes*/
const integer blinks = 65;				/*the number of times a new edge will flash when it appears*/
const char compsymb = 'C';			/*computer's initial*/

/* crsr : Cursor;  ### UNKNOWN TYPE */
/* str : STRING; ### UNKNOWN TYPE */
/* pt : point; ### UNKNOWN TYPE */
/* r : rect; ### UNKNOWN TYPE */
Rect moverect;					/*rectangle surrounding the portion of the screen that indicates*/
/*	whose move it is.*/
Rect quitbox;					/*the "quit" box*/
char p;

/**/
/*										SCREEN PROCEDURES										 */

/*Given box coordinates x,y returns coords of top-left point of box (i,j)*/
void ScreenLoc (integer x, integer y,
								integer& i,
								integer& j) {
	
	i = x * boxwidth;
	j = y * boxwidth;
}   /*ScreenLoc*/

/*Given the coordinates of two boxes x1, y1 and x2, y2, this will draw in*/
/*	an edge connecting the dot in the upper left hand corner of the two boxes.*/
void DrawEdge (integer x1, integer y1, integer x2, integer y2) {
	
	integer i1, j1, i2, j2, f;
	ScreenLoc(x1, y1, i1, j1);
	ScreenLoc(x2, y2, i2, j2);
	Moveto(i1, j1);
	LineTo(i2, j2);
	if (mode != 5) {
		Note(250, volume, 2);
		for (f=0; f<blinks; ++f) {
			PenMode(notpatCopy);
			Lineto(i1, j1);
			PenMode(patCopy);
			Lineto(i2, j2);
		}
	}
}   /*DrawEdge*/

/*Print the dots on the screen which form the playing board*/
void PrintDots() {
	
	integer hor, ver;
	Rectangle rect;
	
	for( hor = 1; hor <= boardsizex + 1; hor++)
		for( ver = 1; ver <= boardsizey + 1; ver++) {
			ScreenLoc(hor, ver, i, j);
			SetRect(Rectangle, i, j, i + 1, j + 1);
			PaintOval(Rectangle);
		}
}   /*PrintDots*/

/*Update a player's score on the drawing window*/
void UpdateScore (who w) {
	
	integer n;
	Rectangle rect;
	
	if (w == firstplayer) 
		n = 70;
	else
		n = 118;
	SetRect(Rectangle, n, scoreline - 9, n + 45, scoreline + 2);
	EraseRect(Rectangle);
	moveto(n, scoreline);
	writedraw(score[w]); {* CONVERSION writedraw(score[w] : 1);
}   /*UpdateScore*/

/*Update whose move it is on the drawing window*/
void UpdateMove (who w) {
	EraseRect(moverect);
	moveto(190, scoreline);
	writedraw(symbol[w]);
}   /*UpdateMove*/

void PrintInitial (integer x, integer y, who person) {
	
	integer hor, ver;
	
	ScreenLoc(x, y, hor, ver);
	hor += (boxwidth DIV 2) - 3;
	ver += (boxwidth DIV 2) + 3;
	MoveTo(hor, ver);
	TextFace([bold]);
	WriteDraw(symbol[person]);
	TextFace([]);
	Note(750, volume, mode == 5 ? 1 : 2)
	score[person]++;
	UpdateScore(person);
	totalscore++;
}   /*PrintInitial*/

void InitializeUI()
{
	hideall;
	setrect(r, 270, 20, 527, 350);
	settextrect(r);
	showtext;
	textface(set::of(eos));
	textsize(9);
	output << "Welcome to Dots and Boxes" << NL;
	output << "  by Jeff Mallett" << NL;
	output << NL;
	output << "Playing modes..." << NL;
	output << " 1   Play against the computer" << NL;
	output << "      (you move first)" << NL;
	output << " 2   Play against the computer" << NL;
	output << "      (I move first)" << NL;
	output << " 3   Play against someone else" << NL;
	output << " 4   Watch the computer play itself" << NL;
	output << " 5   Computer vs. itself quickly" << NL;
	output << NL;
	do {
		output << "Which playing mode (1/2/3/4/5)? ";
		input >> mode >> NL;
	} while (!((mode > 0) && (mode < 6)));
	do {
		output << "Number of dots across (up to " << format(maxx + 1 , 1) << ")? ";
		input >> boardsizex >> NL;
		boardsizex = boardsizex - 1;
	} while (!((boardsizex > 0) && (boardsizex <= maxx)));
	do {
		output << "Number of dots down (up to " << format(maxx + 1 , 1) << ")? ";
		input >> boardsizey >> NL;
		boardsizey = boardsizey - 1;
	} while (!((boardsizey > 0) && (boardsizey <= maxx)));
	if (mode < 3) 
	{
		output << "My Initial is " << compsymb << " for computer ... " << NL;
		do {
			output << "What is your initial? ";
			input >> p >> NL;
		} while (p == compsymb);
		if (mode == 2) 
		{
			symbol[firstplayer] = compsymb;
			symbol[secondplayer] = p;
			kind[firstplayer] = computer;
			kind[secondplayer] = human;
		}
		else
		{
			symbol[firstplayer] = p;
			symbol[secondplayer] = compsymb;
			kind[firstplayer] = human;
			kind[secondplayer] = computer;
		}
	}
	else if (mode == 3) 
	{
		output << "Enter the first player's initial: ";
		input >> symbol[firstplayer] >> NL;
		do {
			output << "Enter the second player's initial: ";
			input >> symbol[secondplayer] >> NL;
		} while (!(symbol[firstplayer] != symbol[secondplayer]));
		kind[firstplayer] = human;
		kind[secondplayer] = human;
	}
	else
	{
		symbol[firstplayer] = 'A';
		symbol[secondplayer] = 'B';
		kind[firstplayer] = computer;
		kind[secondplayer] = computer;
	}
	output << NL;
	if (mode < 4) 
	{     /*brief instructions*/
		output << "To add an edge move the mouse between" << NL;
		output << "two adjacent dots and click.  When the" << NL;
		output << "edge completes a square, the player" << NL;
		output << "receives one point and he may move" << NL;
		output << "again.  When all the squares have been" << NL;
		output << "taken the game is over and the player" << NL;
		output << "with the highest score wins." << NL;
		output << "May the best man win!" << NL;
	}
	
  /*Initialize Cursor*/
	crsr.hotspot.h = 7;
	crsr.hotspot.v = 7;
	for( i = 0; i <= 15; i ++)
	{
		crsr.data[i] = 0;
		crsr.mask[i] = 0;
	}
	for( i = 2; i <= 5; i ++)
	{
		crsr.data[i] = 256;
		crsr.mask[i] = 256;
	}
	for( i = 9; i <= 12; i ++)
	{
		crsr.data[i] = 256;
		crsr.mask[i] = 256;
	}
	crsr.data[7] = 15736;
	crsr.mask[7] = 15480;
	setcursor(crsr);
	
	randseed = tickcount - 101 * boardsizex - 1001 * boardsizey + mode;
	/*Initialize arrays*/

	/*Initialize screen*/
	setrect(r, 1, 20, 270, 360);
	setdrawingrect(r);
	showdrawing;
	PrintDots();
	textsize(9);
	moveto(10, scoreline - 13);
	writedraw("SCORE:         ", symbol[firstplayer], "              ", symbol[secondplayer]);
	UpdateScore(firstplayer);
	UpdateScore(secondplayer);
	setrect(moverect, 190, scoreline - 9, 200, scoreline + 3);
	moveto(160, scoreline - 13);
	writedraw("Player to move");
	setrect(quitbox, 90, 295, 155, 320);
	frameroundrect(quitbox, 10, 10);
	moveto(112, 310);
	writedraw("QUIT");
	UpdateMove(firstplayer);
	whoseturn = firstplayer;
}  /*InitializeUI*/

void DrawMove(direction x, int y, int d) {
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

void CheckQuit() {
	if (Button) 
	{
		getmouse(pt.h, pt.v);
		if (ptinrect(pt, quitbox)) {
			invertroundrect(quitbox, 10, 10);
			exit(0);
		}
	}
}  /*CheckQuit*/


void HumanTurn(int &x, int &y, int &d)
{ 
	do {  /*Get Legal Move*/
		do {  /*Get Legal Mouse Click*/
			while (Button)
				;
			while (~ Button)
				;
			GetMouse(h, v);
			x = h / boxwidth;
			y = v / boxwidth;
		} while (!((x >= 0) && (y >= 0) && (x <= boardsizex + 1) && (y <= boardsizey + 1)));
		h = h % boxwidth;
		v = v % boxwidth;
		badpos = false;
		if ((h < v) && (h < boxwidth - v)) 
			d = left;
		else if ((h > v) && (h < boxwidth - v)) 
			d = up;
		else if ((h > v) && (h > boxwidth - v)) 
			d = right;
		else if ((h < v) && (h > boxwidth - v)) 
			d = down;
		else
			badpos = true;
		if (badpos == false) 
			if ((x == 0) && (d != right)) 
				badpos = true;
			else if ((y == 0) && (d != down)) 
				badpos = true;
			else if ((x == boardsizex + 1) && (d != left)) 
				badpos = true;
			else if ((y == boardsizey + 1) && (d != up)) 
				badpos = true;
			else if (edge[x][y][d] == true) 
				badpos = true;
	} while (badpos);
}  /*HumanTurn*/

void GameOver()
{
    output << "GAME OVER" << NL;
    if (score[firstplayer] == score[secondplayer]) 
		output << "It's a tie!" << NL;
    else if (score[firstplayer] > score[secondplayer]) 
		output << "The first player wins!" << NL;
    else
		output << "The second player wins!" << NL;
    output << "Select the QUIT box to exit..." << NL;
    do {
		getmouse(pt);
    } while (!(ptinrect(pt, quitbox) & button));
}  /*GameOver*/

/**/
/*										MAIN PROGRAM											 */

int main() { /*MAIN PROGRAM*/
	
	InitializeUI();

	Initialize();
						
	/**/
	/*										PLAY THE GAME											 */
	
	do {
		/*New player's turn*/
		do {
			/*Next phase of player's turn*/
			UpdateMove(whoseturn);
			oldscore = score[whoseturn];
			CheckQuit()
			
			if (kind[whoseturn] == human) 
				HumanTurn(x, y, d);
			else
				ComputerTurn(x, y, d);
					
			/*Take care of the player's move*/
			DrawMove(x, y, d);
			checkneeded = false;
			AddEdge(x, y, d, whoseturn, checkneeded);
			AddEdge(NewXCoord(x, d), NewYCoord(y, d), OppositeDirection(d), whoseturn, checkneeded);
			if (pathscomputed) 
			{
				UpdatePathFrom(x, y);
				UpdatePathFrom(NewXCoord(x, d), NewYCoord(y, d));
			}
			if (checkneeded && mode != 3) 
				CheckSafety(safe);
															
        } while (!((totalscore == maxtotalscore) || (oldscore == score[whoseturn])));
        if (whoseturn == firstplayer) 
			whoseturn = secondplayer;
        else
			whoseturn = firstplayer;
    } while (totalscore != maxtotalscore);

	GameOver();
}
