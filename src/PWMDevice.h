#ifndef __PWMDEVICE_H__
#define __PWMDEVICE_H__

#include "helpers.h"

#ifndef __DEVICE_H__
    #include "Device.h"
#endif

class PWMDevice : public Device
{
	int pwm;
	int pin;
    bool run;

public:
	PWMDevice(int pin);
	~PWMDevice();
    void setPWM(int pwm);
    int getPWM();
};

#endif //__PWMDEVICE_H__
