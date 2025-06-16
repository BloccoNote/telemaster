#include <LiquidCrystal_I2C.h>
#include <time.h>

#define INTERRUPT_PIN 2
#define ANALOG_BUTTON_PIN A7

/*

NOTE: I valori valgono per le resistenze date. Funzionano in SIMULAZIONE SU THINKERCAD, non ho provato irl :
Link per il circuito su thinkercad (NON MODIFICARE)                                                           <-- modo 100% safe per non farsi cuzzare il circuito di thinker cad
 https://www.tinkercad.com/things/eSHNgc3p2s1-telemaster?sharecode=-aP5_nGmigwwymauiQA1TFkWA68BaPdvQ_gAVPeCcEM

 testando su arduino nano conviene usare valori sotto al 1kohm perché altrimenti il valore del voltaggio è troppo basso per triggerare l'irnterrupt
 cambiato da adafruit a liquidcrystal i2c

== circuit for multiple buttons==

         Analog pin 1
            |
Ground--1K--|--------|--------|-------|
            |        |        |       |
           btn1     btn2     btn3    btn4 
            |        |        |       |    
            |     100 Ohm   220 Ohm  330 Ohm
            |--------|--------|-------|----- +5V

  NOTE: if you whant to use different resistence values you have to calibrate it agan

== LCD_I2C display ==
 - ON ARDUINO nano
    SDA_pin -> Analog pin 4
    SCL_pin -> Analog pin 5

*/

LiquidCrystal_I2C lcd(0x27, 16, 2);
volatile bool button_pressed = false;
int value, mode, index_value = 0;
float val;
//multiple interrupt per button
volatile bool enalble_interrupt = true;
time_t int_time;

/* incorportes an anti-bouncing functions to button
void handleInterrupt(){
  if(enalble_interrupt){
    enalble_interrupt = false;Serial.println(value);
  	int_time = millis();
  }
  if(millis() - int_time > 50){
  	button_pressed = true;
  	Serial.println("INTERRUPT!");
    enalble_interrupt = true;
  }
}
*/

void handleInterrupt(){
  button_pressed = true;
  Serial.println("PRESSED!");
}


void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(ANALOG_BUTTON_PIN, INPUT);
  Serial.begin(9600);
  while(!Serial);
  Serial.println("Serial connection established!");
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), (handleInterrupt) , RISING);
  lcd.init();
  lcd.backlight();
}


void loop(){
  if(button_pressed){
    mode = analogRead(ANALOG_BUTTON_PIN);
  	//Serial.println(mode);
  	value = mode / 100;
    Serial.println(value);
    //Serial.println("INSIDE BODY");
    if(value != 0 && value != index_value){
      switch (value){
        case 7:
          Serial.println("Button 1 pressed!\n");
          lcd.setCursor(0,0);
          lcd.print("Butt 1");
          index_value = value;
          break;

        case 8:
          Serial.println("Button 2 pressed!\n");
          lcd.setCursor(0,0);
          lcd.print("Butt 2");
          index_value = value;
          break;

        case 9:
          Serial.println("Button 3 pressed!\n");
          lcd.setCursor(0,0);
          lcd.print("Butt 3");
          index_value = value;
          break;

        case 10:
          Serial.println("Button 4 pressed!\n");
          lcd.setCursor(0,0);
          lcd.print("Butt 4");
          index_value = value;
          break;

        default:
          break;
      }
      delay(300);
    }
    button_pressed = false;
  }
}
