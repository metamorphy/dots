/*
	UTrackerDots.h
	
	Copyright ©1991-1992 Apple Computer, Inc.
	All rights reserved

	Change History:
	
 */

#ifndef __UTRACKERDOTS__
#define __UTRACKERDOTS__

#ifndef __UCOMMAND__
#include <UCommand.h>
#endif

//----------------------------------------------------------------------------------------
// Forward and external classes (see Types.h for PascalObj macro)
//----------------------------------------------------------------------------------------

class PascalObj TDocumentDots;		

//----------------------------------------------------------------------------------------
// TTrackerDots
//----------------------------------------------------------------------------------------

class TTrackerDots: public TTracker
{
	public:
		TDocumentDots* fDocumentDots;
		
		// Construction/Destruction
		virtual pascal void Initialize(); // Override
			// Comment Required
			
		virtual pascal void ITrackerDots(TDocumentDots* itsDocumentDots,
											 TView* itsView,
											 TScroller* itsScroller,
											 const VPoint& itsMouse);
			// Comment Required
			
		// Commands			
		virtual pascal void DoIt(); // Override
			// Comment Required
			
		virtual pascal void UndoIt(); // Override
			// Comment Required
			
		virtual pascal void RedoIt(); // Override
		
		// Tracking
		virtual pascal void TrackFeedback(TrackPhase aTrackPhase,
										const VPoint& anchorPoint,
										const VPoint& previousPoint,
										const VPoint& nextPoint,
										Boolean mouseDidMove,
										Boolean turnItOn); // Override
			// Comment Required
			
		virtual pascal TTracker* TrackMouse(TrackPhase aTrackPhase,
											 VPoint& anchorPoint,
											 VPoint& previousPoint,
											 VPoint& nextPoint,
											 Boolean mouseDidMove); // Override
			// Comment Required
			
};

#endif