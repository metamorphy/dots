/*
	UDocumentDots.h
	
	Copyright ©1991-1992 Apple Computer, Inc.
	All rights reserved

	Change History:
	
 */

#ifndef __UDOCUMENTDOTS__
#define __UDOCUMENTDOTS__

#ifndef __UFILEBASEDDOCUMENT__
#include <UFileBasedDocument.h>
#endif

//----------------------------------------------------------------------------------------
// Constants
//----------------------------------------------------------------------------------------

// Commands
const CommandNumber cCommandHandledByDocument = 401;

// Files
const OSType kFileType = 'SF01';	// File-type code used for document files 

//----------------------------------------------------------------------------------------
// TDocumentDots
//----------------------------------------------------------------------------------------

class TDocumentDots: public TFileBasedDocument
{
public:
		
	// Construction/Destruction
	virtual pascal void Initialize(); // Override
	
	virtual pascal void  IDocumentDots(TFile* itsFile, OSType itsCreator);
	virtual pascal void  DoInitialState(); // Override
	virtual pascal void  FreeData(); // Override
	virtual pascal void  Free(); // Override
			
	// Views
	virtual pascal void  DoMakeViews(Boolean forPrinting); // Override

	// Commands
	virtual pascal void  DoSetupMenus(); // Override
	virtual pascal void  DoMenuCommand(CommandNumber aCommandNumber); // Override

	// Saving and Restoring
	virtual pascal void  DoNeedDiskSpace(TFile* ,
										long& dataForkBytes,
										long& rsrcForkBytes); // Override
	virtual pascal void  DoRead(TFile* aFile,
								Boolean forPrinting); // Override
	virtual pascal void  DoWrite(TFile* aFile,
								Boolean makingCopy); // Override

};

#endif
