#include "Sd_tokenizer.h"

Sd_tokenizer tokenizer(10, "test.txt", 2, 16);

void setup(){
  Serial.begin(9600);
  while(!Serial);

}



void loop(){
  char tokens[16*2+4] = {0};
  int indx = rand()%10;
  tokenizer.find_sd_line(indx);
  tokenizer.get_string_to_print(tokens);
  Serial.println(tokens);
}