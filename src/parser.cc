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

VoxalValue Parser::parse_value() {
    if(expect(TK_IDENT)) {
        return VoxalValueRef(accept().strrep);
    }

    if(expect(TK_LITERAL_STRING)) {
        return VoxalValueString(accept().strrep);
    }

    if(expect(TK_LITERAL_CONST)) {
        return VoxalValueConst(accept().numrep);
    }

    if(expect(TK_LEFTPAR)) {
        return VoxalValueCall(parse_function());
    }

    return VoxalValue();
}

VoxalFunction Parser::parse_function() {
    VoxalFunction func;

    if(expect(TK_LEFTPAR)) {
        accept();
        if(expect(TK_IDENT)) {
            Token ident = accept();

            if(ident.strrep == "define") {
                func.type = VX_FUNC_DEFINITION;
                ident = accept();
            }

            func.ident = ident.strrep;

            while(!expect(TK_RIGHTPAR)) {
                VoxalValue param = parse_value();

                func.params.push_back(param);
            }
        }
    }

    return func;
}

VoxalProgram Parser::parse_program() {
    VoxalProgram program;

    while(lx->has_tokens()) {
        VoxalFunction call = parse_function();

        program.calls.push_back(call);
    }

    return program;
}