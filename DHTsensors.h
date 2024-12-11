#include <DHT.h>

//Constants
#define DHTPIN 2            // what pin we're connected to
//#define DHTTYPE DHT11     // DHT 11
#define DHTTYPE DHT22       // DHT 22 (AM2302), AM2321
//#define DHTTYPE DHT21     // DHT 21 (AM2301)

bool detectModule = false;                                  // Was detect DHT use True if moduledetection not needed
bool measureFirstRun = true;
#define measureInterval 30e3                                // in miliseconds = 30 * 1000 (e3 = 3 zeros) = 15sec
unsigned long lastMeasureInterval = measureInterval;        // time of last point added

byte t_measure;
float h, t;


DHT dht(DHTPIN, DHTTYPE); // Initialize DHT sensor
