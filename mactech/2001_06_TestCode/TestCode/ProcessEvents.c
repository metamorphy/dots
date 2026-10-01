#include "ProcessEvents.h"
#include "globals.h"

static OSErr DoCommand (long mResult, short modifiers)
{
#pragma unused(modifiers)
	short menu, item;
	WindowPtr wind;
	OSErr err = noErr;

	menu = HiWord(mResult);
	item = LoWord(mResult);
	
	wind = FrontWindow();
	
	switch (menu) {
	case mFile:
		switch (item) {
			MenuHandle menuHdl;
			DialogPtr dlg;
			Str255 itemText;
			Handle itemHdl;
			Rect itemRect;
			long itemValue;
			short itemHit,itemType;
			case iNewGame:
				gNewGameFlag = true;
				break;
			case iGameSize:
				dlg = GetNewDialog(kGameSizeDialog,nil,(WindowPtr)-1);
				GetDialogItem(dlg,kGameSizeItem,&itemType,&itemHdl,&itemRect);
				NumToString(gBoardSize,itemText);
				SetDialogItemText(itemHdl,itemText);
				SelectDialogItemText(dlg,kGameSizeItem,0,0x7fff);
				do {
					ModalDialog(nil,&itemHit);
				} while (itemHit!=1);
				GetDialogItem(dlg,kGameSizeItem,&itemType,&itemHdl,&itemRect);
				GetDialogItemText(itemHdl,itemText);
				StringToNum(itemText,&itemValue);
				gBoardSize = itemValue;
				DisposeDialog(dlg);
				break;
			case iHumanPlaysFirst:
				gHumanPlaysFirst = !gHumanPlaysFirst;
				menuHdl = GetMenuHandle(mFile);
				CheckItem(menuHdl, iHumanPlaysFirst, gHumanPlaysFirst);
				break;
			case iQuit:
				gQuitFlag = true;
				break;
			default:
				break;
		}
		break;
 	}
	HiliteMenu(0);
	return err;
}

static OSErr HandleKey (EventRecord *ev)
{
	unsigned char theChar;
	unsigned char theKey;
	short modifiers;
	long menuCmd;
	WindowPtr wind;
	OSErr err = noErr;
	
	wind = FrontWindow();
	theChar = ev->message & charCodeMask;
	theKey = (ev->message & keyCodeMask) >> 8;
	modifiers = ev->modifiers;
	
	if ( (modifiers & cmdKey) != 0) {
		menuCmd = MenuKey(theChar);  
		if (HiWord(menuCmd) != 0) 
			return DoCommand(menuCmd, modifiers);
	} 
		
	return err;
}

void ProcessEvent(EventRecord *ev) {
	WindowPtr   	window;
	short       	thePart;
	Rect        	screenRect;
	Point			aPoint = {100, 100};
	switch (ev->what) {
	case mouseDown:
	
		thePart = FindWindow( ev->where, &window );
		
		switch( thePart ) {
			case inMenuBar: 
				DoCommand(MenuSelect(ev->where), ev->modifiers);
				break;
			case inDrag:
				screenRect = (**GetGrayRgn()).rgnBBox;
				DragWindow( window, ev->where, &screenRect );
				break ;
			case inContent:
				if (window != FrontWindow()) {
					SelectWindow( window );
				}
				break ;
			case inGoAway:
				if (TrackGoAway( window, ev->where )) {
					DisposeWindow ( window );
					gQuitFlag = true;

				}
				break ;
			default:							
				break ;
		}
		break ;
			
	case updateEvt:
		window = (WindowPtr)ev->message;
		SetPort( window ) ;
		BeginUpdate( window );
		EndUpdate( window );
		break ;
		
	case keyDown:
	case autoKey:
		HandleKey(ev);
		break;
		
	case diskEvt:
		if ( HiWord(ev->message) != noErr ) 
			(void) DIBadMount(aPoint, ev->message);
		break;
		
	case osEvt:
	case activateEvt:
		break;
	}
}
