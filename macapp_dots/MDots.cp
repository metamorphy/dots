//----------------------------------------------------------------------------------------
// MDots.cp
// Copyright ©1991-1992 Apple Computer, Inc.
//----------------------------------------------------------------------------------------

#ifndef __UPRINTING__
#include <UPrinting.h>
#endif

#ifndef __UMACAPPUTILITIES__
#include <UMacAppUtilities.h>
#endif

#ifndef __UERRORMGR__
#include <UErrorMgr.h>
#endif

#ifndef __UDIALOG__
#include <UDialog.h>
#endif

#ifndef __UGRIDVIEW__
#include <UGridView.h>
#endif

#ifndef __UMACAPPGLOBALS__
#include <UMacAppGlobals.h>
#endif

#ifndef __UAPPLICATIONDOTS__
#include "UApplicationDots.h"
#endif

#ifndef __UVIEWDOTS__
#include "UViewDots.h"
#endif

// Generate universal code
#pragma processor 68000


//----------------------------------------------------------------------------------------
// main: 
//----------------------------------------------------------------------------------------
#pragma segment Main

void main ()
{
	TApplicationDots	*gApplicationDots;			// Application object
	
	InitToolBox();											// Essential toolbox & utilities
	
	if (ValidateConfiguration(gConfiguration))
	{
		InitUMacApp(8);										// Initialize MacApp with 10 calls to MoreMasters
		InitUPrinting();									// Initialize the printing gear
		InitUTEView();										// Initialize the text editing unit 
		InitUDialog();										// Initialize the dialog unit
		InitUGridView();									// Initialize the GridView unit
		
		InitUViewDots();								// Initialize the application's view unit.

		gApplicationDots = new TApplicationDots;	// Allocate application object
		gApplicationDots->IApplicationDots();		//	Initialize the object
		gApplicationDots->Run();						//	Well lets run it then!
	}
	else
		StdAlert(phUnsupportedConfiguration);
} // main 

