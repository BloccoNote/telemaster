
#ifndef _STRING_HANDLER_
#define _STRING_HANDLER_

#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <bits/stdc++.h>
using namespace std;

///TODO: sarebbe fico se ogni funzione restituisse una string da stampare in caso di errore



const int MAX_STRING = 1024; 
const int MAX_TRY    = 30;


class string_handler
{
protected:
    char buffer[MAX_STRING];
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

    bool Get_print_token(char* tkn);


    /// @name UTILITY FUNCTIONS
    /// @{
    void PrintError(const char* ErrMsg);

    void copy_string(char* dest,char* source);
    int find_string_len(const char* str, char end_char = '\0');

    void apend_string_to_vector(vector<char> &final_buffer, char* str, int len);
    void convert_char_vector_to_string(char* buff, vector<char> src);
    ///@}
};


#endif //_STRING_HANDLER_