//--------------------------------------------------------------------------------------------------
//																				
//	File:			Dots.r		 											
//																				
//	Description:	Resource File for the Dots Sample Program.		
//																				
//																				
//	Copyright © 1991 - 1992 by Apple Computer, Inc.  All rights reserved.  		
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// INCLUDE FILES 
//--------------------------------------------------------------------------------------------------

// Standard Includes 

#ifndef __TYPES.R__
#include "Types.r"
#endif

#ifndef __SYSTYPES.R__
#include "SysTypes.r"
#endif

#ifndef __BalloonTypes__
#include "BalloonTypes.r"
#endif

#ifndef __MacAppTypes__
#include "MacAppTypes.r"
#endif

#ifndef __ViewTypes__
#include "ViewTypes.r"
#endif

#if qDebug
include "Debug.rsrc" not 'ckid';
#endif

include "MacApp.rsrc" not 'ckid';

// Includes for Building Blocks

include "Printing.rsrc" not 'ckid';
// include "Dialog.rsrc" not 'ckid';

// Include Code Resources

include $$Shell("ObjApp")$$Shell("XAppName") 'CODE';

//--------------------------------------------------------------------------------------------------
// CONSTANTS 
//--------------------------------------------------------------------------------------------------

// Finder Information
// ------------------

#define kApplicationName	$$Shell("XAppName")
#define kSignature			'SS01'			// Application signature 
#define kFileType			'SF01'			// Data file type 
#define kStationery			'sF01'			// Stationery file type 

// Bundle Constants
// ------------------

#define kBundleID						128
#define kApplicationID					128
#define kDocumentID						129
#define kStationeryID					130

// Constants for Menu IDs
// ----------------------

#define mPlayMenu	4

// Command Numbers For Menu Items
// ------------------------------

#define cCommandHandledByApplication	400
#define cCommandHandledByDocument		401
#define cCommandHandledByView			402
#define cTrackerCommand					403

// STR# resources
// --------------

#define kDotsMenuHelp				1000
#define kDotsWindowTitles			1001

// Text styles
// --------------

#define kSystemTextStyle				1000

// 'View' resources
// --------------

#define kDotsWindowID				kDefaultWindowID

// Memory Management Resources
// ---------------------------

#define kDotsSeg	1000
#define kDotsMem	1000
#define kDotsRes	1000

//--------------------------------------------------------------------------------------------------
// RESOURCES FOR MENUS 
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// Menu Bars 
//--------------------------------------------------------------------------------------------------

resource 'MBAR' (kMBarDisplayed,
#if qNames
kApplicationName,
#endif
	purgeable) {
	{ mApple; mFile; mEdit; mPlayMenu }
};

//--------------------------------------------------------------------------------------------------
// Menus 
//--------------------------------------------------------------------------------------------------

include "Defaults.rsrc" 'CMNU' (mApple);	// Grab the default Apple menus
include "Defaults.rsrc" 'CMNU' (mFile);		// Grab the default File menus
include "Defaults.rsrc" 'CMNU' (mEdit);		// Grab the default Edit menu

resource 'CMNU' (mPlayMenu,
#if qNames
"Play Menu",
#endif
nonpurgeable) {
	mPlayMenu,
	textMenuProc,
	EnablingManagedByMacApp,
	enabled,
	"Play",
	 {	/* array: 3 elements */
/* [1] */	"Command Handled By Application",noIcon, "1", noMark, plain, cCommandHandledByApplication;
/* [2] */	"Command Handled By Document", noIcon, "2", noMark, plain, cCommandHandledByDocument;
/* [3] */	"Command Handled By View",	noIcon, "3", noMark, plain, cCommandHandledByView;
	  }
	};

resource 'CMNU' (mBuzzwords,
#if qNames
"Buzzwords",
#endif
nonpurgeable
) {
	mBuzzwords,
	textMenuProc,
	EnablingManagedByMacApp,
	enabled,
	"Buzzwords",
	 {	/* array: 13 elements */
		"Page Setup Change",	noIcon,	noKey,	noMark, plain, cChangePrinterStyle;
		"Typing",				noIcon,	noKey,	noMark,	plain, cTyping;
		"Tracker Command",		noIcon,	noKey,	noMark, plain, cTrackerCommand;
	}
};

