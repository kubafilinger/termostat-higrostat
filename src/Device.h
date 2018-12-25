#ifndef __DEVICE_H__
#define __DEVICE_H__

#include "helpers.h"

class Device
{
	int pin;
    bool run;

public:
	Device(int pin);
	~Device();
    void enable();
    void disable();
    bool isEnable();
};

#endif //__DEVICE_H__
