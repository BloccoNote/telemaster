#include "sd_to_lcd.h"

///@name CONSTRUCTORS AND DESTRUCTORS IMPLEMENTATIONS

/// @brief Constructor
/// @param r lcd rows (default 2)
/// @param c lcd cols (default 16)
sd2Lcd::sd2Lcd(int r, int c){
  Init(r,c);
}

/// @brief copy constructor
/// @param X 
sd2Lcd::sd2Lcd(const sd2Lcd &X){
  Init(X);
}

/// @brief destructor
sd2Lcd::~sd2Lcd(){
  Init(0,0);
}

/// @name INITS IMPLEMENTATIONS

/// @brief Initialize object
/// @param r lcd rows (default 2)
/// @param c lcd cols (default 16)
void sd2Lcd::Init(int r, int c){
  if(r <= 0 || c <= 0){
    PrintMsg(Error, "r & c >= 0 ?");
    r = 2;
    c = 16;
  }
  is_sd = false;
  rows = r;
  cols = c;
  buff_len = 0; 
  text_len = 0;
  current_pos = 0;
  current_row = 0;
  if(buffer != NULL){
    delete[] buffer;
    buffer = NULL;
  }
  ptr = NULL;
}

/// @brief Initiaize object with another one
/// @param X 
void sd2Lcd::Init(const sd2Lcd &X){
  Init(X.rows, X.cols);
  is_sd = X.is_sd;
  buff_len = X.buff_len;
  text_len = X.text_len;
  current_pos = X.current_pos;
  current_row = X.current_row;
  ptr = X.ptr;
  if(X.buffer != NULL){
    buffer = new char[buff_len];
    memcpy(buffer, X.buffer, buff_len);
  }
}

/// @name SD FUNCTIONS IMPLEMENTATIONS

/// @brief Initialize sd reader
/// @param filename name of file in sd [directory-s/] filename.<extention>
/// @param cs_pin   Arduino pin for sd CS pin
/// @return false if error
bool sd2Lcd::init_sd_read(const char* filename, int cs_pin){
  if(filename == NULL) return false;
  if(!SD.begin(cs_pin)){
    PrintMsg(Error, "X SD");
    return false;
  }
  PrintMsg(Info, "OK SD");
	fileptr = SD.open(filename, FILE_READ);
  fileptr.seek(SEEK_SET);
  is_sd = true;
  return true;
}


/// @brief reads a line form the sd card
/// @param buff Null pointer to char
/// @return false if EOF
/// @note init_sd_read(filename, cs_pin) must be called before
bool sd2Lcd::read_line_sd(char *&buff){
  if(!is_sd){
    PrintMsg(Error,"No SD_init");
    stall();
  }
	if(buff != NULL){
		delete[] buff; //if buff pointer is not initialized with null it crashes here
		buff = NULL;
	}
	
	bool return_val = true;
	unsigned long starting_position = fileptr.position();
	unsigned long end_position = 0;
	char current_char = 0;

	while(current_char != '\n'){
		if(fileptr.available() <= 1){ //<= 1 handle "\n\0" scenarios
			return_val = false;
			break;
		}
		current_char = fileptr.read();
	}

	end_position = fileptr.position() + 1; // include the \n character

	if(starting_position > end_position){
		PrintMsg(Error, "st > end -p");
    stall();
	}

	buff_len = end_position - starting_position;
  if(buff_len > MAX_BUFF_LEN){
    end_position = buff_len;
    buff_len = MAX_BUFF_LEN;
  }
	buff = new char [buff_len];

	if(!fileptr.seek(starting_position)){
		PrintMsg(Error, "error in seek() method");
		stall();
	}

	fileptr.read(buff, buff_len);
	buff[buff_len-1] = '\0';
  if(buff_len == MAX_BUFF_LEN) fileptr.seek(end_position);
	return return_val;
}

/// @brief finds index of a given string
/// @param separator char that separates index field to text field
/// @param buff result of read_line_sd() function
/// @return value in index field
/// @note [ 0 index]< 1 sep_char>[ 2 text]
/// @example                   \n
/// 0001 hello,word!           \n
///  ^  ^     ^                \n
///  0  1     2                \n
int sd2Lcd::find_index(char* buff, const char separator){
  if(buff == NULL){
    PrintMsg(Error, "got NULL buff");
    return -1;
  }
	int index;
	char* p;
	int value = 0;
	if(!(p = strchr(buff, separator))){
		PrintMsg(Error, "sep ch n.f.");
		stall();
	}
	index = p - buff -1;
	for(int i = 0; i <= index ; i++){
		switch(buff[i]){
			default:
				value *= 10;
				value += (buff[i] - '0');
			break;
		}
	}
	return value;
}


