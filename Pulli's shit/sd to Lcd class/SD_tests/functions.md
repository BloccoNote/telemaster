bool init_sd_read(const char* filename){
  if(filename == NULL) return false;
  if(!SD.begin(CS_SD_PIN)){
    Serial.println("[ ERROR ] connection failed with SD card");
    return false;
  }
  Serial.println("[ INFO ] SD connection successfully established");
	fileptr = SD.open(filename, FILE_READ);
  return true;
}

/// @brief reads a line form the sd card
/// @return false if EOF
bool read_line_sd(){
	if(buff != NULL){
		delete buff; //if buff pointer is not initialized with null it crashes here
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
		PrintMsg(Error, "end of line should be higher than start of line");
    stall();
	}

	buff_len = end_position - starting_position;
	buff = new char [buff_len];

	if(!fileptr.seek(starting_position)){
		PrintMsg(Error, "error in seek() method");
		stall();
	}

	fileptr.read(buff, buff_len);
	buff[buff_len-1] = '\0';
	
	return return_val;
}

/// @brief converts all characters up to the first appearance of the 'separator' character to integers
/// @param separator separates integers from chars
/// @return the value
int find_index(char separator){
	int index;
	char* p;
	int value = 0;
	if(!(p = strchr(buff, separator))){
		PrintMsg(Error, "seprator character is not in the buffer");
		stall();
	}
	index = p - buff -1;
	for(int i = 0; i <= index ; i++){
		switch(buff[i]){
			case '0':
			break;
			default:
				value *= 10;
				value += (buff[i] - '0');
			break;
		}
	}
	return value;
}

int find_and_remove_index(char separator){
	int index;
	char* p = NULL;
	int value = 0;
	int i = 0;
	if(!(p = strchr(buff, separator))){
		PrintMsg(Error, "seprator character is not in the buffer");
		stall();
	}
	index = p - buff -1;
	for(; i <= index ; i++){
		switch(buff[i]){
			case '0':
			break;
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
	memcpy(buff, p+index, buff_len);
	delete p;
	p = NULL;
	return value;
}


void stall(){
  Serial.println("[ ERROR ] program is stalled");
  while(1);
}


void PrintMsg(Rep msgtype, const char* msg){
	const char* Types[]= {
		"[ ERROR ] ",
		"[ INFO ] ",
		"[ WARNING] "
	};
	if(!Serial) return;
	Serial.print(Types[msgtype]);
	Serial.println(msg);
}
