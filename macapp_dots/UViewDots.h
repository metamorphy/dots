/*
	UViewDots.h
	
	Copyright ©1991-1992 Apple Computer, Inc.
	All rights reserved

	Change History:
	
 */

#ifndef __UVIEWDOTS__
#define __UVIEWDOTS__

#ifndef __UVIEW__
#include <UView.h>
#endif

//----------------------------------------------------------------------------------------
// Forward and external classes (see Types.h for PascalObj macro)
//----------------------------------------------------------------------------------------

class PascalObj TDocumentDots;
class PascalObj TStream;

//----------------------------------------------------------------------------------------
// TViewDots
//----------------------------------------------------------------------------------------

class TViewDots: public TView
{
	public:
		TDocumentDots* fDocumentDots;
		
		// Construction/Destruction
		virtual pascal void Initialize(); // Override
		
		virtual pascal void IViewDots(TDocumentDots* itsDocument,
											TView* itsSuperView,
											const VPoint& itsLocation,
											const VPoint& itsSize);
											
		virtual pascal void DoPostCreate(TDocument* itsDocument); // Override
							   
		// Drawing
		virtual pascal void DrawEdge(int x1, int y1, int x2, int y2, Boolean blink);

		virtual pascal void Draw(const VRect& area); // Override
		
		// Commands
		virtual pascal void DoSetupMenus(); // Override
		
		virtual pascal void DoMenuCommand(CommandNumber aCommandNumber); // Override
		
		virtual pascal void DoMouseCommand(VPoint& theMouse,
									  TToolboxEvent* event,
									  CPoint hysteresis); // Override
};

//----------------------------------------------------------------------------------------
// Global initialization procedure
//----------------------------------------------------------------------------------------

extern pascal void InitUViewDots();
	// Call this routine at initialization time

#endif