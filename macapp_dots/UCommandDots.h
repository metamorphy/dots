/*
	UCommandDots.h
	
	Copyright ©1991-1992 Apple Computer, Inc.
	All rights reserved

	Change History:
	
 */

#ifndef __UCOMMANDDOTS__
#define __UCOMMANDDOTS__

#ifndef __UCOMMAND__
#include <UCommand.h>
#endif

//----------------------------------------------------------------------------------------
// Forward and external classes (see Types.h for PascalObj macro)
//----------------------------------------------------------------------------------------

class PascalObj TDocumentDots;
		
//----------------------------------------------------------------------------------------
// TCommandDots
//----------------------------------------------------------------------------------------

class TCommandDots: public TCommand
{
	public:
		TDocumentDots* fDocumentDots;
		
		// Construction/Destruction
		virtual pascal void Initialize(); // Override
		
		virtual pascal void ICommandDots(CommandNumber itsCommandNumber, TDocumentDots* itsDocumentDots);
		
		// Commands
		virtual pascal void DoIt(); // Override
		
		virtual pascal void UndoIt(); // Override
		
		virtual pascal void RedoIt(); // Override
			
};

#endif
