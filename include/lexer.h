#ifndef __LEXER_H__
#define __LEXER_H__

#include <fstream>
#include <vector>
#include <string>
#include <regex>

struct Token;

using TokenIterator = std::vector<Token>::iterator;

enum TokenType {
    TK_LEFTPAR,
    TK_RIGHTPAR,
    TK_IDENT,
    TK_LITERAL_STRING,
    TK_LITERAL_CONST,
    TK_INVALID
};

struct Token {
    TokenType type;

    std::string strrep;
    double numrep;

    Token();
    Token(std::string rep);

    bool operator==(const Token& other);
    bool operator!=(const Token& other);
};

class Lexer {
private:
    std::ifstream sourcefile;
    std::vector<Token> tokens;

    TokenIterator iter;

    char *read_sourcefile(int *len);
public:
    Lexer() {}
    Lexer(std::string filepath);

    void tokenize();

    bool has_tokens();

    Token next_token(int peek = -1);
    Token prev_token();

    ~Lexer();
};

#endif //__LEXER_H__