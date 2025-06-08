/// @file telemaster_pv.ino
/// @brief contains usefull function to read strings from sd card
/// @note it follows this convention <index>'separator'<string>'\n' \n
/// example: 001-this example is usefull \n
/// example: 1@wow, this is crazy \n
/// example: 01 more usefull example \n
/// in example 1 separator is '-', in ex 2 is '@' ecc. \name
/// @authors Pulli & Gori


#include<SPI.h>
#include<SD.h>
#include<string.h>

#define CS_SD_PIN 10 //pin of CS pin of SD reader
#define BUTTON_PIN 3 // ...
#define FILE_NAME "test.txt" 
#define SEARCH_INDEX 24

/* content of test .txt
	001 test1
	002 test2
	3 test3
	00015 test4
	110 test5
	111 test6
	0024 test7
	1101  test8
*/

File Fileptr; 

void setup(){
	//in case of debug use     while(!digitalRead(BUTTON_PIN));
	pinMode(BUTTON_PIN, INPUT);

	//Serial handler
	Serial.begin(9600);
	Serial.println("[ INFO ] waiting for serial connection");
	while(!Serial);
	Serial.println("[ INFO ] successful serial connection");

	//SD handler
	if(!SD.begin(CS_SD_PIN)){
		Serial.println("[ ERROR ] Connection failed with SD Card");
		//stall the program
		stall();
	}
	Serial.println("[ INFO ] SD connection successfully established");
	Fileptr = SD.open(FILE_NAME, FILE_READ);
	if(FILE_NAME == NULL) stall();
}

/// @brief stalls the program: it's litteraly a while(1);
void stall(){
	Serial.println("[ INFO ] the program is at a standstill");
	while(1);
}

/// @brief reads from sd card untill it finds a '\n' or the end of file
/// @param buff pointer ONLY to string
/// @param length is filled with buff length 
/// @return 0 if no error, stall if error, 1 if reached end of file
/// @note buff pointer when initialized MUST be set to NULL!!!!
/// @bug last character of file will be lost safe if it's a '\n' or ' ' character
int read_line_sd(char* &buff , int &length){
	if(buff != NULL){
		delete buff; //if buff pointer is not initialized with null it crashes here
	}
	int return_val = 0;
	unsigned long starting_position = Fileptr.position();
	unsigned long end_position = 0;
	char current_char = 0;
	while(current_char != '\n'){
		if(Fileptr.available() <= 1){ //<= 1 handle "\n\0" scenarios
			return_val = 1;
			break;
		}
		current_char = Fileptr.read();
	}
	end_position = Fileptr.position();
	if(starting_position > end_position){
		Serial.println("[ ERROR ] end of line should be higher than start of line");
		stall();
	}
	length = end_position - starting_position;
	buff = new char [length];
	if(!Fileptr.seek(starting_position)){
		Serial.println("[ ERROR ] error in seek() method");
		stall();
	}
	Fileptr.read(buff, length);
	buff[length-1] = '\0';
	return return_val;
}

/// @brief converts all characters up to the first appearance of the 'separator' character to integers
/// @param buff string of characters
/// @param separator separates integers from chars
/// @return the value
int find_index(char buff[], char separator = ' '){
	int index;
	char* p;
	int value = 0;
	int exp = 1;
	if(!(p = strchr(buff, separator))){
		Serial.println("[ ERROR ] seprator character is not in the buffer");
		stall();
	}
	index = p - buff -1;
	for(int i = 0; i <= index ; i++){
		switch(buff[i]){
			case '0':
			break;
			default:
				value *= 10;
				value += (buff[i] - '0');
			break;
		}
	}
	return value;
}

void loop() {
	char* buff = NULL;
	int len = 0;
	int index = 0;
	int search;
	do{
		search = !read_line_sd(buff, len);
		index = find_index(buff);
		if(index == SEARCH_INDEX){
			Serial.println("FOUND THE STRING!");
			Serial.println(buff+4);
			break;
		}
		//debug staff
	}while(search);
	if(!search) Serial.println("no string found with index given");
	delete buff;
	Fileptr.close();
	stall();
}

