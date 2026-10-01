#include <MacTypes.h>
#include <Dialogs.h>
#include <DiskInit.h>
#include <Events.h>
#include <Fonts.h>
#include <MacWindows.h>
#include <TextEdit.h>
#include <ToolUtils.h>

enum {mApple=128,mFile};
enum {iNewGame=1,iSeparator1,iHumanPlaysFirst,iGameSize,iSeparator2,iQuit};

enum {kGameSizeDialog=128};
enum {kGameSizeItem=2};

void ProcessEvent(EventRecord *ev);