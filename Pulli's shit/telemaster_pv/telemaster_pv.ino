//TELEMASTER V 1.01

#include <LiquidCrystal_I2C.h>
//#include <time.h>
#include "sd_to_lcd.h"
#include <stdlib.h>
#include <time.h>

#define INTERRUPT_PIN 2
#define ANALOG_BUTTON_PIN A7
#define CS_SD_PIN 7
#define FILE_NAME "test.txt"

/*

NOTE: I valori valgono per le resistenze date. Funzionano in SIMULAZIONE SU THINKERCAD, non ho provato irl :
Link per il circuito su thinkercad (NON MODIFICARE)                                                           <-- modo 100% safe per non farsi cuzzare il circuito di thinker cad
 https://www.tinkercad.com/things/eSHNgc3p2s1-telemaster?sharecode=-aP5_nGmigwwymauiQA1TFkWA68BaPdvQ_gAVPeCcEM

 testando su arduino nano conviene usare valori sotto al 1kohm perché altrimenti il valore del voltaggio è troppo basso per triggerare l'irnterrupt
 cambiato da adafruit a liquidcrystal i2c

== circuit for multiple buttons==

         Analog pin 7  interrutp pin (D2 o 3)
            |______________|
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

== SD Cconfig =======
 - ON ARDUINO nano
	MOSI -> D11
	MISO -> D12
	SCK  -> D13
  #define CS_SD_PIN 7
*/

LiquidCrystal_I2C lcd(0x27, 16, 2);
sd2Lcd S;
char* tkn[2];
char* buff = NULL;
bool end;
bool BackLight;
volatile bool button_pressed = false;
int value, mode, index_value = 0;
float val;

//multiple interrupt per button
volatile bool enalble_interrupt = true;
time_t int_time;

/* incorportes an anti-bouncing functions to button */
void handleInterrupt(){
  if(enalble_interrupt){
    int_time = millis();
    enalble_interrupt = false;
  }
  if(millis() - int_time > 25){
  	button_pressed = true;
    enalble_interrupt = true;
    mode = analogRead(ANALOG_BUTTON_PIN);
  	value = mode / 100;
  }
}


/* NO anti-debouce 
void handleInterrupt(){
  button_pressed = true;
  mode = analogRead(ANALOG_BUTTON_PIN);
  value = mode / 100;
  Serial.println(value);
}
*/

byte backslash[8] = {
  B00000,
  B10000,
  B01000,
  B00100,
  B00010,
  B00001, 
  B00000,
  B00000
};


void wait(size_t time_ms){
  time_t time = millis();
  while(millis() - time < time_ms && !button_pressed);
}

void backl(){
  if(BackLight) return;
  BackLight = true;
  lcd.backlight();
}

void Nobackl(){
  if(!BackLight) return;
  BackLight = false;
  lcd.clear();
  lcd.noBacklight();
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(ANALOG_BUTTON_PIN, INPUT);
  // serial
  Serial.begin(9600);
  while(!Serial);
  S.PrintMsg(Info, "OK Ser");
  // interrupt
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), (handleInterrupt) , RISING);
  // lcd
  lcd.init();
  lcd.backlight();
  lcd.createChar(0, backslash); // Store the custom backslash character
  // sd2Lcd
  if(!S.init_sd_read(FILE_NAME, CS_SD_PIN)) S.stall();
  // static allocation >> dynamic for necessary stuff
  char row1[17] = {0};
  char row2[17] = {0};
  tkn[0] = row1;
  tkn[1] = row2;
  //rand
  randomSeed(analogRead(0));
}


void loop(){
  if(value != 0 && value != index_value){
    lcd.clear();
    backl();
    switch (value){
      case 7:
      {
      button_pressed  = false;
      index_value = value;
      int indx = random(0, 12);
      S.find_sd_line_by_index(buff, indx, ' ');
      delete[] buff;
      buff = NULL;
      lcd.clear();
      while(!button_pressed){
        do{
          end = S.Get_print_token(tkn);
          lcd.setCursor(0,0);
          lcd.print(tkn[0]);
          lcd.setCursor(0,1);
          lcd.print(tkn[1]);
          wait(2000);
          if(end) lcd.clear();
        }while(end && !button_pressed);
        S.reset_print_token();
      }
      break;
      }
      case 8:
      {
        button_pressed = false;
        index_value = value;
        char loading[] = {'-', byte(0), '|', '/', '\0'};
        int i = 0;
        lcd.setCursor(4,0);
        lcd.print("Loading...");
        while(!button_pressed){
          lcd.setCursor(7,1);
          lcd.print(loading[i++]);
          wait(600);
          if(i > 3) i = 0;
        }
        break;
      }
      case 9:
        index_value = value;
        button_pressed = false;
        do{
          lcd.setCursor(0,0);
          lcd.print("Siete Fottuti!");
          BackLight ? Nobackl() : backl();
          wait(1000);
        }while(!button_pressed);
        break;

      case 10:
        index_value = value;
        Nobackl();
        break;

      default:
        break;
    }
    delay(100);
  }
  button_pressed = false;
}
