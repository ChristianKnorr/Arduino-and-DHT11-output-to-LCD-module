/*
  https://www.hive-rd.com/blog/arduino-dht11-output-lcd-module/
  https://github.com/adafruit/Adafruit_Sensor
  https://howtomechatronics.com/tutorials/arduino/lcd-tutorial/ // Umlaut
  https://playground.arduino.cc/Learning/arduinoSleepCode
*/
#include <LiquidCrystal.h>
//#include <DHT.h>
#include <DHT_U.h>
#include <avr/sleep.h>
#define DHTTYPE DHT11 // DHT 11 
#define DHTPIN 1 // Pin which is connected to the DHT sensor.

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

DHT_Unified dht(DHTPIN, DHTTYPE);

void setup() {
  lcd.begin(16, 2); //16 by 2 character display
  dht.begin();
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  dht.humidity().getSensor(&sensor);
  lcd.clear();
  byte UmlautGrad[8] = { B01100, B10010, B10010, B01100, B00000, B00000, B00000 };
  lcd.createChar(1, UmlautGrad); // °C
  byte UmlautCursor[8] = { B00000, B00000, B00000, B00000, B00000, B00000, B00001 };
  lcd.createChar(2, UmlautCursor); // .
}

void printCursor() {
  lcd.setCursor(15, 1);
  lcd.write(2); // Cursor
  delay(500); //wait a while
  lcd.setCursor(15, 1);
  lcd.print(" ");
  delay(500);
  lcd.setCursor(0, 0);
}

void loop() {
  sensors_event_t event;
  if ( (isnan(event.temperature)) || (isnan(event.relative_humidity))) {
    lcd.clear();
    lcd.setCursor(5, 0);
    lcd.print("Fehler");
    lcd.setCursor(2, 1);
    lcd.print("DHT11 Sensor");
    sleepNow(); // Ende...
  } else {
    printCursor();
    dht.humidity().getEvent(&event);
    lcd.print("Luftfeuchte ");
    lcd.print(round(event.relative_humidity));
    lcd.print("%");
    dht.temperature().getEvent(&event);
    lcd.setCursor(0, 1);
    lcd.print("Temperatur ");
    lcd.print(round(event.temperature));
    lcd.write(1); // °
    lcd.print("C");
  }
}

void sleepNow() { // here we put the arduino to sleep
  /* Now is the time to set the sleep mode. In the Atmega8 datasheet
     http://www.atmel.com/dyn/resources/prod_documents/doc2486.pdf on page 35
     there is a list of sleep modes which explains which clocks and
     wake up sources are available in which sleep mode.

     In the avr/sleep.h file, the call names of these sleep modes are to be found:

     The 5 different modes are:
         SLEEP_MODE_IDLE         -the least power savings
         SLEEP_MODE_ADC
         SLEEP_MODE_PWR_SAVE
         SLEEP_MODE_STANDBY
         SLEEP_MODE_PWR_DOWN     -the most power savings

     For now, we want as much power savings as possible, so we
     choose the according
     sleep mode: SLEEP_MODE_PWR_DOWN */
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);   // sleep mode is set here
  sleep_enable();          // enables the sleep bit in the mcucr register
  // so sleep is possible. just a safety pin
  while(true) delay(10000);
}
