#include "Rtc.h"

Rtc::Rtc(ThreeWire &wire)
{
	this->rtc = new RtcDS1302<ThreeWire>(wire);
}

Rtc::~Rtc()
{
	delete this->rtc;
}

void Rtc::begin() {
	this->rtc->Begin();
}

String Rtc::prettyFormat() {
	char result[17];
	RtcDateTime dateTime = this->rtc->GetDateTime();

	sprintf(result, "%02u:%02u %02u/%02u/%04u", dateTime.Hour(), dateTime.Minute(), dateTime.Day(), dateTime.Month(), dateTime.Year());

	return static_cast<String>(result);
}