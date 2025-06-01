#include "string handler.h"



string_handler::string_handler(int r, int c){
    Init(r,c);
    
}

string_handler::~string_handler()
{
}

void string_handler::Init(int r, int c){
    rows = r;
    cols = c;
    clear_buffers();
}

void string_handler::clear_buffers(){
    current_pos = 0;
    text_len = 0;
    memset(text, 0, MAX_STRING);
    memset(buffer, 0, MAX_STRING);
}


void string_handler::SetText(char* in){
    if(in == NULL){
        PrintError("'in' buffer pointer is NULL");
        return;
    }
    text_len = find_string_len(in);
    copy_string(text, in);
    copy_string(buffer, in);
}



void string_handler::Get_print_token(char* tkn){
    int row_ptr = 0;
    char* end_word = 0;
    int word_len = 0;
    char* buff = buffer;
    vector<char> final_buffer = {0};
    int try_count = 0;
    while(try_count < 15){
        if((end_word = strchr(buff, ' ')) == NULL){
            if(row_ptr + (text_len - current_pos) > cols){
                if(current_row <= rows-1){
                final_buffer.push_back('\n');
                current_row++;
                row_ptr = 0;
                }
                else {
                    break;
                }
            }
            apend_string_to_vector(final_buffer, buff, text_len-current_pos);
            break;
        }
        word_len = (end_word - buff)+1;
    printf("====================================\n");
    printf("world len is: %d,\ncurrent buffer: %s\n", word_len, buff);
    printf("current_ptr = %d,\ncols = %d\n", row_ptr, cols);
    printf("====================================\n");
        if(row_ptr + word_len > cols){
            if(current_row < rows-1){
                final_buffer.push_back('\n');
                current_row++;
                row_ptr = 0;
                    printf("====================================\n");
                    printf("end of row!, row is %d / %d\n", current_row, rows);
                    printf("====================================\n");
            }
            else{
                    printf("====================================\n");
                    printf("OUT\n");
                    printf("====================================\n");
                break;
            }
        }
        apend_string_to_vector(final_buffer, buff, word_len);
        buff += word_len;
        row_ptr += word_len;
        current_pos += word_len;
        try_count++;
    }
    final_buffer.push_back('\0');
    convert_char_vector_to_string(tkn, final_buffer);
    ///BUG: final string is generated but cannot be printed
    printf("====================================\n");
    printf("return buffer : %s\n", tkn);
    printf("====================================\n");
}




void string_handler::PrintError(const char* ErrMsg){
    ///TODO: add SerialPrintl(...)
    cout << "[ ERROR ] " << ErrMsg << endl;
}

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
    printf("====================================\n");
    printf("source = %s,\n dest = %s\n", source, dest);
    printf("====================================\n");
}

int string_handler::find_string_len(char* str, char end_char){
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
    printf("====================================\n");
    printf("%s len is %ld\n", str, endchr - str);
    printf("====================================\n");
    return (int)(endchr - str);
}


void string_handler::apend_string_to_vector(vector<char> &final_bffer, const char* str, int len){
    for(int i = 0; i < len; i++){
        final_bffer.push_back(str[i]);
    }
}

void string_handler::convert_char_vector_to_string(char* buff, vector<char> src){
    printf("====================================\n");
    printf("start conversion of vectro\n");
    printf("====================================\n");
    int len = src.size();
    for(int i = 0; i < len; i ++){
        printf("%c = %c\n", buff[i] , src[i]);
        buff[i] = src[i];
    }
}

