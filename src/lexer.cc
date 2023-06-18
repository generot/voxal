#include "lexer.h"

using namespace std;

Token::Token() {
    numrep = 0.0;
    strrep = "";
    type = TK_INVALID;
}

Token::Token(string rep) {
    regex ident_regex("[^\"'()\\s]+");
    regex num_regex("\\d+(\\.\\d+)?");
    regex str_regex("\".*\"");

    if(rep == "(")      type = TK_LEFTPAR;
    else if(rep == ")") type = TK_RIGHTPAR;
    else if(regex_search(rep, ident_regex)) {
        if(regex_search(rep, num_regex)) {
            type = TK_LITERAL_CONST;

            numrep = stod(rep);
        } else {
            type = TK_IDENT;
        }
    } 
    else if(regex_search(rep, str_regex)) {
        type = TK_LITERAL_STRING;
    } else {
        type = TK_INVALID;
    }

    strrep = rep;
}

bool Token::operator==(const Token& other) {
    return (
        strrep == other.strrep &&
        type == other.type
    );
}

bool Token::operator!=(const Token& other) {
    return !(*this == other);
}

Lexer::Lexer(string filepath) {
    sourcefile.open(filepath);

    iter = TokenIterator();
    tokens = vector<Token>();
}

char *Lexer::read_sourcefile(int *len) {
    int buflen = 0;

    sourcefile.seekg(0, sourcefile.end);
    buflen = sourcefile.tellg();
    sourcefile.seekg(0, sourcefile.beg);

    char *buf = new char[buflen]{0};

    sourcefile.read(buf, buflen);

    *len = buflen;

    return buf;
}

void Lexer::tokenize() {
    regex lexing_rx("[()]|[^\"'()\\s]+|(\".*\")", 
        regex_constants::ECMAScript);

    char *buf = NULL;
    int buflen = 0;

    buf = read_sourcefile(&buflen);

    cregex_iterator it = cregex_iterator(buf, buf + buflen, lexing_rx);
    cregex_iterator end = cregex_iterator();

    for(cregex_iterator i = it; i != end; i++) {
        cmatch m = *i;
        string curr_str = m.str(0);

        Token curr_token = Token(curr_str);

        tokens.push_back(move(curr_token));
    }

    delete[] buf;
}

Token Lexer::next_token(int lookahead) {
    if(iter == TokenIterator()) {
        iter = tokens.begin();
    }

    if(lookahead != 0) {
        return *(iter + lookahead);
    }

    if(iter == tokens.end()) {
        return Token();
    }

    return *iter++;
}

Token Lexer::prev_token() {
    return *iter--;
}

Lexer::~Lexer() {
    sourcefile.close();
    tokens.clear();
}