#ifndef _SD_TO_LCD_
#define _SD_TO_LCD_

#include<SPI.h>
#include<SD.h>
#include <stdlib.h>
#include <string.h>

const int MAX_STRING = 64; 
const int MAX_BUFF_LEN = 105; 
//fits "disse la vacca al mulo, oggi ti puzza il culo, disse il mulo alla vacca, ho appena fatto la cacca!" -> index 001 of test.txt
const int MAX_TRY_COUNT = 10;


typedef enum {
	Error = 0,
	Info,
	Warning,
}Rep;

class sd2Lcd{
private:
  //Sd reader variables
  File fileptr;
  char *buffer, *ptr = NULL;
  int buff_len = 0; 
  int text_len = 0;
  //lcd_displayer variables
  int rows = 2;
  int cols = 16;
  int current_pos = 0;
  int current_row = 0;

public:
  sd2Lcd(int r = 2, int c = 16);
  sd2Lcd(const sd2Lcd &X);
  ~sd2Lcd();

  void Init(int r = 2, int c = 16);
  void Init(const sd2Lcd &X);

  bool init_sd_read(const char* filename, int cs_pin);
  bool read_line_sd(char *&buff);
  int find_index(char* buff, const char separator = ' ');
  int find_and_remove_index(char* &buff, const char separator = ' ');
  bool find_sd_line_by_index(char* &buff, int index, const char separator = ' ');

  void SetText(char* in);
  int find_string_len(const char* str, char end_char = '\n');
  void copy_string(char* dest, char* source);
  bool Get_print_token(char** lcd_lines);
  void reset_print_token();

  void PrintMsg(Rep msgtype, const char* msg);
  void stall();

  sd2Lcd& operator=(const sd2Lcd &X);

};

#endif