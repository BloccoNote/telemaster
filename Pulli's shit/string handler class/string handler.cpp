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
}

/// @brief Fills the buffer 'tkn' with tokens of 'buffer' so that can fit a display of rows * cols dimentions
/// @param tkn PREALLOCATED buffer of minimum dimentions rows*cols + rows
/// @return true if it tokenized last element of buffer
/// @example if the display is a LCD_liquid display (16*2) and text is: \n
/// "Hello world! See you in the next print\0"                          \n
/// the result would be:                                                \n
/// "Hello world! \nSee you in the \0"                                  \n
///                ^                ^                                   \n
///                13               15                                  \n
/// next time you call this function will continue the phrase           \n
/// @note as you can see from the example in 'tkn' buffer you have to account extra characters '\n' and '\0'
bool string_handler::Get_print_token(char* tkn){
    //variable definition
    int row_ptr = 0;
    int word_len = 0;
    int try_count = 0;
    char* end_word = 0;
    char* buff = buffer + current_pos;
    current_row = 0;
    vector<char> final_buffer = {0};

    while(try_count <= MAX_TRY){
        // calcolates world len based on the spaces
        if((end_word = strchr(buff, ' ')) == NULL){

            if(row_ptr + (text_len - current_pos) > cols){
                if(current_row < rows-1){
                    final_buffer.push_back('\n');
                    current_row++;
                    row_ptr = 0;
                }
                else {
                    break;
                }
            }

            int final_len = text_len-current_pos;
            apend_string_to_vector(final_buffer, buff, final_len);
            current_pos += final_len;
            break;

        }

        word_len = (end_word - buff) + 1; // the +1 include the space
        if(word_len > cols){
            PrintError("in buff there is a word longer than 'cols'");
            return true;
        }

        if(row_ptr + word_len > cols){

            if(current_row < rows-1){
                final_buffer.push_back('\n');
                current_row++;
                row_ptr = 0;
            }
            else{
                break;
            }
        }

        apend_string_to_vector(final_buffer, buff, word_len);
        buff += word_len;
        row_ptr += word_len;
        current_pos += word_len;
        try_count++;

    }
    if(try_count == MAX_TRY){
        PrintError("Max try reached");
        return true;
    }
    final_buffer.push_back('\0');
    convert_char_vector_to_string(tkn, final_buffer);
    if(current_pos >= text_len) return true;
    return false;
}


/// @brief The error message fucntion
/// @param ErrMsg the string to be printed
/// TODO: When porting this class to arduino do not use cout but SerialPrintln(ErrorMsg);
/// TODO: it would be cool if it could tell you in what function the error accourred or the line of text, mabye a macro?
void string_handler::PrintError(const char* ErrMsg){
    cout << "[ ERROR ] " << ErrMsg << endl;
}

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
    char* endchr = NULL;
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

/// @brief apend 'string' of length 'len' to char vector 'final_buffer'
/// @param final_buffer vector of char elements
/// @param str 
/// @param len 
void string_handler::apend_string_to_vector(vector<char> &final_buffer, char* str, int len){
    int i = 0;
    if(final_buffer.size() == 1){
        //first element of string was the \0 char (end string)
        final_buffer[0] = str[0];
        i++;
    }
    for(i; i < len; i++){
        final_buffer.push_back(str[i]);
    }
}

/// @brief converts a vector of char into a string
/// @param buff the reciving string
/// @param src vector of char
void string_handler::convert_char_vector_to_string(char* buff, vector<char> src){

    int len = src.size();
    int buff_len = find_string_len(buff);
    if(buff_len < len){
        PrintError("buffer given is too short");
    }
    for(int i = 0; i < len; i ++){
        buff[i] = src[i];
    }
}

