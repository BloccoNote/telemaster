/// @file sd_to_lcd.h
/// @brief declaration of funcitons & enums
/// @author Alessandro Maryni.

/// @mainpage Sd to Lcd library \n
/// easy handler of reading from an sd card and printing to lcd display
/// @subsection usage \n
/// @subsubsection global variables
/// class object should be global \n
/// \n
/// sd2Lcd S(rows, cols); \n
/// \n
/// note: by default rows and cols are 2 and 16 \n
/// maby allocates string array in global \n
/// \n
/// char* Lcd_rows[rows];
/// \n
/// @subsubsection setup()
/// in setup you should do something like: \n
/// \n
/// if(!S.init_sd_read(FILE_NAME, CS_SD_PIN)) S.stall(); \n
/// \n
/// and should allocate strings for printing in lcd. \n
/// \n
/// char lcd_row_1[cols+1] = {0} \n
/// ... \n
/// char lcd_row_n[cols+1] = {0} \n
/// \n
/// and insert them in string array \n
/// \n 
/// lcd_rows[0] = lcd_row_1; ...
/// \n
/// [continua a scrivere documentazione]

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
  bool is_sd = false;
  //lcd_displayer variables
  int rows = 2;
  int cols = 16;
  int current_pos = 0;
  int current_row = 0;

public:
  ///@name CONSTRUCTORS AND DESTRUCTORS
  sd2Lcd(int r = 2, int c = 16);
  sd2Lcd(const sd2Lcd &X);
  ~sd2Lcd();

  /// @name INITS

  void Init(int r = 2, int c = 16);
  void Init(const sd2Lcd &X);

  /// @name SD FUNCTIONS

  bool init_sd_read(const char* filename, int cs_pin);
  bool read_line_sd(char *&buff);
  int find_index(char* buff, const char separator = ' ');
  int find_and_remove_index(char* &buff, const char separator = ' ');
  bool find_sd_line_by_index(char* &buff, int index, const char separator = ' ');

  /// @name LCD STRING HANDLER & TOKENIZER

  void SetText(char* in);
  int find_string_len(const char* str, char end_char = '\n');
  bool Get_print_token(char** lcd_lines);
  void reset_print_token();

  /// @name UTILILTY FUNCTIONS

  void PrintMsg(Rep msgtype, const char* msg);
  void stall();

  /// @name OVERLOADS

  sd2Lcd& operator=(const sd2Lcd &X);

};

#endif