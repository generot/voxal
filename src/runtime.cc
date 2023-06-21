#include "runtime.h"

using namespace std;

RuntimeSymbol::RuntimeSymbol() {
    type = SYMBOL_NONE,
    body = NULL;
}

RuntimeSymbol::~RuntimeSymbol() {}

RuntimeSymbol::RuntimeSymbol(string name, VoxalBuiltin builtin) {
    type = SYMBOL_BUILTIN;

    builtin_body = builtin;
    ident = name;
}

RuntimeSymbol::RuntimeSymbol(VoxalValueFunc *function) {
    if (function->func.type != VX_FTYPE_DEFINITION) {
        throw std::runtime_error("RuntimeSymbol: Function definition expected.");
    }

    auto &fparams = function->func.params;

    if (fparams.size() > 1) {
        params = std::vector<VoxalValue *>(fparams.begin(), fparams.end() - 1);
    }

    type = SYMBOL_FUNCTION;

    ident = function->func.ident;
    body = function->func.params.back();
}

Runtime::Runtime(VoxalProgram prog) {
    program = prog;
    define_builtins(symbol_table);

    for(VoxalValue *statement : prog.statements) {
        if(statement->reportType() == VX_VTYPE_FUNCTION) {
            VoxalValueFunc *function = (VoxalValueFunc*)statement;

            if(function->func.type == VX_FTYPE_DEFINITION) {
                symbol_table[function->func.ident] = RuntimeSymbol(function);
            } else {
                calls.push_back(function);
            }
        }
    }
}

void Runtime::run() {
    for(VoxalValueFunc *call : calls) {
        VoxalValueConst *res = (VoxalValueConst*)call_function(call, symbol_table);

        cout << res->const_val << endl;
    }
}

void define_builtins(SymbolTable& symtable) {
    unordered_map<string, VoxalBuiltin> builtins = {
        { "+", create_op_builtin(2, [=](vector<double> args) { return args[0] + args[1]; }) },
        { "-", create_op_builtin(2, [=](vector<double> args) { return args[0] - args[1]; }) },
        { "*", create_op_builtin(2, [=](vector<double> args) { return args[0] * args[1]; }) },
        { "/", create_op_builtin(2, [=](vector<double> args) { return args[0] / args[1]; }) },
        { "<", create_op_builtin(2, [=](vector<double> args) { return args[0] < args[1]; }) },
        { ">", create_op_builtin(2, [=](vector<double> args) { return args[0] > args[1]; }) },
        { "<=", create_op_builtin(2, [=](vector<double> args) { return args[0] <= args[1]; }) },
        { ">=", create_op_builtin(2, [=](vector<double> args) { return args[0] >= args[1]; }) },
        { "!=", create_op_builtin(2, [=](vector<double> args) { return args[0] != args[1]; }) },
        { "==", create_op_builtin(2, [=](vector<double> args) { return args[0] == args[1]; }) },
        { "&&", create_op_builtin(2, [=](vector<double> args) { return args[0] && args[1]; }) },
        { "||", create_op_builtin(2, [=](vector<double> args) { return args[0] || args[1]; }) },
    };

    for(pair<string, VoxalBuiltin> tuple : builtins) {
        symtable[tuple.first] = RuntimeSymbol(tuple.first, VoxalBuiltin(tuple.second));
    }
}

VoxalValue *call_function(VoxalValueFunc *function, SymbolTable context) {
    RuntimeSymbol sym = context[function->func.ident];

    if(sym.type == SYMBOL_BUILTIN) {
        return sym.builtin_body(function->func.params, context);
    }

    return NULL;
}

VoxalBuiltin create_op_builtin(int argc, function<double(vector<double>)> callback) {
    VoxalBuiltin new_builtin = [=](vector<VoxalValue*> args, SymbolTable context) -> VoxalValue* {
        if(args.size() != argc) {
            return NULL;
        }

        vector<double> params;

        VoxalValue *func_ret = NULL;
        VoxalValueConst *cnst = NULL;

        for(VoxalValue *i : args) {
            switch(i->reportType()) {
            case VX_VTYPE_LCONST:
                cnst = (VoxalValueConst*)i;
                break;
            case VX_VTYPE_FUNCTION:
                func_ret = call_function((VoxalValueFunc*)i, context);

                if(func_ret->reportType() == VX_VTYPE_LCONST) {
                    cnst = (VoxalValueConst*)func_ret;
                }

                break;
            }

            params.push_back(cnst->const_val);
        }

        double res = callback(params);

        return new VoxalValueConst(res);
    };

    return new_builtin;
}