
/* ======================================================================
Function: setupDHT
Purpose : Initialize DHT 11, 12, 22, 21
Input   : 
Output  : 
Comments: -
====================================================================== */
bool setupDHT()
{
	// Init DHT - need to find a way to know is it running properly
	dht.begin();
	float t = dht.readTemperature(); // Check if any reads failed and exit early (to try again) 
	if ( isnan(t) )
	{
    Serial.println(F("DHT sensor is not connected!"));
		detectModule = false;
		return false;
	}
	detectModule = true;
	return true;
}

/* ======================================================================
Function: getWeatherDHT
Purpose : Read Sensor data
Input   : 
Output  : 
Comments: 
====================================================================== */
void getWeatherDHT()
{
	float h_tmp, t_tmp;
	t_tmp = dht.readTemperature();
    
    // Read temperature as Fahrenheit ( t_measure == 1 )
    //float f = dht.readTemperature(true);

	h_tmp = round( dht.readHumidity() * 100 ) / 100.0F;
	
	// If we start getting totaly wrong readings
	if ( ( t_tmp == 0 && h_tmp == 0 ) || isnan(h_tmp) || isnan(t_tmp) || t_tmp > 100 || t_tmp < -60 )
	{
    Serial.println(F("Fail read DHT!"));
		return;
	}
	else
	{
		t = t_tmp;
		h = h_tmp;

		if ( t_measure == 1 )
			t = Fahrenheit(t);
	
	    float T = Kelvin(t); // Kelvin             
  	}
  
}

//Feet to Meter conversion
float Meter( float feet ) { return feet * 0.3048; }

//Celsius to Fahrenheit conversion
float Fahrenheit( float celsius ) { return 1.8 * celsius + 32; }

//Celsius to Kelvin conversion
float Kelvin( float celsius ) { return celsius + 273.15; }

//hPascals to inhg conversion
float iNHg( float hpa ) { return hpa * 0.02952998; }