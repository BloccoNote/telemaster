#include <iostream>
#include "string handler.h"


int main(){
    string_handler S;
    char* tests = "Hello world!, i want to try this new fantastic program\n";
    S.SetText(tests);
    char tokens[MAX_STRING];
    S.Get_print_token(tokens);
    cout << "result strings are: \n" << tokens << endl;
    return 0;
}