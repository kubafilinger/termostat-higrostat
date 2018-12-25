#include "PWMDevice.h"

PWMDevice::PWMDevice(int pin)
:Device(pin)
{
	this->pin = pin;

	pinMode(pin, OUTPUT);
	this->disable();
}

PWMDevice::~PWMDevice()
{
}

void PWMDevice::setPWM(int pwm) {
    //todo: zabezpieczenie bo zakres tylko 0 -100 procent
    this->pwm = pwm;
}

int PWMDevice::getPWM() {
    return this->pwm;
}