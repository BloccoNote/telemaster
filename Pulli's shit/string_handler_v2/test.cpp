#include <iostream>
#include "string handler.h"
#include <unistd.h>

void print_all_text(string_handler &s){
    char* tokens[2];
    tokens[0] = new char[17];
    tokens[1] = new char[17];
    bool not_done = true;
    do{
        not_done = s.Get_print_token(tokens);
        printf("\e[H\e[2J");
        cout << "0123456789ABCDEF" << endl;
        cout << "----------------\n" << tokens[0] << endl << tokens[1] << "\n----------------" << endl;
        sleep(3);
    }while(not_done);
    delete tokens[0];
    delete tokens[1];
}

int main(){
    string_handler S;
    char* tests = "Hello world!, this example show you what this class can do to strings of max lenght 1024";
    char* test_2 = new char[MAX_STRING];
    char* tok[2];
    tok[0] = new char[17];
    tok[1] = new char[17];

    
    //testing while loop capabilities
    S.SetText(tests);
    print_all_text(S);
    

    // testing non constant string input
    cout << "insert text here: ";
    scanf("%[^\n]s", test_2);
    getchar();
    S.SetText(test_2);
    print_all_text(S);

    //testing weird cases;
    S.Init();
    S.Get_print_token(tok);
    strcmp(tok[0], "\0") != 0 ? printf("X\n") : printf("OK\n");

    S.SetText(". . . . . . . assert                       Too many spaces!!");
    print_all_text(S);

    S.Init(-1,-1);
    delete test_2;
    delete tok[0];
    delete tok[1];


    return 0;
}