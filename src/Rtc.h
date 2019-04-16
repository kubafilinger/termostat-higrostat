#ifndef __RTC_H__
#define __RTC_H__

#ifndef Arduino_h
    #include <Arduino.h>
#endif
#include <ThreeWire.h>
#include <RtcDS1302.h>
#include "helpers.h"

class Rtc
{
    RtcDS1302<ThreeWire> *rtc;

public:
	Rtc(ThreeWire &wire);
	~Rtc();
    void begin();
    String prettyFormat();
};

#endif //__RTC_H__
