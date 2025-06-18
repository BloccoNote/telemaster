#include "sd_to_lcd.h"


// SD Cconfig
// MOSI -> D11
// MISO -> D12
// SCK  -> D13
#define CS_SD_PIN 7 //pin of CS pin of SD reader
#define FILE_NAME "test.txt"

bool once = true;
sd2Lcd S;
char* buff = NULL;
char* tkn[2];

void setup(){
  Serial.begin(9600);
  while(!Serial);
	Serial.println("Connected succesfully!");
  if(!S.init_sd_read(FILE_NAME, CS_SD_PIN)) S.stall();
	tkn[0] = new char[17];
	tkn[1] = new char[17];
}

void loop() {
	if(once){
		bool end;
  //while(read_line_sd()){
		//if(!S.read_line_sd(buff)) S.stall();
    //int index = S.find_and_remove_index(buff);
		S.find_sd_line_by_index(buff, 10, ' ');
		S.SetText(buff);
		while(1){
			do{
				end = S.Get_print_token(tkn);
				Serial.println(tkn[0]);
				Serial.println(tkn[1]);
				delay(3000);
			}while(end);
			S.reset_print_token();
		}
		once = false;
		delete tkn[0];
		delete tkn[1];
		Serial.println("==end==");
  }
}