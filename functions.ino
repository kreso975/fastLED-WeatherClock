// Interval for rotating display
unsigned long displayRotateInterval = 10000;
unsigned long lastDisplayChange = 0;
byte displayMode = 0;


void updateDisplay() {
    unsigned long currentMillis = millis();
    if (currentMillis - lastDisplayChange >= displayRotateInterval) {
        lastDisplayChange = currentMillis;
        displayMode = (displayMode + 1) % 4; // Rotate between 3 display modes
    }

    
    fill_solid(leds, NUM_LEDS, CRGB::Black); // Clear the display
    
    switch (displayMode) {
        case 0:
            drawTime(1, 0, CRGB(255, 0, 0), true, false); // Display time
            break;
        case 1:
            drawDate(1, 0, CRGB(255, 0, 0) ); // Display date
            break;
        case 2:
            drawTempHum(0, 0, CRGB::Green, true);
            break;
        case 3:
            drawTempHum(0, 0, CRGB::Blue, false);
            break;

    }
}


void drawLetter(int posx, int posy, char letter, CRGB color) {
    if ((posx > -FontWidth) && (posx < kMatrixWidth)) {
        for (int x = 0; x < FontWidth; x++) {
            for (int y = 0; y < FontHeight; y++) {
                if (bitRead(pgm_read_byte(&(Font[letter][FontWidth - 1 - x])), y) == 1) {
                    leds[XYsafe(posx + x, posy + y)] = color;
                }
            }
        }
    }
}

void drawTempHum(int x, int y, CRGB color, bool isTemperature)
{
    char tmpStr[10]; // Buffer to hold the converted string 

    // Adjust initial x position
    x = 4;
    if (isTemperature) {
        dtostrf(t, 3, 0, tmpStr);
        Serial.println("Temperature string: " + String(tmpStr));
        // Add "C" at y position 1 and x position -2
        drawLetter(x, 1, 'C', CRGB(255, 0, 0));
        x += FontWidth - 3; // Move to the next position
        // Add "." at y position 0 and x position -2
        drawLetter(x, -5, '.', CRGB(255, 0, 0));
        x += 6; // Move to the next position
    } else {
        dtostrf(h, 3, 0, tmpStr);
        Serial.println("Humidity string: " + String(tmpStr));
        // Add "%" at y position 0 and x position -2
        drawLetter(x, 1, '%', CRGB(0, 0, 255));
        x += FontWidth + 1; // Move to the next position
    }

    int length = strlen(tmpStr); // Get the length of the character array
    for (int i = 0; i < length; i++) {
        char letter = tmpStr[length - 1 - i]; // Reverse order
        drawLetter(x, y, letter, color); // Draw the letter
        x += FontWidth + 1; // Move to the next position
    }
}


void drawTime(int x, int y, CRGB color, bool colon, bool seconds) {
    // Get current time components
    int hours = timeClient.getHours();
    int minutes = timeClient.getMinutes();
    int secs = timeClient.getSeconds();

    // Clear the LEDs
    fill_solid(leds, NUM_LEDS, CRGB::Black); // Clear the display
    
    // Display time on LED matrix with swapped positions
    x -= 0;
    drawLetter(x, y, minutes % 10 + '0', color);
    x += FontWidth + 1;
    drawLetter(x, y, minutes / 10 + '0', color);
    x += FontWidth;
    if (colon) {
        if (secs % 2 == 0) {
            drawLetter(x - 1, y, ':', color);
        }
        x += 4;
    }
    drawLetter(x, y, hours % 10 + '0', color);
    x += FontWidth + 1;
    if (hours / 10 > 0) {
        drawLetter(x, y, hours / 10 + '0', color);
    }

    if (seconds)
        leds[XYsafe(secs * kMatrixWidth / 60, kMatrixHeight - 1)] = color;

}


void drawDate(int x, int y, CRGB color) {
    // Get current time
    time_t epochTime = timeClient.getEpochTime();
    struct tm *ptm = gmtime((time_t*)&epochTime);

    // Get current date components
    int day = ptm->tm_mday;
    int month = ptm->tm_mon + 1; // tm_mon is 0-11, so we add 1

    // Clear the display
    fill_solid(leds, NUM_LEDS, CRGB::Black);

    // Display date on LED matrix
    x -= 0;
    drawLetter(x, y, month % 10 + '0', color);
    x += FontWidth + 1;
    drawLetter(x, y, month / 10 + '0', color);
    x += FontWidth + 1;
    drawLetter(x-2, y+1, '.', color);
    x += 4;
    drawLetter(x, y, day % 10 + '0', color);
    x += FontWidth + 1;
    drawLetter(x, y, day / 10 + '0', color);
}



uint16_t XY( uint8_t x, uint8_t y)
{
  uint16_t i;
  
  if( kMatrixSerpentineLayout == false)
  {
    if (kMatrixVertical == false)
      i = (y * kMatrixWidth) + x;
    else
      i = kMatrixHeight * (kMatrixWidth - (x+1))+y;
  }

  if( kMatrixSerpentineLayout == true)
  {
    if (kMatrixVertical == false)
    {
      if( y & 0x01)
      {
        // Odd rows run backwards
        uint8_t reverseX = (kMatrixWidth - 1) - x;
        i = (y * kMatrixWidth) + reverseX;
      } 
      else
      {
        // Even rows run forwards
        i = (y * kMatrixWidth) + x;
      }
    }
    else
    { // vertical positioning
      if ( x & 0x01)
        i = kMatrixHeight * (kMatrixWidth - (x+1))+y;
      else
        i = kMatrixHeight * (kMatrixWidth - x) - (y+1);
    }
  }
  
  return i;
}

uint16_t XYsafe( uint8_t x, uint8_t y)
{
  if( x >= kMatrixWidth) return -1;
  if( y >= kMatrixHeight) return -1;
  return XY(x,y);
}

