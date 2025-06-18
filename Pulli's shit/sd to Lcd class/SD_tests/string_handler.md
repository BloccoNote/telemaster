#include <stdlib.h>
#include <string.h>
const int MAX_STRING = 64; 
const int MAX_TRY_COUNT = 10;


char *buffer, *ptr;
int rows = 2;
int cols = 16;
int current_pos = 0;
int current_row = 0;
int text_len = 0;

/// @brief Set text to be tokenized
/// @param in string of text
/// @note MUST end with '\0' character
void SetText(char* in){
    current_pos = 0;
    current_row = 0;
    text_len = 0;
    if(in == NULL){
        PrintMsg(Error, "'in' buffer pointer is NULL");
        return;
    }
    //Init(rows, cols);
    text_len = find_string_len(in, '\0');
    if(buffer != NULL){
      delete buffer;
      buffer = NULL;
    }
    buffer = new char[text_len+1];
    memset(buffer, 0, text_len+1);
    copy_string(buffer, in);
    ptr = buffer;
}

/// @brief finds the lenght of a string utill 'end_char'
/// @param str the string
/// @param end_char terminating character, defaulted to '\0'
/// @return the lenght
int find_string_len(const char* str, char end_char){
    const char* endchr = NULL;
    if(str ==  NULL) {
        PrintMsg(Error, "'str' buffer is NULL");
        return 0;
    }
    endchr = strchr(str, end_char);
    if((endchr = strchr(str, end_char)) == NULL){
        PrintMsg(Error, "could not find end_char in string str");
        return 0;
    }
    return (int)(endchr - str);
}


/// @brief Safely copy a string to another, memcopy sucks
/// @param dest destination string
/// @param source source string
void copy_string(char* dest, char* source){
    int len;
    if(dest == NULL || source == NULL){
        PrintMsg(Error, "source string or destination string is a NULL pointer");
        return;
    }
    if((len = find_string_len(source)) ==0 ) return;
    for(int i = 0; i < len; i++){
        dest[i] = source[i];
    }
}

// ========================================================
/// @brief Fills the buffer 'lcd_lines' with tokens of 'buffer' so that can fit a display of rows * cols dimentions
/// @param lcd_lines PREALLOCATED matrix of size rows * (cols + 1)
/// @return false if end or error
/// @note the +1 in columns is for '\0' character
bool Get_print_token(char** lcd_lines){
    if(ptr == NULL){
        PrintMsg(Error, "set text first, internal buffer is empty");
        return false;
    }
    for(int i = 0; i < rows; i++){
        memset(lcd_lines[i], 0, cols + 1);
    }
    int c = cols - 1;
    int try_count = 0;
    bool ret_val = true;
    bool end_r = false;
    do{
        //checks if is the end of text
        if(text_len - current_pos <= cols){
            for(int i = 0; i < text_len - current_pos; i++){
                lcd_lines[current_row][i] = ptr[i];
            }
            return false;
        }
        //if an entire row fits in buff print that one
        else if(ptr[c] == ' ' || ptr[c] == '\n' || ptr[c] == '\0'){
            if(ptr[c] == '\0' || ptr[c+1] == '\0') ret_val = false;
            for(int i = 0; i < cols; i++){
                lcd_lines[current_row][i] = ptr[i];
            }
            current_row++;
            current_pos += c;
            ptr += c;
        }
        // find shortest sentence that fits
        else{
            int i = c;
            for(; i >=  0; i--){
                if(ptr[i] == ' ') break;
            }
            for(int j = 0; j < i; j++){
                lcd_lines[current_row][j] = ptr[j];
            }
            current_row++;
            current_pos += i+1;
            ptr += i+1;
        }
        if(try_count >= MAX_TRY_COUNT){
            PrintMsg(Error, "maximum tries exceeded, word is longer than 16 chars or other");
        }
        try_count++;
        end_r = current_row < rows;
    }while((text_len - current_pos ) == 0 || end_r);    
    if(!end_r) current_row = 0;
    return true;
}

