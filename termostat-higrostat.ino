#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "DHT.h"
#include <SPI.h>
#include <SD.h>
#include "src/consts.h"
#include "src/Device.h"
#include "src/PWMDevice.h"
#include "src/helpers.h"

float avgTemperature = 0;
float outsideTemperature = 0;
float airHumidity = 0;
float outsideAirHumidity = 0;
bool canSaveToSdCard = false;
unsigned long time = 0;
int timeDisplayScreen = 0;

LiquidCrystal_I2C lcd(0x38, 3, 1, 2, 4, 5, 6, 7);
DHT hygrometerInternal(DHT_INTERNAL_PIN, DHTTYPE);
DHT hygrometerExternal(DHT_EXTERNAL_PIN, DHTTYPE);
Device *heat = new Device(HEAT);
Device *airHumidifier = new Device(AIR_HUMIDIFIER);
PWMDevice *innerFan = new PWMDevice(FAN_IN);
PWMDevice *outerFan = new PWMDevice(FAN_OUT);

void setup() {
    lcd.begin(16, 2);
    lcd.clear();
    lcd.home();
    lcd.print("Inicjalizacja");

    heat->disable();
    innerFan->disable();
    outerFan->disable();
    airHumidifier->disable();

    hygrometerInternal.begin();
    hygrometerExternal.begin();
  
    if (SD.begin(chipSelect)) {
        canSaveToSdCard = true;
    
        if(!SD.exists("data.csv")) {
            File dataFile = SD.open("data.csv", FILE_WRITE);
            
            dataFile.println("temp1,temp2,temp3,temp_outside,humidity_inside,humidity_outside,inner_fan,outer_fan,heat,air_humidifier,time");
            dataFile.close();
        }
    } else {
        lcd.clear();
        lcd.home();
        lcd.print("SD Card Error!");
    }

    delay(1000);
    
    lcd.clear();
    lcd.home();
}

void loop() {
    avgTemperature = (getTemperature(tempOne) + getTemperature(tempTwo) + getTemperature(tempThree)) / 3;
    outsideTemperature = hygrometerExternal.readTemperature();
    airHumidity = hygrometerInternal.readHumidity();
    outsideAirHumidity = hygrometerExternal.readHumidity();
  
    if(avgTemperature - TEMP_DEVIATION > REF_TEMP) { // temperatura wyzsza niz powinna
        heat->disable();
        innerFan->enable();
        outerFan->enable();
    } else if(avgTemperature + TEMP_DEVIATION < REF_TEMP) { // temp nizsza niz powinna
        heat->enable();
        innerFan->disable();
        outerFan->disable();
    } else { // temperatura idealna
        heat->disable();
        innerFan->disable();
        outerFan->disable();
    }

    if(airHumidity - HUMIDITY_DEVIATION > REF_HUMIDITY) { // wilgotnosc wyzsza niz powinna
        airHumidifier->disable();
    } else if(airHumidity + HUMIDITY_DEVIATION < REF_HUMIDITY) { // wilgotnosc nizsza
        airHumidifier->enable();
    } else { // wilgotność idealna
        airHumidifier->disable();
    }

    lcd.clear();

    if(millis() - time >= SDCARD_SAVE_FREQ) { // 15 minutes
        lcd.setCursor(14, 0);
        lcd.print("S");
        
        String row = String(
            String(getTemperature(tempOne)) + "," + 
            String(getTemperature(tempTwo)) + "," + 
            String(getTemperature(tempThree)) + "," + 
            String(outsideTemperature) + "," + 
            String(airHumidity) + "," + 
            String(outsideAirHumidity) + "," + 
            innerFan->isEnable() + "," + 
            outerFan->isEnable() + "," +
            digitalRead(LIGHT_DETECTOR) + "," + 
            heat->isEnable() + "," + 
            millis()
        );

        saveToSDCard(row);
        time = millis();
    }

    if(timeDisplayScreen < TIME_DISPLAY_SCREEN) { // screen 1
        lcd.setCursor(0, 0);
        lcd.print("Inside:");

        lcd.setCursor(0, 1);
        lcd.print(avgTemperature, 1);
        lcd.print("*C");

        // display air humidity
        lcd.setCursor(8, 1);
        lcd.print(airHumidity, 1);
        lcd.print("%");
    } else if(timeDisplayScreen < TIME_DISPLAY_SCREEN * 2) { // screen 2
        lcd.setCursor(0, 0);
        lcd.print("Outside:");

        lcd.setCursor(0, 1);
        lcd.print(outsideTemperature, 1);
        lcd.print("*C");

        // display outside air humidity
        lcd.setCursor(8, 1);
        lcd.print(outsideAirHumidity, 1);
        lcd.print("%");
    } else {
        timeDisplayScreen = 0;
    }

    // diaplay innerFan and heat
    if(innerFan->isEnable()) {
        lcd.setCursor(13, 0);
        lcd.print("F");
    } else {
        lcd.setCursor(13, 0);
        lcd.print(" ");
    }

    if(heat->isEnable()) {
        lcd.setCursor(15, 0);
        lcd.print("H");
    } else {
        lcd.setCursor(15, 0);
        lcd.print(" ");
    }
    
    timeDisplayScreen++;
    
    delay(1000);
}

bool saveToSDCard(String row) {
    if(!canSaveToSdCard) {
        return false;
    }

    File dataFile = SD.open("data.csv", FILE_WRITE);

    if(dataFile) {
        dataFile.println(row);
        dataFile.close();

        return true;
    }

    return false;
}

float getTemperature(int analogPin) {
    return (((analogRead(analogPin) * 5.0) / 1024.0) - 0.5) * 100;
}
