/*
	UApplicationDots.h
	
	Copyright ©1991-1992 Apple Computer, Inc.
	All rights reserved

	Change History:
	
*/

#ifndef __UAPPLICATIONDOTS__
#define __UAPPLICATIONDOTS__

#ifndef __UAPPLICATION__
#include <UApplication.h>
#endif

//----------------------------------------------------------------------------------------
// TApplicationDots
//----------------------------------------------------------------------------------------

class TApplicationDots: public TApplication
{
	public:
		
		// Construction/Destruction
		virtual pascal void IApplicationDots();
		
		// Documents
		virtual pascal TDocument* DoMakeDocument(CommandNumber itsCommandNumber, TFile* itsFile );	//	Override
		
		// Commands
		virtual pascal void DoSetupMenus(); // Override		
		virtual pascal void DoMenuCommand(CommandNumber aCommandNumber); // Override
		
};	

#endif
