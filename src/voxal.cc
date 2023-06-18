#include <iostream>

#include "voxal.h"

using namespace std;

int main(int argc, char **argv) {
    if(argc < 2) {
        return 1;
    }

    string path = string(argv[1]);
    Lexer lexer(path);

    lexer.tokenize();

#ifdef __DEBUG__    
    string types[] = { "LEFT_PAR", "RIGHT_PAR", "IDENT", "STRING", "CONST", "INVALID" };

    for(Token i = lexer.next_token(); i != Token(); i = lexer.next_token()) {
        cout << i.strrep << " " << types[i.type] << endl;

    }
#endif

    return 0;
}