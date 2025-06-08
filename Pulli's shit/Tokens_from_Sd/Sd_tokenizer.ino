#include "Sd_tokenizer.h"


Sd_tokenizer::Sd_tokenizer(int cs_pin, const char* filename, int r, int c) : buff(NULL){
	Init(cs_pin, filename);
	S.Init(r,c);
}

Sd_tokenizer::~Sd_tokenizer(){
	if(buff != NULL){
		delete buff;
		buff = NULL;
	}
}

void Sd_tokenizer::Init(int cs_pin, const char* filename){
	if(filename == NULL || cs_pin < 0) stall();
	if(!fileptr){
		if(!SD.begin(cs_pin)){
			PrintMsg(Error, "Connection failed with SD Card");
			//stall the program
			stall();
		}
		PrintMsg(Info, "SD connection successfully established");
		fileptr = SD.open(filename, FILE_READ);
	}
}


int Sd_tokenizer::read_line_sd(){
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
		PrintMsg(Error, "end of line should be higher than start of line");
		stall();
	}
	buff_len = end_position - starting_position;
	buff = new char [buff_len];
	if(!Fileptr.seek(starting_position)){
		PrintMsg(Error, "error in seek() method");
		stall();
	}
	Fileptr.read(buff, buff_len);
	buff[buff_len-1] = '\0';
	return return_val;
}

/// @brief converts all characters up to the first appearance of the 'separator' character to integers
/// @param separator separates integers from chars
/// @return the value
int Sd_tokenizer::find_index(char separator){
	int index;
	char* p;
	int value = 0;
	if(!(p = strchr(buff, separator))){
		PrintMsg(Error, "seprator character is not in the buffer");
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

int Sd_tokenizer::find_and_remove_index(char separator){
	int index;
	char* p = NULL;
	int value = 0;
	int i = 0
	if(!(p = strchr(buff, separator))){
		PrintMsg(Error, "seprator character is not in the buffer");
		stall();
	}
	index = p - buff -1;
	for(; i <= index ; i++){
		switch(buff[i]){
			case '0':
			break;
			default:
				value *= 10;
				value += (buff[i] - '0');
			break;
		}
	}
	index++;
	p = buff;
	buff_len -= index;
	buff = new char[buff_len];
	memccpy(buff, p+index, buff_len);
	delete p;
	p = NULL;
	return value;
}

void Sd_tokenizer::find_sd_line(const char separator, int index){
	int search, indx;
	do{
		search = !read_line_sd(buff, buff_len);
		indx = find_and_remove_index(buff);
		if(indx == index){
			S.SetText(buff);
			break;
		}
		//debug staff
	}while(search);
	if(!search) PrintMsg(Warning, "no string found with index given");
}

void Sd_tokenizer::get_string_to_print(char* tkn){
	S.Get_print_token(tkn);
}


void sd_tokenizer::stall(){
	PrintMsg(Info, "the program is at a standstill");
	while(1);
}

void Sd_tokenizer::PrintMsg(Rep msgtype, const char* msg){
	const char* Types[]= {
		"[ ERROR ] ",
		"[ INFO ] ",
		"[ WARNING] "
	};
	if(!Serial) return;
	Serial.print(Types[msgtype]);
	Serial.println(msg);
}