//--------------------------------------------------------------------------------------------------
// Balloon Help Resources for Menus 
//--------------------------------------------------------------------------------------------------

#include $$Shell("MALibraries") "HelpStrings.r";

include "Defaults.rsrc"  'hmnu' (mFile);	
include "Defaults.rsrc"  'hmnu' (mEdit);	

resource 'STR#' (kDotsMenuHelp, purgeable) { 
	{
		"No help yet for this  item",
		"Help For Menu Title",
		"Help For First Menu Item",
		"Help For Second Menu Item",
		"Help For Third Menu Item",
	}
};

resource 'hmnu' (mPlayMenu) 
{
	HelpMgrVersion,
	hmDefaultOptions,
	0, // balloon definition function 
	0, // variation code 
	
	// Missing items 
	HMStringResItem 
	{
		kDotsMenuHelp,1,
		kDotsMenuHelp,1,
		kDotsMenuHelp,1,
		kDotsMenuHelp,1,
	},
	
	// Title and items 
	{	
		// Title 
		HMStringResItem 
		{
			kDotsMenuHelp,2,
			kDotsMenuHelp,2,
			kDotsMenuHelp,2,
			kDotsMenuHelp,2,
		},
		// First Item 
		HMStringResItem 
		{
			kDotsMenuHelp,3,
			kDotsMenuHelp,3,
			kDotsMenuHelp,3,
			kDotsMenuHelp,3,
		},
		// Second Item
		HMStringResItem 
		{
			kDotsMenuHelp,4,
			kDotsMenuHelp,4,
			kDotsMenuHelp,4,
			kDotsMenuHelp,4,
		},
		// Third Item
		HMStringResItem 
		{
			kDotsMenuHelp,5,
			kDotsMenuHelp,5,
			kDotsMenuHelp,5,
			kDotsMenuHelp,5,
		},
	}
};

//--------------------------------------------------------------------------------------------------
// RESOURCES FOR VIEWS, DIALOGS, ALERTS etc. 
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// Views 
//--------------------------------------------------------------------------------------------------

resource 'STR#' (kDotsWindowTitles,
#if qNames
"Window titles",
#endif
purgeable) {
	{
	""
	}
};

resource 'TxSt' (kSystemTextStyle, purgeable) {tsPlain, 0, black, ""};

resource 'View' (kDotsWindowID,
#if qNames
"kDotsWindowID",
#endif
purgeable) { MAThreeOh, 
	{
	ViewSignatureAndClassname
		{'wind', 534, "", 'WIND', enabled, noIdle, {}, MAThreeOh, 
		{50, 20}, {260, 430}, sizeVariable, sizeVariable, shown, doesntWantToBeTarget, 
		handlesCursor, letsSubViewsHandleCursor, noCursorID, 
		handlesHelp, letsSubViewsHandleHelp, noHelpID, 1, 
		NoDrawingEnvironment {}, 
		AdornerListSignatureAndClassname {NoIdentifier, AdornerList, AdornerElementSize, AdornerElementSizeShift, DynamicArrayAllocationIncrement, {
			AdornFirst, AdornerLocalObject {EraseAdorner}, 
			DrawView, AdornerLocalObject {DrawAdorner}, 
			AdornLast, AdornerLocalObject {ResizeIconAdorner}}}, emptyUserArea, 
		Window {zoomDocProc, 'SKEL', goAwayBox, resizable, ignoreFirstClick, 
			freeOnClosing, disposeOnFree, closesDocument, openWithDocument, dontAdaptToScreen, stagger, forceOnScreen, 
			dontCenter, doesntFloat, doesntHideOnSuspend, generateActivates, filler, kDotsWindowTitles, 1}, 3}, 
	ViewSignatureAndClassname
		{'scrl', 186, "", 'SCLR', enabled, noIdle, {}, MAThreeOh, 
		{0, 0}, {245, 415}, sizeRelSuperView, sizeRelSuperView, shown, doesntWantToBeTarget, 
		handlesCursor, letsSubViewsHandleCursor, noCursorID, 
		handlesHelp, letsSubViewsHandleHelp, noHelpID, 1, 
		NoDrawingEnvironment {}, 
		NoAdorners {}, emptyUserArea, 
		Scroller {'vCLR', 'hCLR', {0, 0}, {16, 16}, VertConstrain, HorzConstrain, noInset, respondsToFKeys}, 1}, 
	ViewSignatureAndClassname
		{'view', 75, "TViewDots", 'SKEL', enabled, noIdle, {}, MAThreeOh, 
		{0, 0}, {72 * 15, 72 * 12}, sizeFixed, sizeFixed, shown, doesntWantToBeTarget, 
		handlesCursor, letsSubViewsHandleCursor, noCursorID, 
		handlesHelp, letsSubViewsHandleHelp, noHelpID, 1, 
		NoDrawingEnvironment {}, 
		NoAdorners {}, emptyUserArea, 
		View {}, 
		NoSubviews},
	ViewSignatureAndClassname
		{'ssbr', 101, "", 'hCLR', enabled, noIdle, {}, MAThreeOh, 
		{245, -1}, {16, 417}, sizeVariable, sizeVariable, shown, doesntWantToBeTarget, 
		handlesCursor, letsSubViewsHandleCursor, noCursorID, 
		handlesHelp, letsSubViewsHandleHelp, noHelpID, 1, 
		NoDrawingEnvironment {}, 
		NoAdorners {}, emptyUserArea, 
		ScrollerScrollBar {mHScrollBarHit, notHilited, notDimmed, sizeable, noInset, kSystemTextStyle, dontPreferOutline, h, 0, 0, 39585}, 
		NoSubviews}, 
	ViewSignatureAndClassname
		{'ssbr', 101, "", 'vCLR', enabled, noIdle, {}, MAThreeOh, 
		{-1, 415}, {247, 16}, sizeVariable, sizeVariable, shown, doesntWantToBeTarget, 
		handlesCursor, letsSubViewsHandleCursor, noCursorID, 
		handlesHelp, letsSubViewsHandleHelp, noHelpID, 1, 
		NoDrawingEnvironment {}, 
		NoAdorners {}, emptyUserArea, 
		ScrollerScrollBar {mVScrollBarHit, notHilited, notDimmed, sizeable, noInset, kSystemTextStyle, dontPreferOutline, v, 0, 0, 49755}, 
		NoSubviews}
	}
};

