//----------------------------------------------------------------------------------------
// UViewDots.cp
// Copyright ©1991-1992 Apple Computer, Inc.
//----------------------------------------------------------------------------------------

#ifndef __UGLOBALS__
#include "UGlobals.h"
#endif

#ifndef __UVIEWDOTS__
#include "UViewDots.h"
#endif

#ifndef __UMACAPPUTILITIES__
#include "UMacAppUtilities.h"
#endif

#ifndef __UDOCUMENTDOTS__
#include "UDocumentDots.h"
#endif

#ifndef __UTRACKERDOTS__
#include "UTrackerDots.h"
#endif

#ifndef __UMENUMGR__
#include "UMenuMgr.h"
#endif

#ifndef __FONTS__
#include "Fonts.h"
#endif


//----------------------------------------------------------------------------------------
// Constants:

const CommandNumber cCommandHandledByView = 402;

#define DRAWPOINT(r, i, j)	SetRect(r, i, j, i+1, j+1); FrameRect(r)

//========================================================================================
// Global Initialization Procedure
//========================================================================================

//----------------------------------------------------------------------------------------
// InitUViewDots: 
//----------------------------------------------------------------------------------------
#pragma segment DlgInit

pascal void InitUViewDots()
{
	// So the linker doesn't dead strip class info 
	macroDontDeadStrip(TViewDots);
} // InitUViewDots 


//========================================================================================
// CLASS TViewDots
//========================================================================================


//----------------------------------------------------------------------------------------
// TViewDots::Initialize: 
//----------------------------------------------------------------------------------------
#pragma segment AOpen

pascal void TViewDots::Initialize() // Override 
{
	inherited::Initialize();
	
	fDocumentDots = NULL;
} // TViewDots::Initialize 
	
//----------------------------------------------------------------------------------------
// TViewDots::IViewDots: 
//----------------------------------------------------------------------------------------
#pragma segment AOpen

pascal void TViewDots::IViewDots(TDocumentDots* itsDocumentDots,
										TView* itsSuperView,
						 				const VPoint& itsLocation,
						 				const VPoint& itsSize)
{
	this->IView(itsDocumentDots,itsSuperView,itsLocation,itsSize,sizeFixed, sizeFixed);
	
	fDocumentDots = itsDocumentDots;
} // TViewDots::IViewDots 

//----------------------------------------------------------------------------------------
// TViewDots::DrawEdge: 
//----------------------------------------------------------------------------------------
#pragma segment ARes

// Given the coordinates of two boxes x1, y1 and x2, y2, this will draw in
//	an edge connecting the dot in the upper left hand corner of the two boxes.
pascal void TViewDots::DrawEdge(int x1, int y1, int x2, int y2, Boolean blink)
{
	int i1, i2, j1, j2, i;
	CRect r;
	long finalTicks;
	
	ForeColor(redColor);
		ScreenLoc(x1, y1, &i1, &j1);
		ScreenLoc(x2, y2, &i2, &j2);
		MoveTo(i1, j1);
		LineTo(i2, j2);
		if (blink) {
			// ### Note(250, volume, 2)
			for (i=1; i<=kNumBlinks; i++) {
				PenMode(notPatCopy);
				LineTo(i1, j1);
				PenMode(patCopy);
				LineTo(i2, j2);
				Delay(1, finalTicks);
			}
		}
	ForeColor(blueColor);
		DRAWPOINT(r, i1, j1);
		DRAWPOINT(r, i2, j2);
	ForeColor(blackColor);
}

//----------------------------------------------------------------------------------------
// TViewDots::Draw: 
//----------------------------------------------------------------------------------------
#pragma segment ARes

pascal void TViewDots::Draw(const VRect& /* area */) // Override 
// Print the dots on the screen which form the playing board
{
	int		h, v, i, j;
	CRect	r;
	
	ForeColor(blueColor);
		for (h = 1; h <= gBoardSizeX + 1; h++)
			for (v = 1; v <= gBoardSizeY + 1; v++) {
				ScreenLoc(h, v, &i, &j);
				DRAWPOINT(r, i, j);
				SetRect(r, i, j, i+1, j+1);
				FrameRect(r);
			}
	ForeColor(blackColor);
	
	for (h=0; h<=kMaxXPlusOne; h++)
		for (v=0; v<=kMaxYPlusOne; v++) {
			if (gEdge[h, v, kRight]) DrawEdge(h, v, h+1, v, FALSE);
			if (gEdge[h, v, kDown ]) DrawEdge(h, v, h, v+1, FALSE);
		}
} // TViewDots::Draw 

//----------------------------------------------------------------------------------------
// TViewDots::DoMenuCommand: This method is overridden to handle menu items which are
// enabled when this view is in the target chain. In this example, this is true when the
// window containing this view is the active window. The inherited method should always be
// called so that MacApp can allow successor objects in the target chain (i.e. document
// and application) to handle THEIR menu items.
//----------------------------------------------------------------------------------------
#pragma segment ASelCommand

pascal void TViewDots::DoMenuCommand(CommandNumber aCommandNumber) // Override 
{
	switch (aCommandNumber) 
	{
		case cCommandHandledByView :
			SysBeep(2);
			break;
			
		default:
			inherited::DoMenuCommand(aCommandNumber);
			break;
	}
} // TViewDots::DoMenuCommand 

//----------------------------------------------------------------------------------------
// TViewDots::DoPostCreate: 
//----------------------------------------------------------------------------------------
#pragma segment AOpen
	
pascal void TViewDots::DoPostCreate(TDocument* itsDocument) // Override 
{
	inherited::DoPostCreate(itsDocument);
	
	fDocumentDots = (TDocumentDots*) itsDocument;
} // TViewDots::DoPostCreate 
	
//----------------------------------------------------------------------------------------
// TViewDots::DoSetupMenus: This method is overridden to enable menu items which
// should be enabled when this view is in the target chain. MacApp initially disables all
// menu items, then lets the objects in the target chain enable those items they handle.
//
// A view is generally in the target chain when its window is the active window, and the
// view or one of its subviews is designated as the target of that window. In this
// example, the window's resource specifies the skeleton view as the target.
// 
// The inherited method is called so that objects further up the target chain (the
// scroller, window, document and application) can set up THEIR menus.
//----------------------------------------------------------------------------------------
#pragma segment ARes
			
pascal void TViewDots::DoSetupMenus() // Override 
{
	inherited::DoSetupMenus();
	Enable(cCommandHandledByView,TRUE);
//	EnableCheck(cCommandHandledByView,TRUE,TRUE);
} // TViewDots::DoSetupMenus 

//----------------------------------------------------------------------------------------
// TViewDots::DoMouseCommand: 
//----------------------------------------------------------------------------------------
#pragma segment ASelCommand

pascal void TViewDots::DoMouseCommand(VPoint& theMouse,
								  TToolboxEvent* /* event */,
								  CPoint /* hysteresis */)	// Override
{
	TTrackerDots* aTracker = new TTrackerDots;

	aTracker->ITrackerDots(fDocumentDots, this, this->GetScroller(FALSE), theMouse);
	this->PostCommand(aTracker);
} // TViewDots::DoMouseCommand 
