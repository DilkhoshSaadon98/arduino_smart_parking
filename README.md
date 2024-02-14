#include <Wire.h>
#include <hd44780.h>                       // main hd44780 header
#include <hd44780ioClass/hd44780_I2Cexp.h> // i2c expander i/o class header

hd44780_I2Cexp lcd; // declare lcd object: auto locate & config exapander chip

// LCD geometry
const int LCD_COLS = 20;
const int LCD_ROWS = 4;

void setup()
{
  lcd.begin(LCD_COLS, LCD_ROWS);
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(3, 0);
  lcd.print("Hello, Miav");
  lcd.setCursor(0, 1);
  lcd.print("I Love You"); 
}

void loop() {}