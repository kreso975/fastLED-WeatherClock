#include <FastLED.h>
#include "Font6x8.h"
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <TimeLib.h>
#include "DHTsensors.h"

// WiFi credentials
const char* ssid = "";
const char* password = "";

// NTP server details
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3600; // Adjust for your timezone +1

#define LED_PIN 0
#define COLOR_ORDER GRB
#define CHIPSET WS2812B
#define BRIGHTNESS 30

// Params for width and height
const uint8_t kMatrixWidth = 32;
const uint8_t kMatrixHeight = 8;

// Param for different pixel layouts
const bool    kMatrixSerpentineLayout = true;
const bool    kMatrixVertical = true;

#define NUM_LEDS (kMatrixWidth * kMatrixHeight)
CRGB leds_plus_safety_pixel[ NUM_LEDS + 1];
CRGB* const leds( leds_plus_safety_pixel + 1);

// Initialize WiFi and NTP client
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, ntpServer, gmtOffset_sec, 60000); // Update every minute


unsigned long lastRefreshTime = 0;

void setup() {
    // Initialize serial communication
    Serial.begin(115200);
    delay(500);

    setupDHT();

    FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
    FastLED.setMaxPowerInVoltsAndMilliamps(5, 700);
    FastLED.setBrightness(BRIGHTNESS);

    // Connect to WiFi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
        // Initialize NTP client
        timeClient.begin();
        timeClient.update();
    }
    Serial.println("Connected to WiFi");
}

void loop() {
  
    timeClient.update();

    // Get the current time
    unsigned long currentTime = millis();
    // Check if 10 seconds have passed since the last refresh
    if (currentTime - lastRefreshTime >= 1000) {
        updateDisplay();
        for (int i = 0; i < NUM_LEDS; i++) { leds[i].nscale8_video(BRIGHTNESS); }
        lastRefreshTime = currentTime;
    }

    if ( ( millis() - lastMeasureInterval > measureInterval ) || measureFirstRun )
    {
			lastMeasureInterval = millis();
			measureFirstRun = false;

			// read Weather Sensor
		  getWeatherDHT();
		}
    
    FastLED.show();
}








