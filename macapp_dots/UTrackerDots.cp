//----------------------------------------------------------------------------------------
// UTrackerDots.cp
// Copyright ©1991-1992 Apple Computer, Inc.
//----------------------------------------------------------------------------------------

#ifndef __UTRACKERDOTS__
#include "UTrackerDots.h"
#endif

#ifndef __UDOCUMENTDOTS__
#include "UDocumentDots.h"
#endif

//----------------------------------------------------------------------------------------
// Constants:

const CommandNumber cTrackerCommand = 403;


//========================================================================================
// CLASS TTrackerDots
//========================================================================================


//----------------------------------------------------------------------------------------
// TTrackerDots::Initialize: 
//----------------------------------------------------------------------------------------
#pragma segment foo

pascal void TTrackerDots::Initialize() // override 
{
	inherited::Initialize();

	fDocumentDots = NULL;
} // TTrackerDots::Initialize 

//----------------------------------------------------------------------------------------
// TTrackerDots::ITrackerDots: 
//----------------------------------------------------------------------------------------
#pragma segment foo

pascal void TTrackerDots::ITrackerDots(TDocumentDots* itsDocumentDots,
											TView* itsView,
											TScroller* itsScroller,
											const VPoint& itsMouse)
{
	this->ITracker(cTrackerCommand,itsDocumentDots,kCanUndo,kDoesNotCauseChange, NULL, 
					itsView, itsScroller, itsMouse);
	fDocumentDots = itsDocumentDots;
} // TTrackerDots::ITrackerDots 

//----------------------------------------------------------------------------------------
// TTrackerDots::DoIt: 
//----------------------------------------------------------------------------------------
#pragma segment ADoCommand

pascal void TTrackerDots::DoIt() // override 
{
	SysBeep(2);
} // TTrackerDots::DoIt 

//----------------------------------------------------------------------------------------
// TTrackerDots::TrackFeedback: 
//----------------------------------------------------------------------------------------
#pragma segment ADoCommand

pascal void TTrackerDots::TrackFeedback(TrackPhase aTrackPhase,
										const VPoint& anchorPoint,
										const VPoint& previousPoint,
										const VPoint& nextPoint,
										Boolean mouseDidMove,
										Boolean turnItOn) // override 
{
	inherited::TrackFeedback(aTrackPhase, anchorPoint, previousPoint, nextPoint, mouseDidMove, turnItOn); 
} // TTrackerDots::TrackFeedback 


//----------------------------------------------------------------------------------------
// TTrackerDots::TrackMouse: 
//----------------------------------------------------------------------------------------
#pragma segment ADoCommand

pascal TTracker* TTrackerDots::TrackMouse(TrackPhase aTrackPhase,
											 VPoint& anchorPoint,
											 VPoint& previousPoint,
											 VPoint& nextPoint,
											 Boolean mouseDidMove) // override 
{
	return inherited::TrackMouse(aTrackPhase,anchorPoint, previousPoint, nextPoint,mouseDidMove);
} // TTrackerDots::TrackMouse 

//----------------------------------------------------------------------------------------
// TTrackerDots::RedoIt: 
//----------------------------------------------------------------------------------------
#pragma segment ADoCommand

pascal void TTrackerDots::RedoIt() // override 
{
	inherited::RedoIt();	// TCommand::ReDoit simply calls DoIt, but this is not
							// always adequate
} // TTrackerDots::RedoIt 

//----------------------------------------------------------------------------------------
// TTrackerDots::UndoIt: 
//----------------------------------------------------------------------------------------
#pragma segment ADoCommand

pascal void TTrackerDots::UndoIt() // override 
{
	SysBeep(2);
	SysBeep(2);
} // TTrackerDots::UndoIt 

