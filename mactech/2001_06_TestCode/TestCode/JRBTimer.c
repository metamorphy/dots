#include "JRBTimer.h"
#include "DriverServices.h"

//
// May, 2000 version
//
// Calculate elapsed time using efficient Uptime routine
// Requires DriverServicesLib, at least for some versions of MacOS
//   (ignore link errors for the duplicates this creates with InterfaceLib)
// Modified to return floating point to avoid overflow
//   (an unsigned long can only hold xxx)


#define kTwoPower32 (4294967296.0) /*2^32*/
#define kTwoPower32Divided1000 (4294967.2960) /*2^32/1000.0*/

static AbsoluteTime startTime;

static AbsoluteTime ReadClock(){
	AbsoluteTime absoluteTime = UpTime();
	return absoluteTime;
}

/* return time since StartTimer in Microseconds */
double StopTimer() {
  AbsoluteTime endTime = ReadClock();
  AbsoluteTime deltaTime = SubAbsoluteFromAbsolute(endTime,startTime);
  Nanoseconds nanosecondTime = AbsoluteToNanoseconds(deltaTime);
  return (double)(nanosecondTime.hi*kTwoPower32Divided1000) + (double)(nanosecondTime.lo/1000);
}

void StartTimer() {
  startTime=ReadClock();
}
