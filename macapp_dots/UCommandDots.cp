//----------------------------------------------------------------------------------------
// UCommandDots.cp
// Copyright ©1991-1992 Apple Computer, Inc.
//----------------------------------------------------------------------------------------

#ifndef __UCOMMANDDOTS__
#include "UCommandDots.h"
#endif

#ifndef __UDOCUMENTDOTS__
#include "UDocumentDots.h"
#endif


//========================================================================================
// CLASS TCommandDots
//========================================================================================


//----------------------------------------------------------------------------------------
// TCommandDots::Initialize: 
//----------------------------------------------------------------------------------------
#pragma segment ASelCommand

pascal void TCommandDots::Initialize() // Override 
{
	inherited::Initialize();
	
	fDocumentDots = NULL;
} // TCommandDots::Initialize 

//----------------------------------------------------------------------------------------
// TCommandDots::ICommandDots: 
//----------------------------------------------------------------------------------------
#pragma segment ASelCommand

pascal void TCommandDots::ICommandDots(CommandNumber itsCommandNumber,
												TDocumentDots* itsDocumentDots)
{
	this->ICommand(itsCommandNumber,itsDocumentDots,kCanUndo,kCausesChange,itsDocumentDots);
	fDocumentDots = itsDocumentDots;
} // TCommandDots::ICommandDots 

//----------------------------------------------------------------------------------------
// TCommandDots::DoIt: 
//----------------------------------------------------------------------------------------
#pragma segment ADoCommand

pascal void TCommandDots::DoIt() // Override 
{
	SysBeep(2);
} // TCommandDots::DoIt 

//----------------------------------------------------------------------------------------
// TCommandDots::RedoIt: 
//----------------------------------------------------------------------------------------
#pragma segment ADoCommand

pascal void TCommandDots::RedoIt(void) // override 
{
	inherited::RedoIt();	// TCommand::RedoIt simply calls DoIt, but this is not
                            // always adequate
} // TCommandDots::RedoIt 

//----------------------------------------------------------------------------------------
// TCommandDots::UndoIt: 
//----------------------------------------------------------------------------------------
#pragma segment ADoCommand

pascal void TCommandDots::UndoIt() // Override 
{
	SysBeep(2);
	SysBeep(2);
} // TCommandDots::UndoIt 
