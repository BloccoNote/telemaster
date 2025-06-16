#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup()
{
  lcd.init();                      // initialize the lcd 
  // Print a message to the LCD.
  lcd.setCursor(1,0);
  lcd.print("Hello, world!");
  lcd.setCursor(2,1);
  lcd.print("test: OK!");
}


void loop()
{
  lcd.backlight();
  delay(2000);
  lcd.noBacklight();
  delay(2000);
}