//--------------------------------------------------------------------------------------------------
// About Box
//--------------------------------------------------------------------------------------------------

resource 'DITL' (phAboutApp,
#if qNames
"phAboutApp",
#endif
	purgeable) {
	 {	/* array DITLarray: 3 elements */
		/* [1] */
		{160, 182, 180, 262},
		Button {
			enabled,
			"OK"
		};
		/* [2] */
		{10, 75, 150, 316},
		StaticText {
			disabled,
			"This sample program demonstrates large views and mouse tracking."
			"\n\nThis program was written "
			"with MacApp¨ © 1985-1992 Apple Computer, Inc."
		};
		/* [3] */
		{10, 20, 42, 52},
		Icon {
			disabled,
			1
		}
	}
};

include "Defaults.rsrc"  'ALRT' (phAboutApp);	// Grab the default about box

include "Defaults.rsrc"  'STR#' (kDefaultCredits);		// Grab the default credits


//--------------------------------------------------------------------------------------------------
// MultiFinderª information
//--------------------------------------------------------------------------------------------------

resource 'SIZE' (-1) {
	saveScreen,
	acceptSuspendResumeEvents,
	enableOptionSwitch,
	canBackground,
	doesActivateOnFGSwitch,
	backgroundAndForeground,
	dontGetFrontClicks,
	ignoreAppDiedEvents,
	is32BitCompatible,
	isHighLevelEventAware,
	localAndRemoteHLEvents,
	notStationeryAware,				
	reserved,
	reserved,
	reserved,
	reserved,
#if qModelFarCode
	575 * 1024,
	550 * 1024,
#elif qDebug
	500 * 1024,
	475 * 1024
#else
	350 * 1024,
	325 * 1024
#endif
};

//--------------------------------------------------------------------------------------------------
// Icons
//--------------------------------------------------------------------------------------------------

