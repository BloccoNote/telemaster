//guarda test.txt per capire meglio, questo programma stampa la stringa associata al num dichiarato in int sel, e la manda al monitor seriale


#include<SPI.h>
#include<SD.h>

File nomeFile;
char stringa[40];
char numero[3];
char temp;
int num;
int sel = 5;
bool var =  false;
bool boh = false;


void setup() {
  Serial.begin(9600);
  while(!Serial){
  }
  Serial.println("inizializzazione scheda...");
  if(!SD.begin(10)){
    Serial.println("inizializzazione fallita");
    while(1);
  }
  Serial.println("inizializzato con successo");

  nomeFile = SD.open("test.txt", FILE_READ);
  
  if(nomeFile){
    while(var == false){
      for(int i = 0; i < 3; i++){
        numero[i] = nomeFile.read() - 48;
      }
      num = numero[0]*100 + numero[1]*10 + numero[2];
      Serial.println(num);
      if(num == sel){
        for(int k = 0; k < 39; k++){
          stringa[k] = nomeFile.read();
          if(stringa[k] == '\n'){
            Serial.println(stringa);
            var = true;
            break;
          }
        }
          var = 1;
      }else{
        while(boh == false){
          temp = nomeFile.read();
          
          if(temp == '\n'){
            boh = true;
          }else{
            boh = false;
          }
        }
        boh = false;
        var = false;
      }
    }
  }

}

void loop() {
  

}
