

#include <Arduino.h>

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif
#include <JsonListener.h>
#include <time.h>
#include "OpenWeatherMapCurrent.h"



#include <SPI.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <Adafruit_AHTX0.h>
#include <Fonts/FreeSans9pt7b.h>
Adafruit_AHTX0 aht;



#include <SSD1306TUR.h>
#include "TrFonts/FreeSansBold12pt7bTR.h"
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library.
// On an arduino UNO:       A4(SDA), A5(SCL)- wemos d1 d2
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET -1		// Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
SSD1306TUR display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
void TrTextsOnScr(float x, int y);



void setupAHT();
void setupOLED();
void conncetingScreen();
void ekranaYaz(float derece, int nem, float saat, float tarih);




OpenWeatherMapCurrent client;

String OPEN_WEATHER_MAP_APP_ID = "API KEY";

String OPEN_WEATHER_MAP_LOCATION_ID = "744926";


String OPEN_WEATHER_MAP_LANGUAGE = "en";
boolean IS_METRIC = true;

#if defined(ESP8266)
const char* ESP_HOST_NAME = "esp-" + ESP.getFlashChipId();
#else
const char* ESP_HOST_NAME = "esp-" + ESP.getEfuseMac();
#endif






const char* ssid       = "wifi";
const char* password   = "psw";

const long utcOffsetInSeconds = 10800;

char daysOfTheWeek[7][12] = {"Pazar", "Pazartesi", "Salı", "Çarşamba", "Perşembe", "Cuma", "Cumartesi"};
String months[12]={"Ocak", "Şubat", "Mart", "Nisan", "Mayıs", "Haziran", "Temmuz", "Ağustos", "Eylül", "Ekim", "Kasım", "Aralık"};

String hs="",ss="",ms="";
int y=0;

void setup(){
  Serial.begin(115200);
  setupAHT();
  setupOLED();

  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

}

void loop() {


  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);
  //TrTextsOnScr(temp.temperature,humidity.relative_humidity);

  




  
  OpenWeatherMapCurrentData data;
  client.setLanguage(OPEN_WEATHER_MAP_LANGUAGE);
  client.setMetric(IS_METRIC);
  client.updateCurrentById(&data, OPEN_WEATHER_MAP_APP_ID, OPEN_WEATHER_MAP_LOCATION_ID);


  ekranaYaz(temp.temperature, humidity.relative_humidity, data.temp, data.humidity);
  Serial.print(temp.temperature);
  Serial.printf("humidity: %d\n", data.humidity);
  Serial.printf("temp: %f\n", data.temp);


  delay(1000);
}




void setupAHT(){
  if (aht.begin()) {
    //Serial.println("Found AHT20");
  } else {
    //Serial.println("Didn't find AHT20");
  } 
}

void setupOLED(){
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
	{
		//Serial.println(F("SSD1306 allocation failed"));
		for (;;)
			; // Don't proceed, loop forever
	}
	// Clear the buffer
	display.clearDisplay();
	conncetingScreen();
}

void TrTextsOnScr(float x, int y)
{
	display.setTextColor(SSD1306_WHITE);
	display.clearDisplay();
	display.setTextSize(1);
  display.setCursor(48, 5);
	display.println(String(x)+" C");
	display.setCursor(50, 15);
	display.print(String(y)+" %RH");
	display.display(); 
	//delay(3000);
}

void conncetingScreen()
{
	display.setTextColor(SSD1306_WHITE);
	display.clearDisplay();
	display.setTextSize(1);
  display.setCursor(20, 10);
	display.println("Connecting...");
	display.display(); 
	//delay(3000);
}

void ekranaYaz(float derece, int nem, float saat, float tarih)
{
	display.setTextColor(SSD1306_WHITE);
	display.clearDisplay();
	display.setTextSize(1);

  display.setCursor(5, 10);
	display.println(String(saat));

  display.setCursor(14, 23);
	display.println(String(tarih));


  display.setCursor(68, 7);
	display.println(String(derece)+" C");

	display.setCursor(70, 14);
	display.print(String(nem)+" %RH");

	display.display(); 
	//delay(3000);
}

