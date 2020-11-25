#include <Arduino.h>
#include <SPI.h>
#include "max7219.h"

// 15 - GPIO15 (CS), 8 - number of digits, 5 - intensivity (brightness)
Max7219 max7219 = Max7219(15, 8, 5);

/* ESP32 SPI
  SCK   - GPIO_18
  MISO  - GPIO_21
  MOSI  - GPIO_23
*/

void setup() 
{
  SPI.begin(18, 21, 23);
  max7219.Begin();
}

void loop() 
{
  max7219.DecodeOn();
  max7219.SetIntensivity(5);
  
  max7219.PrintNtos(max7219.DIGIT_7, 1234, 6);
  delay(1000);
  max7219.Clean();

  max7219.PrintItos(max7219.DIGIT_4, 5678);
  delay(1000);
  max7219.Clean();

  max7219.PrintFtos(max7219.DIGIT_8, -45.678f, 2);
  delay(1000);
  max7219.Clean();

  for (uint8_t i = 0; i < 8; i++)
  {
    max7219.PrintDigit(i + 1, i + 1, false);
    delay(100);
  }

  for (uint8_t i = 0x0F; i > 0x00; i--)
  {
    max7219.SetIntensivity(i);
    delay(100);
  }
  
  for (uint8_t i = 0; i < 0x0F; i++)
  {
    max7219.SetIntensivity(i);
    delay(100);
  }
  
  for (uint8_t i = 8; i > 0 ; i--)
  {
    max7219.PrintItos(i, 87654321);
    delay(200);
    max7219.Clean();  
  }
  
  max7219.Clean();
  delay(1000);
}
