#include "parser.h"
#include "tree.h"

#include <sstream>
#include <stdexcept>

using namespace std;

Parser::Parser(Lexer *const lx_ref) { lx = lx_ref; }

bool Parser::expect(TokenType expected) {
    return lx->next_token(0).type == expected;
}

Token Parser::accept() {
    return lx->next_token();
}

VoxalFunction Parser::parse_function() {
    VoxalFunction func;

    if(expect(TK_LEFTPAR)) {
        accept();
        if(expect(TK_IDENT)) {
            Token ident = accept();

            if(ident.strrep == "define") {
                func.type = VX_FTYPE_DEFINITION;
                ident = accept();
            }

            func.ident = ident.strrep;

            while(!expect(TK_RIGHTPAR)) {
                VoxalValue *param = parse_value();

                func.params.push_back(param);
            }

            Token right_par = accept();

            if(func.type == VX_FTYPE_DEFINITION && 
            func.params.back()->reportType() != VX_VTYPE_FUNCTION) {
                //REPORT ERROR    
            }
        }
    }

    return func;
}

VoxalValue *Parser::parse_value() {
    if(expect(TK_IDENT)) {
        return new VoxalValueRef(accept().strrep);
    }

    if(expect(TK_LITERAL_STRING)) {
        return new VoxalValueString(accept().strrep);
    }

    if(expect(TK_LITERAL_CONST)) {
        return new VoxalValueConst(accept().numrep);
    }

    if(expect(TK_LEFTPAR)) {
        return new VoxalValueFunc(parse_function());
    }

    return new VoxalValue();
}

/*
    Rewrite this shit to properly produce expressions (VoxalValue), instead of functions only!
    The current setup is pretty much useless...
*/
VoxalProgram Parser::parse_program() {
    VoxalProgram program;

    while(lx->has_tokens()) {
        VoxalValue *statement = parse_value();

        program.statements.push_back(statement);
    }

    return program;
}