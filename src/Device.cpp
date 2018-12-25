#include "Device.h"

Device::Device(int pin)
{
	this->pin = pin;

	pinMode(pin, OUTPUT);
	this->disable();
}

Device::~Device()
{
}

void Device::enable() {
	this->run = true;
	digitalWrite(this->pin, HIGH);
}

void Device::disable() {
	this->run = false;
	digitalWrite(this->pin, LOW);
}

bool Device::isEnable() {
	return this->run;
}

