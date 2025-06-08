#ifndef _SD_TOKENIZER_
#define _SD_TOKENIZER_


#include<SPI.h>
#include<SD.h>
 
#define _ARDUINO_
#include "string handler.h"

typedef enum {
	Error = 0,
	Info,
	Warning,
}Rep;


class Sd_tokenizer
{
private:
	int cs_sd_pin;
	char* buff = NULL;
	int buff_len;
	File fileptr;
	string_handler S;

public:
	Sd_tokenizer(int cs_pin, const char* filename, int r, int c);
	~Sd_tokenizer();

	void Init(int cs_pin, const char* filename);

	int read_line_sd();
	int find_index(const char separator = ' ');
	int find_and_remove_index(const char separator = ' ');
	void find_sd_line(int index, const char separator = ' ');
	void get_string_to_print(char* tkn);


	///@name utility functions
	///@{
	void stall();
	void PrintMsg(Rep msgtype, const char* msg);

	///@}
};











#endif