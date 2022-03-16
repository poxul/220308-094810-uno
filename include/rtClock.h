#ifndef RT_CLOCK_CLASS
#define RT_CLOCK_CLASS

#include <RtcDS3231.h>

void printDateTime(const RtcDateTime &dt);
void printDate(const RtcDateTime& dt);
void setupRTC ();
void checkRTC() ;

#endif