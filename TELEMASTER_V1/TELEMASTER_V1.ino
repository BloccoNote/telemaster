#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); //rivedi questo
#include<SPI.h>   //pare che per usare l'sd questo si attacchi a pin 11, 12 e 13
#include<SD.h>    //guarda questo

//robe per il leggitips
File nomeFile;
char stringa[128];
char numero[3];
char temp;
int num;
int sel;
bool var = false;
bool boh = false;

//bottoni
int bot1 = 2; //bottone attendere
int bot2 = 3; //bottone tips
int bot3 = 4; //bottone SIETE FOTTUTI!
int bot4 = 5; //bottone spegni


void setup() {
  //setup bottoni
  pinMode(bot1, INPUT);
  pinMode(bot2, INPUT);
  pinMode(bot3, INPUT);
  pinMode(bot4, INPUT);
  //setup lcd
  lcd.init();
  lcd.setCursor(0,0);
  lcd.backlight();
  lcd.print("telemaster v.1");
  //setup scheda sd
  if(!SD.begin(10)){
    lcd.setCursor(0,0);
    lcd.print("errore scheda sd");
    while(true){
    }
  }
  nomeFile = SD.open("test.txt", FILE_READ);
  if(nomeFile == NULL){
    lcd.setCursor(0,0);
    lcd.print("errore file   ");
    while(true){
    }
  }
  delay(3000);
  lcd.setCursor(0,0);
  lcd.clear();
  lcd.noBacklight();

}

void loop() {
    if(digitalRead(bot1) == HIGH){
      lcd.clear();
      lcd.backlight();
      lcd.setCursor(0,0);
      lcd.print("attendere");
      delay(100);
    }
    if(digitalRead(bot2) == HIGH){
      while(true){
        


        
      stampasuschermo(stringa);
      if(digitalRead(bot4) == HIGH || digitalRead(bot1) == HIGH || digitalRead(bot2) == HIGH){
          break;
        }
      }
    }
    if(digitalRead(bot3) == HIGH){
      lcd.clear();
      lcd.backlight();
      lcd.setCursor(0,0);
      lcd.print("SIETE FOTTUTI!!!");
      while(true){
        lcd.backlight();
        delay(500);
        lcd.noBacklight();
        delay(500);
        if(digitalRead(bot4) == HIGH || digitalRead(bot1) == HIGH || digitalRead(bot2) == HIGH){
          break;
        }
      }
    }
    if(digitalRead(bot4) == HIGH){
      lcd.clear();
      lcd.noBacklight();
      lcd.setCursor(0,0);
      delay(100);
    }
}

void stampasuschermo(char * stringa){
  lcd.setCursor(0,0);
  lcd.backlight();
  lcd.print("stampo stringa");
  delay(100);
}
