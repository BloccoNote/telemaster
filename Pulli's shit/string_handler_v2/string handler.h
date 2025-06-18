/// @attention to use this class in arduino ide you have to define a variable, here an example on how to use it  \n
/// #define _ARDUINO_  \n
/// #include "string handler.h" \n
/// ...


#ifndef _STRING_HANDLER_
#define _STRING_HANDLER_

#include <string.h>
#include <stdlib.h>

#ifndef _ARDUINO_

    #include <iostream>
    #include <bits/stdc++.h>

#else

    #include "Arduino.h"
    ///NOTE: you can find Vector library in here: https://docs.arduino.cc/libraries/vector/
    #include "Vector.h"

#endif //_ARDUINO_

using namespace std;

///TODO: sarebbe fico se ogni funzione restituisse una string da stampare in caso di errore

const int MAX_STRING = 1024; 
const int MAX_TRY    = 10;


class string_handler
{
protected:
    char buffer[MAX_STRING], *ptr;
    int rows = 2;
    int cols = 16;
    int current_pos = 0;
    int current_row = 0;
    int text_len = 0;

public:
    string_handler(int r = 2, int c = 16);
    ~string_handler();

    void Init(int r = 2, int c = 16);
    void clear_buffers();
    void SetText(char* in);

    bool Get_print_token(char** lcd_lines);


    /// @name UTILITY FUNCTIONS
    /// @{
    void PrintError(const char* ErrMsg);

    void copy_string(char* dest,char* source);
    int find_string_len(const char* str, char end_char = '\0');

    ///@}
};


#endif //_STRING_HANDLER_