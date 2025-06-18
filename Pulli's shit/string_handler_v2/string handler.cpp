#include "string handler.h"


/// @brief Constructor
/// @param r rows
/// @param c columns
string_handler::string_handler(int r, int c){
    Init(r,c);
    
}

/// @brief destructor
string_handler::~string_handler()
{
}

/// @brief Initialize all variables
/// @param r rows
/// @param c cols
void string_handler::Init(int r, int c){
    if(r <= 0 || c <= 0){
        PrintError("Rows or Columns should be an integer > 0");
        PrintError("Setting defaul values: rows = 2, columns = 16");
        r = 2;
        c = 16;
    }
    rows = r;
    cols = c;
    clear_buffers();
}

/// @brief clears the buffers used to me more
void string_handler::clear_buffers(){
    current_pos = 0;
    text_len = 0;
    text_len = 0;
    memset(buffer, 0, MAX_STRING);
}

/// @brief Set text to be tokenized
/// @param in string of text
/// @note MUST end with '\0' character
void string_handler::SetText(char* in){
    if(in == NULL){
        PrintError("'in' buffer pointer is NULL");
        return;
    }
    Init(rows, cols);
    text_len = find_string_len(in);
    copy_string(buffer, in);
    ptr = buffer;
}

/// @brief Fills the buffer 'lcd_lines' with tokens of 'buffer' so that can fit a display of rows * cols dimentions
/// @param lcd_lines PREALLOCATED matrix of size rows * (cols + 1)
/// @return false if end or error
/// @note the +1 in columns is for '\0' character
bool string_handler::Get_print_token(char** lcd_lines){
    if(ptr == NULL){
        PrintError("set text first, internal buffer is empty");
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
        if(try_count >= MAX_TRY){
            PrintError("maximum tries exceeded, word is longer than 16 chars or other");
        }
        try_count++;
        end_r = current_row < rows;
    }while((text_len - current_pos ) == 0 || end_r);    
    if(!end_r) current_row = 0;
    return true;
}


#ifndef _ARDUINO_
/// @brief The error message fucntion
/// @param ErrMsg the string to be printed
/// TODO: When porting this class to arduino do not use cout but SerialPrintln(ErrorMsg);
/// TODO: it would be cool if it could tell you in what function the error accourred or the line of text, mabye a macro?
void string_handler::PrintError(const char* ErrMsg){
    cout << "[ ERROR ] " << ErrMsg << endl;
}
#else 
// @brief The error message fucntion
/// @param ErrMsg the string to be printed
/// TODO: When porting this class to arduino do not use cout but SerialPrintln(ErrorMsg);
/// TODO: it would be cool if it could tell you in what function the error accourred or the line of text, mabye a macro?
void string_handler::PrintError(const char* ErrMsg){
    Serial.print("[ ERROR ]");
    Serial.println(ErrMsg);
}
#endif

/// @brief Safely copy a string to another, memcopy sucks
/// @param dest destination string
/// @param source source string
void string_handler::copy_string(char* dest, char* source){
    int len;
    if(dest == NULL || source == NULL){
        PrintError("source string or destination string is a NULL pointer");
        return;
    }
    if((len = find_string_len(source)) ==0 ) return;
    for(int i = 0; i < len; i++){
        dest[i] = source[i];
    }
}

/// @brief finds the lenght of a string utill 'end_char'
/// @param str the string
/// @param end_char terminating character, defaulted to '\0'
/// @return the lenght
int string_handler::find_string_len(const char* str, char end_char){
    const char* endchr = NULL;
    if(str ==  NULL) {
        PrintError("'str' buffer is NULL");
        return 0;
    }
    endchr = strchr(str, end_char);
    if((endchr = strchr(str, end_char)) == NULL){
        PrintError("could not find end_char in string str");
        return 0;
    }
    return (int)(endchr - str);
}
