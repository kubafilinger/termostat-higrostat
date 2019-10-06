#include <Wire.h>
#include <LiquidCrystalI2C.h>
#include "DHT.h"
#include <SPI.h>
#include <SD.h>
#include <ThreeWire.h>
#include "src/consts.h"
#include "src/Device.h"
#include "src/PWMDevice.h"
#include "src/Rtc.h"
#include "src/helpers.h"

float insideTemperature = 0;
float outsideTemperature = 0;
float airHumidity = 0;
float outsideAirHumidity = 0;
bool canSaveToSdCard = false;
unsigned long time = 0;
int timeDisplayScreen = 0;

ThreeWire wire(RTC_DATA, RTC_CLOCK, RTC_RESET);
Rtc *rtc = new Rtc(wire);
LiquidCrystalI2C lcd(0x38, 3, 1, 2, 4, 5, 6, 7);
DHT hygrometerInternal(DHT_INTERNAL_PIN, DHTTYPE);
DHT hygrometerExternal(DHT_EXTERNAL_PIN, DHTTYPE);
Device *heat = new Device(HEAT);
Device *airHumidifier = new Device(AIR_HUMIDIFIER);
PWMDevice *innerFan = new PWMDevice(FAN_IN);
PWMDevice *outerFan = new PWMDevice(FAN_OUT);

void setup() {
    lcd.begin(LCD_COLS, 2);
    lcd.printRow(0, "Inicjalizacja");
    rtc->begin();

    heat->disable();
    innerFan->disable();
    outerFan->disable();
    airHumidifier->disable();

    hygrometerInternal.begin();
    hygrometerExternal.begin();
  
    if (SD.begin(chipSelect)) {
        canSaveToSdCard = true;
    
        if(!SD.exists(csvFileName)) {
            File dataFile = SD.open(csvFileName, FILE_WRITE);
            
            dataFile.println("temp2,temp_inside,temp_outside,humidity_inside,humidity_outside,inner_fan,outer_fan,light,heat,air_humidifier,time");
            dataFile.close();
        }
    } else {
        lcd.printRow(0, "SD Card Error!");
    }

    delay(1000);
}

void loop() {
    outsideTemperature = hygrometerExternal.readTemperature();
    insideTemperature = hygrometerInternal.readTemperature();
    airHumidity = hygrometerInternal.readHumidity();
    outsideAirHumidity = hygrometerExternal.readHumidity();
  
    //LOGIC

    const int daysCount = 4;
    int day1 = 1;
    int day2 = 2;
    int day3 = 3;
    int day4 = 0;

    if (day % daysCount = day1) {
        heat->enable();
        innerFan->disable();
        outerFan->disable();
        airHumidifier->disable();
    } else if (day % daysCount = day2) {
        heat->disable();
        innerFan->enable();
        outerFan->disable();
        airHumidifier->disable();
        
    } else if (day % daysCount = day3) {
        heat->disable();
        innerFan->disable();
        outerFan->enable();
        airHumidifier->disable();
        
    } else if (day % daysCount = day4) {
        heat->disable();
        innerFan->disable();
        outerFan->disable();
        airHumidifier->enable();
        
    }

    //SAVE TO SDCARD

    if(millis() - time >= SDCARD_SAVE_TIME) { // 15 minutes
        String csvRow = String(
            String(getTemperature(tempTwo)) + "," + 
            String(insideTemperature) + "," + 
            String(outsideTemperature) + "," + 
            String(airHumidity) + "," + 
            String(outsideAirHumidity) + "," + 
            innerFan->isEnable() + "," + 
            outerFan->isEnable() + "," +
            lightIsDetected() + "," + 
            heat->isEnable() + "," +
            airHumidifier->isEnable() + "," +
            rtc->prettyFormat()
        );

        saveToSDCard(csvRow);
        time = millis();
    }

    //DISPLAY ON LCD

    char row1[LCD_COLS + 1], row2[LCD_COLS + 1], innerFanSign, outerFanSign, heatSign;

    // diaplay fan and heat
    innerFanSign = innerFan->isEnable() ? LCD_SIGN_INNER_FAN : ' '; 
    outerFanSign = outerFan->isEnable() ? LCD_SIGN_OUTER_FAN : ' ';
    heatSign = heat->isEnable() ? LCD_SIGN_HEAT : ' ';

    if (timeDisplayScreen >= TIME_DISPLAY_SCREEN * NUMBER_OF_SCREENS) {
        timeDisplayScreen = 0;
    }

    if (timeDisplayScreen < TIME_DISPLAY_SCREEN) { // screen 1
        sprintf(row1, "%s", rtc->prettyFormat().c_str());
        sprintf(row2, "I: %.1f*C %.1f%% %c%c%c", insideTemperature, airHumidity, innerFanSign, outerFanSign, heatSign);
    } else if (timeDisplayScreen < TIME_DISPLAY_SCREEN * 2) { // screen 2
        sprintf(row1, "%s", rtc->prettyFormat().c_str());
        sprintf(row2, "O: %.1f*C %.1f%% %c%c%c", outsideTemperature, outsideAirHumidity, innerFanSign, outerFanSign, heatSign);
    }

    lcd.printRow(0, String(row1));
    lcd.printRow(1, String(row2));
    
    timeDisplayScreen++;
    
    delay(1000);
}

bool saveToSDCard(String csvRow) {
    if(!canSaveToSdCard) {
        return false;
    }

    File dataFile = SD.open(csvFileName, FILE_WRITE);

    if(dataFile) {
        dataFile.println(csvRow);
        dataFile.close();

        return true;
    }

    return false;
}

float getTemperature(int analogPin) {
    return (((analogRead(analogPin) * 5.0) / 1024.0) - 0.5) * 100;
}

bool lightIsDetected() {
    return digitalRead(LIGHT_DETECTOR) ? false : true;
}