/// @brief finds index and removes it from a given string
/// @param separator char that separates index field to text field
/// @param buff result of read_line_sd() function
/// @return value in index field
/// @note [ 0 index]< 1 sep_char>[ 2 text]
/// @example                   \n
/// 0001 hello,word!           \n
///  ^  ^     ^                \n
///  0  1     2                \n
int sd2Lcd::find_and_remove_index(char* &buff, const char separator){
  if(buff == NULL){
    PrintMsg(Error, "got NULL buff");
    return -1;
  }
	int index;
	char* p = NULL;
	int value = 0;
	int i = 0;
	if(!(p = strchr(buff, separator))){
		PrintMsg(Error, "sep ch n.f.");
		stall();
	}
	index = p - buff -1;
	for(; i <= index ; i++){
		switch(buff[i]){
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
	memcpy(buff, p+index+1, buff_len);
	delete[] p;
	p = NULL;
	return value;
}

/// @brief does exactly what the name suggests
/// @param buff NULL pointer only to char
/// @param index desidered index
/// @param separator character that divides index and text, default ' ' (space)
/// @note it fills the buff with desired data stripped of index and set it to be tokenized
/// @return false if error
bool sd2Lcd::find_sd_line_by_index(char* &buff,  int index, const char separator)
{
	int search, indx;
  fileptr.seek(SEEK_SET);
	do{
		search = read_line_sd(buff);
		indx = find_index(buff, separator);
		if(indx == index){
      find_and_remove_index(buff, separator);
			SetText(buff);
      return true;
		}
	}while(search);
  return false;
}

/// @brief rewinds file reading
bool sd2Lcd::rewind_file(){
  return fileptr.seek(SEEK_SET);
}

/// @name LCD STRING HANDLER IMPLEMENTATIONS

/// @brief Set text to be tokenized
/// @param in string of text
/// @note 'in' MUST end with '\0' character
void sd2Lcd::SetText(char* in){
    current_pos = 0;
    current_row = 0;
    text_len = 0;
    if(in == NULL){
        PrintMsg(Error, "in = NULL");
        return;
    }
    //Init(rows, cols);
    text_len = find_string_len(in, '\0');
    if(buffer != NULL){
      delete[] buffer;
      buffer = NULL;
    }
    buffer = new char[text_len+1];
    memset(buffer, 0, text_len+1);
    //copy_string(buffer, in);
    strcpy(buffer, in);
    ptr = buffer;
}

/// @brief finds the lenght of a string utill 'end_char'
/// @param str the string
/// @param end_char terminating character, defaulted to '\0'
/// @return lenght of string
int sd2Lcd::find_string_len(const char* str, char end_char){
    const char* endchr = NULL;
    if(str ==  NULL) {
        PrintMsg(Error, "'str' buffer is NULL");
        return 0;
    }
    endchr = strchr(str, end_char);
    if((endchr = strchr(str, end_char)) == NULL){
        PrintMsg(Error, "could not find end_char");
        return 0;
    }
    return (int)(endchr - str);
}

/// @brief Fills the buffer 'lcd_lines' with tokens of 'buffer' so that can fit a display of rows * cols dimentions
/// @param lcd_lines PREALLOCATED matrix of size rows * (cols + 1)
/// @note must be rows string of len cols + 1
/// @return false if end or error
/// @note the + 1 in columns is for '\0' character
bool sd2Lcd::Get_print_token(char** lcd_lines){
    if(lcd_lines == NULL){
        PrintMsg(Error, "buff = NULL");
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
            PrintMsg(Error, "maximum tries exceeded");
        }
        try_count++;
        end_r = current_row < rows;
    }while((text_len - current_pos ) == 0 || end_r);    
    if(!end_r) current_row = 0;
    return true;
}

/// @brief Clears print status
/// @note resets printing position so that by calling Get_print_token() \name
/// you start form the beginnig;
void sd2Lcd::reset_print_token(){
  ptr = buffer;
  current_pos = 0;
  current_row = 0;
}

/// @name UTILILTY FUNCTIONS IMPLEMENTATIONS

/// @brief prints message
/// @param msgtype 'Error' or 'Info'
/// @param msg message to be printed
void sd2Lcd::PrintMsg(Rep msgtype, const char* msg){
	const char* Types[]= {
		"[ ERROR ] ",
		"[ INFO ] ",
	};
	if(!Serial) return;
	Serial.print(Types[msgtype]);
	Serial.println(msg);
}

/// @brief stalls the program
void sd2Lcd::stall(){
  Serial.println("[ ERROR ] program is stalled");
  while(1);
}

/// @name OVERLOADS

/// @brief overload of operator=
/// @param X 
/// @return object copied
sd2Lcd& sd2Lcd::operator=(const sd2Lcd &X){
  Init(X);
}