//----------------------------------------------------------------------------------------
// UDotsApplication.cp
// Copyright ©1991-1992 Apple Computer, Inc.
//----------------------------------------------------------------------------------------

#ifndef __UAPPLICATIONDOTS__
#include "UApplicationDots.h"
#endif

#ifndef __UDOCUMENTDOTS__
#include "UDocumentDots.h"
#endif

#ifndef __UVIEWDOTS__
#include "UViewDots.h"
#endif

#ifndef __UMENUMGR__
#include "UMenuMgr.h"
#endif

//----------------------------------------------------------------------------------------
// Constants:

const OSType kSignature			= 'SS01';			// Application signature
		
const CommandNumber cCommandHandledByApplication =	400;

//========================================================================================
// CLASS TApplicationDots
//========================================================================================


//----------------------------------------------------------------------------------------
// TApplicationDots::IApplicationDots: 
//----------------------------------------------------------------------------------------
#pragma segment AInit

pascal void TApplicationDots::IApplicationDots()
{
	this->IApplication(kFileType,kSignature);
} // TApplicationDots::IApplicationDots 

//----------------------------------------------------------------------------------------
// TApplicationDots::DoMakeDocument: This method is overridden to return a document
// object of the appropriate class for this application. MacApp calls this method when the
// user chooose New or Open from the File menu. For applications which hande multiple
// document types, the command number can be used to discriminate between different user
// requests. The file object represents the file chosen by the user in the Standard File
// dialog.
//----------------------------------------------------------------------------------------
#pragma segment AOpen
			
pascal TDocument* TApplicationDots::DoMakeDocument(CommandNumber itsCommandNumber,
														TFile* itsFile) // Override 
{
	TDocumentDots* aDocument = new TDocumentDots;
	
	aDocument->IDocumentDots(itsFile,kSignature);
	return aDocument;
} // TApplicationDots::DoMakeDocument 

//----------------------------------------------------------------------------------------
// TApplicationDots::DoMenuCommand: This method is overridden to handle menu items
// which are enabled when this object is in the target chain. The application object is
// always in the target chain.
// 
// The inherited method is called so that MacApp can handle application-level menu items
// like the "About " menu item in the Apple menu.
//----------------------------------------------------------------------------------------
#pragma segment ASelCommand

pascal void TApplicationDots::DoMenuCommand(CommandNumber aCommandNumber) // Override 
{
	switch (aCommandNumber) 
	{
		case cCommandHandledByApplication : 
			SysBeep(2);
			break;
		default:
			inherited::DoMenuCommand(aCommandNumber);
			break;
	}
} // TApplicationDots::DoMenuCommand 

//----------------------------------------------------------------------------------------
// TApplicationDots::DoSetupMenus: This method is overridden to enable menu items
// which should be enabled when this object is in the target chain. MacApp initially
// disables all menu items, then lets the objects in the target chain enable those items
// they handle.
//
// The application object is always in the target chain, so the About menu item in the
// Apple menu and the item with command number cMenuHandledByApplication should be enabled
// even if there are no open documents.
// 
// The inherited method is called so that MacApp can enable application-level menu items
// like the "About " menu item.
//----------------------------------------------------------------------------------------
#pragma segment ARes

pascal void TApplicationDots::DoSetupMenus() // Override 
{
	inherited::DoSetupMenus();				// Always call the inherited method first

	Enable(cCommandHandledByApplication,TRUE);
} // TApplicationDots::DoSetupMenus 

