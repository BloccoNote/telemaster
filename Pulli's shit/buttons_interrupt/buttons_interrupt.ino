#include <Adafruit_LiquidCrystal.h>
#include <time.h>

#define INTERRUPT_PIN 3
#define ANALOG_BUTTON_PIN A1

/*

NOTE: I valori valgono per le resistenze date. Funzionano in SIMULAZIONE SU THINKERCAD, non ho provato irl :
Link per il circuito su thinkercad (NON MODIFICARE)
 https://www.tinkercad.com/things/eSHNgc3p2s1-telemaster?sharecode=-aP5_nGmigwwymauiQA1TFkWA68BaPdvQ_gAVPeCcEM
== circuit ==

         Analog pin 1
            |
Ground--1K--|--------|--------|-------|
            |        |        |       |
           btn1     btn2     btn3    btn4 
            |        |        |       |    
         220 Ohm  330 Ohm  680 Ohm   1.0K 
            |--------|--------|-------|----- +5V


  NOTE: if you whant to use different resistence values you have to calibrate it agan
*/

Adafruit_LiquidCrystal Lcd(0);
volatile bool button_pressed = false;
int value, mode, index_value = 0;
//multiple interrupt per button
volatile bool enalble_interrupt = true;
time_t int_time;

/*
void handleInterrupt(){
  if(enalble_interrupt){
    enalble_interrupt = false;
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
}


void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(A5, INPUT);
  Serial.begin(9600);
  while(!Serial);
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), (handleInterrupt) , RISING);
  Lcd.begin(16, 2);
  Lcd.setBacklight(1);
}


void loop(){
  
  if(button_pressed){
    mode = analogRead(ANALOG_BUTTON_PIN);
  	//Serial.println(mode);
  	value = mode * 5 / 650;
    //Serial.println(value);
    //Serial.println("INSIDE BODY");
    if(value != 0 && value != index_value){
      switch (value){
        case 3:
          Serial.println("Button 1 pressed!\n");
          Lcd.setCursor(0,0);
          Lcd.print("Butt 1");
          index_value = value;
          break;

        case 4:
          Serial.println("Button 2 pressed!\n");
          Lcd.setCursor(0,0);
          Lcd.print("Butt 2");
          index_value = value;
          break;

        case 5:
          Serial.println("Button 3 pressed!\n");
          Lcd.setCursor(0,0);
          Lcd.print("Butt 3");
          index_value = value;
          break;

        case 6:
          Serial.println("Button 4 pressed!\n");
          Lcd.setCursor(0,0);
          Lcd.print("Butt 4");
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