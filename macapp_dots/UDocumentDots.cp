//----------------------------------------------------------------------------------------
// UDocumentDots.cp
// Copyright ©1991-1992 Apple Computer, Inc.
//----------------------------------------------------------------------------------------


#ifndef __UDOCUMENTDOTS__
#include "UDocumentDots.h"
#endif

#ifndef __UWINDOW__
#include "UWindow.h"
#endif

#ifndef __UVIEWSERVER__
#include <UViewServer.h>
#endif

#ifndef __UPRINTING__
#include <UPrinting.h>
#endif

#ifndef __UMENUMGR__
#include <UMenuMgr.h>
#endif

#ifndef __UCOMMANDDOTS__
#include "UCommandDots.h"
#endif

#ifndef __UVIEWDOTS__
#include "UViewDots.h"
#endif

//----------------------------------------------------------------------------------------
// Constants:

const kDotsWindowID = kDefaultWindowID;

//========================================================================================
// CLASS TDocumentDots
//========================================================================================


//----------------------------------------------------------------------------------------
// TDocumentDots::Initialize: 
//----------------------------------------------------------------------------------------
#pragma segment AOpen

pascal void TDocumentDots::Initialize() // Override 
{
	inherited::Initialize();
} // TDocumentDots::Initialize 

//----------------------------------------------------------------------------------------
// TDocumentDots::IDocumentDots: 
//----------------------------------------------------------------------------------------
#pragma segment AOpen
		
pascal void TDocumentDots::IDocumentDots(TFile* itsFile,
												OSType itsCreator)
{
	this->IFileBasedDocument(itsFile,itsCreator);
} // TDocumentDots::IDocumentDots 

//----------------------------------------------------------------------------------------
// TDocumentDots::Free: 
//----------------------------------------------------------------------------------------
#pragma segment AClose

pascal void TDocumentDots::Free() // Override 
{
	inherited::Free();
} // TDocumentDots::Free 

//----------------------------------------------------------------------------------------
// TDocumentDots::FreeData: 
//----------------------------------------------------------------------------------------
#pragma segment AClose

pascal void TDocumentDots::FreeData() // Override 
{
	inherited::FreeData();
} // TDocumentDots::FreeData 

//----------------------------------------------------------------------------------------
// TDocumentDots::DoInitialState: 
//----------------------------------------------------------------------------------------
#pragma segment AOpen

pascal void TDocumentDots::DoInitialState() // Override 
{
	inherited::DoInitialState();
} // TDocumentDots::DoInitialState 

//----------------------------------------------------------------------------------------
// TDocumentDots::DoMakeViews: 
//----------------------------------------------------------------------------------------
#pragma segment AOpen

pascal void TDocumentDots::DoMakeViews(Boolean /*forPrinting*/) // Override 
{
	TWindow* aWindow = NULL;
	TStdPrintHandler* aHandler = NULL;
	TViewDots* aViewDots = NULL;

	FailNIL(aWindow = gViewServer->NewTemplateWindow(kDotsWindowID, this));

	aViewDots = (TViewDots*) (aWindow->FindSubView('SKEL'));	// Must cast because FindSubView returns TView

	aHandler = new TStdPrintHandler;
	aHandler->IStdPrintHandler(this,			// its document 
							   aViewDots,	// its view 
							   !kSquareDots,	// does not have square dots 
							   kFixedSize,		// horzontal page size is fixed 
							   kFixedSize);		// vertical page size is fixed 
} // TDocumentDots::DoMakeViews 

//----------------------------------------------------------------------------------------
// TDocumentDots::DoNeedDiskSpace: 
//----------------------------------------------------------------------------------------
#pragma segment AWriteFile

pascal void TDocumentDots::DoNeedDiskSpace(TFile* itsFile,
										long& dataForkBytes,
										long& rsrcForkBytes) // Override 
{
	inherited::DoNeedDiskSpace(itsFile, dataForkBytes, rsrcForkBytes);
} // TDocumentDots::DoNeedDiskSpace 

//----------------------------------------------------------------------------------------
// TDocumentDots::DoMenuCommand: This method is overridden to handle menu items which
// are enabled when this document is in the target chain. In this example, this is true
// when the document is open and its window is the active window. The inherited method
// should always be called so that MacApp can allow successor objects in the target chain
// (i.e. the application) to handle THEIR menu items.
//----------------------------------------------------------------------------------------
#pragma segment ASelCommand

pascal void TDocumentDots::DoMenuCommand(CommandNumber aCommandNumber) // Override 
{
	switch (aCommandNumber) 
	{
		case cCommandHandledByDocument: 
			{
				TCommandDots* aCommand = new TCommandDots;
				aCommand->ICommandDots(aCommandNumber, this);
				this->PostCommand(aCommand);
			}
			break;
		default:
			inherited::DoMenuCommand(aCommandNumber);
			break;
	}
} // TDocumentDots::DoMenuCommand 

//----------------------------------------------------------------------------------------
// TDocumentDots::DoRead: 
//----------------------------------------------------------------------------------------
#pragma segment AReadFile

pascal void TDocumentDots::DoRead(TFile* aFile,
						   			Boolean forPrinting) // Override 
{
	inherited::DoRead(aFile,forPrinting);
} // TDocumentDots::DoRead 

//----------------------------------------------------------------------------------------
// TDocumentDots::DoSetupMenus: This method is overridden to enable menu items which
// should be enabled when this object is in the target chain. MacApp initially disables
// all menu items, then lets the objects in the target chain enable those items they
// handle.
//
// A document object is in the target chain when its window is the active window.
//
// The inherited method is called so that TDocument can enable document-level menu items
// like "Save ". This also ensures that objects further up the target chain (the
// application) can set up THEIR menus.
//----------------------------------------------------------------------------------------
#pragma segment ARes

pascal void TDocumentDots::DoSetupMenus() // Override 
{
	inherited::DoSetupMenus();
	
	Enable(cCommandHandledByDocument,TRUE);
} // TDocumentDots::DoSetupMenus 

//----------------------------------------------------------------------------------------
// TDocumentDots::DoWrite: 
//----------------------------------------------------------------------------------------
#pragma segment AWriteFile

pascal void TDocumentDots::DoWrite(TFile* aFile,
									Boolean makingCopy) // Override 
{
	inherited::DoWrite(aFile,makingCopy);
} // TDocumentDots::DoWrite 