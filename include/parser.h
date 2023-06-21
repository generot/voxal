#ifndef __PARSER_H__
#define __PARSER_H__

#include "tree.h"
#include "lexer.h"

class Parser {
protected:
    Lexer *lx;

    bool expect(TokenType expected);
    Token accept();

    VoxalValue *parse_value();
    VoxalFunction parse_function();
public:
    Parser(Lexer *const lx_ref);

    VoxalProgram parse_program();
};

#endif //__PARSER_H__