/*                    TELEMASTER V 1.2
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

#include <LiquidCrystal_I2C.h>
#include <stdlib.h>
#include <time.h>
#include "sd_to_lcd.h"

#define INTERRUPT_PIN 2       //digital pin for interrupt
#define ANALOG_BUTTON_PIN A7  
#define CS_SD_PIN 7
#define FILE_NAME "test.txt"

#define N_SKULL 3 //number of skull to be printed in "sete fottuti" screen

//lcd vars
LiquidCrystal_I2C lcd(0x27, 16, 2);
bool BackLight;
// sd2Lcd vars
sd2Lcd S;
char* tkn[2];
char* buff = NULL;
// handler vars
bool end;
int value, mode, index_value, spacing = 0;
float val;
//debouncing vars
volatile bool enalble_interrupt = true;
time_t int_time;

// CUSTOM LCD CHARS 2 out of 8

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

byte skull[8] = {
  B00000,
  B01110,
  B10101,
  B10101,
  B11011,
  B01110,
  B01010,
  B00000,
};

// FUNCTION DECLARATIONS
void handleInterrupt();
void wait(size_t time_ms);
void backl();
void Nobackl();

void setup()
{
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
  lcd.createChar(0, backslash); // Store the custom character backslash 
  lcd.createChar(1, skull);     // Store the custom character skull
  // sd2Lcd
  if(!S.init_sd_read(FILE_NAME, CS_SD_PIN)) S.stall();
  // static allocation >> dynamic for necessary stuff
  char row1[17] = {0};
  char row2[17] = {0};
  tkn[0] = row1;
  tkn[1] = row2;
  //rand
  randomSeed(analogRead(0));
  // estetic shit  --- it centers skulls in "siete fottuti screen"
  spacing = N_SKULL % 2 ?            \
            7 - (int)(N_SKULL / 2) : \
            8 - (int)(N_SKULL / 2) ;

  // printing intro message
  lcd.setCursor(2,0);
  lcd.print("Welcome to");
  lcd.setCursor(1,1);
  lcd.print("Telemaster V.1");
}


void loop(){
  if(value != 0 && value != index_value){
    lcd.clear();
    switch (value){
      case 7: //                                RANDOM STRING FROM SD
      {
      index_value = value;
      while(value == index_value){
        int indx = random(0, 12);
        S.find_sd_line_by_index(buff, indx, ' ');
        delete[] buff;
        buff = NULL;
        lcd.clear();
        backl();
        do{
          end = S.Get_print_token(tkn);
          lcd.setCursor(0,0);
          lcd.print(tkn[0]);
          lcd.setCursor(0,1);
          lcd.print(tkn[1]);
          wait(2000);
          if(end) lcd.clear();
        }while(end && (value == index_value));
        wait(1000);
      }
      S.DelText();
      break;
      }
      case 8: //                                   LOADING SCREEN 
      {
        index_value = value;
        char loading[] = {'-', byte(0), '|', '/', '\0'};
        int i = 0;
        lcd.setCursor(4,0);
        backl();
        lcd.print("Loading...");
        while(value == index_value){
          lcd.setCursor(7,1);
          lcd.print(loading[i++]);
          wait(600);
          if(i > 3) i = 0;
        }
        break;
      }
      case 9: //                                     "siete Fottuti"
        index_value = value;
        lcd.setCursor(1,0);
        lcd.print("Siete Fottuti!");
        lcd.setCursor(spacing,1);
        for(int i = 0; i < N_SKULL; i++){ lcd.print((char) byte(1)); }
        do{
          wait(1000);
          BackLight ? Nobackl() : backl();
        }while(value == index_value);
        break;
      case 10: //                                         LCD OFF
        index_value = value;
        lcd.clear();
        Nobackl();
        break;

      default:
        // if you get something werid lcd_off
        value = 10;
        break;
    }
  }
  delay(100);
}

/* incorportes an anti-bouncing functions to button */
void handleInterrupt(){
  if(enalble_interrupt){
    int_time = millis();
    enalble_interrupt = false;
  }
  if(millis() - int_time > 10){
    enalble_interrupt = true;
    mode = analogRead(ANALOG_BUTTON_PIN);
    if(mode == 0 | mode > 980) mode = 1000;
  	value = mode / 100;
  }
}


/* NO anti-debouce 
void handleInterrupt(){
  mode = analogRead(ANALOG_BUTTON_PIN);
  value = mode / 100;
  Serial.println(value);
}
*/

void wait(size_t time_ms){
  time_t time = millis();
  while(millis() - time < time_ms && value == index_value);
}

void backl(){
  if(BackLight) return;
  BackLight = true;
  lcd.backlight();
}

void Nobackl(){
  if(!BackLight) return;
  BackLight = false;
  lcd.noBacklight();
}
