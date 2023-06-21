#ifndef __RUNTIME_H__
#define __RUNTIME_H__

#include <iostream>
#include <unordered_map>
#include <functional>
#include <string>

#include "parser.h"

enum RuntimeSymbolType {
    SYMBOL_FUNCTION,
    SYMBOL_BUILTIN,
    SYMBOL_NONE
};

struct RuntimeSymbol;

using SymbolTable = std::unordered_map<std::string, RuntimeSymbol>;
using VoxalBuiltin = std::function<VoxalValue*(std::vector<VoxalValue*>, SymbolTable)>;

struct RuntimeSymbol {
    RuntimeSymbolType type;

    std::string ident;
    std::vector<VoxalValue*> params;

    VoxalValue *body;
    VoxalBuiltin builtin_body;

    RuntimeSymbol();
    RuntimeSymbol(std::string name, VoxalBuiltin builtin);
    RuntimeSymbol(VoxalValueFunc *function);
    ~RuntimeSymbol();
};

class Runtime {
private:
    VoxalProgram program;
    SymbolTable symbol_table;

    std::vector<VoxalValueFunc*> calls;

public:
    Runtime(VoxalProgram prog);

    void run();
};

VoxalBuiltin create_op_builtin(int argc, std::function<double(std::vector<double>)> callback);
VoxalValue *call_function(VoxalValueFunc *function, SymbolTable context);
void define_builtins(SymbolTable& symtable);

#endif //__RUNTIME_H__