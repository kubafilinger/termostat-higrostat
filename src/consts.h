// outputs
#define FAN_IN 3
#define FAN_OUT 4
#define HEAT 8
#define AIR_HUMIDIFIER 9

// inputs 
#define DHT_EXTERNAL_PIN 0
#define DHT_INTERNAL_PIN 1
#define LIGHT_DETECTOR 5
#define RTC_CLOCK 6
#define RTC_DATA 2
#define RTC_RESET 7

// analog inputs
const int tempOne = 1;
const int tempTwo = 2;
const int tempThree = 3;

// temp and hum
#define REF_TEMP 28.8
#define TEMP_DEVIATION 1.0
#define HUMIDITY_DEVIATION 5.0
#define REF_HUMIDITY 50.0
#define DHTTYPE DHT11

// lcd
#define LCD_COLS 16
#define LCD_SIGN_INNER_FAN 'F'
#define LCD_SIGN_OUTER_FAN 'O'
#define LCD_SIGN_HEAT 'H'
#define TIME_DISPLAY_SCREEN 5
#define NUMBER_OF_SCREENS 2

//sd card
#define SDCARD_SAVE_TIME 299115
const int chipSelect = 10;
char* csvFileName = "data.csv";
