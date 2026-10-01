# Dots and Boxes

A [Dots and Boxes](https://en.wikipedia.org/wiki/Dots_and_boxes) player by Jeff Mallett, written in Lightspeed Pascal for the Macintosh in 1986 and ported to C++ in 2001, when it won the [MacTech Magazine](https://en.wikipedia.org/wiki/MacTech) Programmer's Challenge.

Players take turns drawing a line between two adjacent dots. Completing the fourth side of a box claims it and earns another turn, and the player with the most boxes wins.

```text
--- : 2-path of length 1-2
+++ : 2-path of length 3+
  O : 1-box
  # : Cut

23A. 2 connected unsafe 1-groups

 --- O --- O ---
     +     +
     +     +


 --- O -#- O ---   |   -#- O --- O ---
     +     +       |       +     +
     +     +       |       +     +
                   |
                   |
    +++   +++      |         +++ O ---
                   |             +
                   |             +
P =0(00)+0(00) = 0        P = 1 (11)

   = Parity 1           = Parity 0
```

This is one of the cases from Jeff's June 2001 notes ([`Dots.txt`](dots_code_2001-06-08/Dots.txt)) for the look-ahead code in `Zugzwang.cpp`. It shows two 1-boxes (`O`) joined to each other and to the rest of the board by 2-paths, and the parity that results from cutting (`#`) in each of two places.

## History

The git history follows the code from 1986 to 2001, one folder per stage:

1. [`think_pascal_dots/`](think_pascal_dots/): the original game in Lightspeed Pascal (later THINK Pascal). `BoxesXI.pas` is dated November 6, 1986 and `BoxesXII.pas`, which adds a Quit box and a custom cursor, November 14, 1986.
2. [`macapp_dots/`](macapp_dots/): an unfinished hand port to Apple's MacApp 3.0 C++ framework from around 1992. It draws the board but has no computer player. `PascalToCPlus.txt` is the MPW search-and-replace script used to start the conversion, and `Boxes Translate.pas` is its output.
3. [`ptoc_dots/`](ptoc_dots/): the May 2001 machine translation. `Boxes.pas` is BoxesXII prepared for the PTOC Pascal-to-C translator, and `Boxes.cpp` and `Screen.cpp` are the translated engine and drawing code.
4. [`cdots/`](cdots/): a Visual C++ 6.0 MFC program used to run and develop the translated code on Windows, as of May 30, 2001. The `Boxes.cpp` and `Screen.cpp` it built were not saved in this folder.
5. [`dots_code_2001-06-08/`](dots_code_2001-06-08/): the same Windows program on June 8, 2001, with the contest interface (`Dots.h`), notes on 2-path parity (`Dots.txt`), and `Zugzwang.cpp`, the look-ahead code taken out of the contest entry to save time.
6. [`mactech/DotsChallenge/`](mactech/DotsChallenge/): the contest entry, a CodeWarrior project with `Boxes.cpp` (the engine) and `Screen.cpp` (drawing). It matches the code MacTech published, except that the cleanup in `TermDots` is commented out in this copy.

## The 2001 Programmer's Challenge

The [challenge](http://preserve.mactech.com/articles/mactech/Vol.17/17.06/Jun01Challenge/index.html) ran in the June 2001 issue and the [results](http://preserve.mactech.com/articles/mactech/Vol.17/17.09/Sep01Challenge/index.html) in September 2001. From the results article, by Bob Boonstra:

> After a Challenge absence of more than three years, Jeff Mallett (Boulder Creek, CA) returns to take first place in the June Dots Challenge. The object of this Challenge was to win a round-robin tournament of the game Dots (or Dots and Boxes). Dots is played on an NxN grid where players take turns connecting adjacent dots horizontally and vertically to enclose boxes. The player capturing the most boxes wins the game. The Challenge was actually scored based on minimizing the number of boxes captured by the opposing player, incorporating the usual efficiency requirement by adding a penalty of 1% for each millisecond of execution time. Solutions were also required to display the game state and the current score after each move.
>
> Jeff maintains four doubly-linked lists of boxes in gList, one list for boxes with 0 edges (0-boxes), another for boxes with 1 edge (1-boxes), etc. He also maintains a data structure called a 2-path (TwoPathRecordType), which is a sequence of connected boxes each of which has two or three existing edges (2-boxes or 3-boxes, respectively).
>
> The heavy lifting is done in the ComputerTurn routine, and I'll try to describe the logic. If there are no 3-boxes, the code looks to see if all unfilled boxes are 2-boxes. If so, any move is going to give a box to the opponent, so Jeff picks the move that gives the minimum away to the opponent. Otherwise, he selects a 0-box, or (as a second choice) a 1-box, provided it is "safe", where an "unsafe" box is one for which adding an edge creates a 3-box.
>
> If there are 3-boxes, and there are safe places to move next, Jeff takes the 3-box and plays again. If the open edge of the 3-box is at the board boundary, he takes the box. Otherwise, he takes as many 3-boxes as he can while saving the moves that give the opponent a square ("handouts"). If a move that gives a handout captures half or more of the remaining boxes, Jeff makes that move. And finally, if forced, he makes a move that gives the opponent the smallest sequence of boxes.
>
> As the comments in Jeff's code indicate, his solution is actually based on 15-year-old code, translated from Pascal into C/C++ for the Challenge. Jeff mentions that the time penalty in the problem caused him to significantly "dumb down" the program, removing enough of the look-ahead code to make it a fast, if mediocre, player.
>
> The second-place entry, from Greg Sadetsky, is based (with permission) on a JavaScript program by UCLA Professor Thomas S. Ferguson. In addition to providing some very entertaining commentary that I wish we had the space to publish, Greg included the URL for the JavaScript code (http://www.stat.ucla.edu/~tom/Games/dots&boxes.html), as well as a page of links to other analyses of the game (http://dmoz.org/Games/Paper_and_Pencil/Dots_and_Boxes/).
>
> The table below lists, for each of the solutions submitted, the number of cells captured by each solution in the tournament, the number of cells captured by the opposing player, the execution time in milliseconds, and the score earned by each solution (with lower scores being better). The table also includes the code and data size for each solution, and the programming language used. As usual, the number in parentheses after the entrant's name is the total number of Challenge points earned in all Challenges prior to this one.

| Name | Player cells | Opponent cells | Time (msec) | Score | Code | Data | Lang |
|---|---:|---:|---:|---:|---:|---:|---|
| **Jeff Mallett (94)** | **3218** | **1362** | **833.7** | **2061.5** | **11572** | **908** | **C++** |
| Greg Sadetsky (14) | 3128 | 1452 | 1566.6 | 2753.8 | 17192 | 1936 | C |
| Ernst Munter (751) | 2529 | 2010 | 721.4 | 3009.9 | 10684 | 586 | C++ |
| Tom Saxton (185) | 1914 | 2666 | 3262.5 | 8777.5 | 7220 | 581 | C++ |
| Randy Boring (142) | 668 | 3422 | 53118.1 | 145423.8 | 17676 | 498 | C |
| T. R. | 1752 | 2297 | 367.1 | 2962.6 | 5188 | 307 | C++ |

[`mactech/`](mactech/) also has the challenge as it was emailed to contestants (`Challenge.txt`), both articles saved as PDFs with `.webloc` links, and Bob Boonstra's test code (`2001_06_TestCode/`), whose `YourCode/` folder holds his sample random players.

This is historical source. The Pascal targets Lightspeed/THINK Pascal and the Mac Toolbox, the MacApp port needs MPW and MacApp 3.0, the Windows programs need Visual C++ 6.0, and the contest entry needs CodeWarrior and the classic Mac OS. None of it is set up as a modern build.