include "Defaults.rsrc"  'ICN#' (kApplicationID);	// MacApp Family large black & white	
include "Defaults.rsrc"  'ICN#' (kDocumentID);		// MacApp Document large black & white
include "Defaults.rsrc"  'ICN#' (kStationeryID);	// MacApp Stationery large black & white	
include "Defaults.rsrc"  'ics#' (kApplicationID);	// MacApp Family small black & white	
include "Defaults.rsrc"  'ics#' (kDocumentID);		// MacApp Document small black & white
include "Defaults.rsrc"  'ics#' (kStationeryID);	// MacApp Stationery small black & white
include "Defaults.rsrc"  'ics4' (kApplicationID);	// MacApp Family small 4 bit
include "Defaults.rsrc"  'ics4' (kDocumentID);		// MacApp Document small 4 bit
include "Defaults.rsrc"  'ics4' (kStationeryID);	// MacApp Stationery small 4 bit
include "Defaults.rsrc"  'ics8' (kApplicationID);	// MacApp Family small 4 bit
include "Defaults.rsrc"  'ics8' (kDocumentID);		// MacApp Document small 4 bit
include "Defaults.rsrc"  'ics8' (kStationeryID);	// MacApp Stationery small 4 bit
include "Defaults.rsrc"  'icl4' (kApplicationID);	// MacApp Family large 4 bit
include "Defaults.rsrc"  'icl4' (kDocumentID);		// MacApp Document large 4 bit
include "Defaults.rsrc"  'icl4' (kStationeryID);	// MacApp Stationery large 4 bit
include "Defaults.rsrc"  'icl8' (kApplicationID);	// MacApp Family large 8 bit
include "Defaults.rsrc"  'icl8' (kDocumentID);		// MacApp Document large 8 bit
include "Defaults.rsrc"  'icl8' (kStationeryID);	// MacApp Stationery large 8 bit

//--------------------------------------------------------------------------------------------------
// Memory usage information for MacAppª
//--------------------------------------------------------------------------------------------------

// See UMemory.h
// -------------

resource 'seg!' (kDotsSeg,
#if qNames
kApplicationName,
#endif
	purgeable) {
	{
		"GNonRes";
		"GClose";
		"GFile";
		"GOpen";
		"GSelCommand";
		"BBNonRes";
		"BBOpen";
		"GNonRes2";
		"GPrint";
		"GReadResources";
		"foo";
	}
};

// Additional Memory Requirements
// ------------------------------

resource 'mem!' (kDotsMem,
#if qNames
	"Additional Memory Requirements",
#endif
	purgeable) {
	0,				// Add to temporary reserve
	0,				// Add to permanent reserve
	0				// Add to stack space
};

// Additional Resident Segments
// ----------------------------

resource 'res!' (kDotsRes,
#if qNames
	kApplicationName,
#endif
	purgeable) {
	{	
		"DotsRes1";
		"DotsRes2";
	};
};

//--------------------------------------------------------------------------------------------------
// Icons, Bundles and FRefsÉÊOh my! (don't forget the Signature)
//--------------------------------------------------------------------------------------------------

type kSignature as 'STR ';
resource kSignature (0,
#if qNames
"Signature",
#endif
	purgeable) {
	"Dots 3.0 ©Apple Computer, Inc. 1988-1992"
};

resource 'FREF' (kApplicationID,
#if qNames
"Dots Application",
#endif
	purgeable) {
	'APPL',
	0,
	""
};

resource 'FREF' (kDocumentID,
#if qNames
"Dots Document",
#endif
	purgeable) {
	kFileType,
	1,
	""
};

resource 'FREF' (kStationeryID,
#if qNames
"Dots Stationery",
#endif
	purgeable) {
	kStationery,
	2,
	""
};

resource 'BNDL' (kBundleID,
#if qNames
"Dots",
#endif
	purgeable) {
	kSignature,
	0,
	{ /* array TypeArray: 2 elements */
		/* [1] */
		'ICN#',
		{ /* array IDArray: 3 elements */
			0, kApplicationID,
			1, kDocumentID,
			2, kStationeryID
		},
		/* [2] */
		'FREF',
		{ /* array IDArray: 3 elements */
			0, kApplicationID,
			1, kDocumentID,
			2, kStationeryID
		}
	}
};

//--------------------------------------------------------------------------------------------------
//  Version resources
//--------------------------------------------------------------------------------------------------

// The revision of this particular file

RESOURCE 'vers' (1,
#if qNames
"File Version",
#endif
	purgeable) {
	0x03,
	0x00,
	final,
	0x00,
	verUs,
	"3.0",
	"Dots 3.0, ©Jeff Mallett, Inc. 1986,1992"
};
include "Defaults.rsrc"  'vers' (2);		// Overall package
