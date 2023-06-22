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
        throw runtime_error("RuntimeSymbol: Function definition expected.");
    }

    auto &fparams = function->func.params;

    if (fparams.size() > 1) {
        params = vector<VoxalValue*>(fparams.begin(), fparams.end() - 1);
    }

    type = SYMBOL_FUNCTION;

    ident = function->func.ident;
    body = function->func.params.back();
}

RuntimeSymbol::RuntimeSymbol(string name, VoxalValue *val) {
    type = SYMBOL_DETERMINABLE;

    ident = name;
    body = val;
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
        VoxalValue *res = eval_value(call, symbol_table);

        print_voxal_value(res);
    }
}

void print_voxal_value(VoxalValue *val) {
    //cout << "Type: " << (int)val->reportType() << endl;

    switch(val->reportType()) {
    case VX_VTYPE_LCONST:
        cout << ((VoxalValueConst*)val)->const_val << endl;
        break;
    case VX_VTYPE_LSTR:
        cout << ((VoxalValueString*)val)->str_val << endl;
        break;
    case VX_VTYPE_REF:
        cout << "<Reference: '" << ((VoxalValueRef*)val)->str_val << "'>" << endl;
        break;
    case VX_VTYPE_FUNCTION:
        cout << "<Function: '" << ((VoxalValueFunc*)val)->func.ident << "'>" << endl;
        break;
    }
}

void define_builtins(SymbolTable& symtable) {
    unordered_map<string, VoxalBuiltin> builtins = {
        { "+",  create_op_builtin(2, [=](vector<double> args) { return args[0] + args[1]; }) },
        { "-",  create_op_builtin(2, [=](vector<double> args) { return args[0] - args[1]; }) },
        { "*",  create_op_builtin(2, [=](vector<double> args) { return args[0] * args[1]; }) },
        { "/",  create_op_builtin(2, [=](vector<double> args) { return args[0] / args[1]; }) },
        { "<",  create_op_builtin(2, [=](vector<double> args) { return args[0] < args[1]; }) },
        { ">",  create_op_builtin(2, [=](vector<double> args) { return args[0] > args[1]; }) },
        { "<=", create_op_builtin(2, [=](vector<double> args) { return args[0] <= args[1]; }) },
        { ">=", create_op_builtin(2, [=](vector<double> args) { return args[0] >= args[1]; }) },
        { "!=", create_op_builtin(2, [=](vector<double> args) { return args[0] != args[1]; }) },
        { "==", create_op_builtin(2, [=](vector<double> args) { return args[0] == args[1]; }) },
        { "&&", create_op_builtin(2, [=](vector<double> args) { return args[0] && args[1]; }) },
        { "||", create_op_builtin(2, [=](vector<double> args) { return args[0] || args[1]; }) },

        { "if", [=](vector<VoxalValue*> args, SymbolTable context) -> VoxalValue* {
            if(args.size() != 3) {
                return NULL;
            }

            if(args[0]->reportType() != VX_VTYPE_FUNCTION) {
                return NULL;
            }

            VoxalValue *res = eval_value((VoxalValueFunc*)args[0], context);
            VoxalValue *ret_val = NULL;

            if(res->reportType() != VX_VTYPE_LCONST) {
                return NULL;
            }

            VoxalValueConst *cnst = (VoxalValueConst*)res;

            if(cnst->const_val) {
                ret_val = args[1];
            } else {
                ret_val = args[2];
            }

            return eval_value(ret_val, context);
        }}
    };

    for(pair<string, VoxalBuiltin> tuple : builtins) {
        symtable[tuple.first] = RuntimeSymbol(tuple.first, VoxalBuiltin(tuple.second));
    }
}

VoxalValue *eval_value(VoxalValue *value, SymbolTable context) {
    switch(value->reportType()) {
    case VX_VTYPE_LCONST:
    case VX_VTYPE_LSTR:
        return value;
    case VX_VTYPE_FUNCTION:
        return call_function((VoxalValueFunc*)value, context);
    case VX_VTYPE_REF:
        return context[((VoxalValueRef*)value)->str_val].body;
    }

    return NULL;
}


VoxalValue *call_function(VoxalValueFunc *function, SymbolTable context) {
    RuntimeSymbol sym = context[function->func.ident];

    if(sym.type == SYMBOL_DETERMINABLE) {
        return sym.body;
    }

    if(sym.type == SYMBOL_BUILTIN) {
        return sym.builtin_body(function->func.params, context);
    } else if(sym.type == SYMBOL_FUNCTION) {
        if(sym.body->reportType() == VX_VTYPE_LSTR || sym.body->reportType() == VX_VTYPE_LCONST) {
            return sym.body;
        }

        if(sym.body->reportType() == VX_VTYPE_REF) {
            return context[((VoxalValueRef*)sym.body)->str_val].body;
        }

        if(function->func.params.size() != sym.params.size()) {
            return NULL;
        }

        for(size_t i = 0; i < sym.params.size(); i++) {
            VoxalValue *p = sym.params[i];
            VoxalValue *val = function->func.params[i];

            if(p->reportType() != VX_VTYPE_REF) {
                return NULL;
            }

            VoxalValueRef *ref = (VoxalValueRef*)p;
            VoxalValue *resolved = eval_value(val, context);

            context[ref->str_val] = RuntimeSymbol(ref->str_val, resolved);
        }

        return call_function((VoxalValueFunc*)sym.body, context);
    }

    return NULL;
}

VoxalBuiltin create_op_builtin(int argc, function<double(vector<double>)> callback) {
    VoxalBuiltin new_builtin = [=](vector<VoxalValue*> args, SymbolTable context) -> VoxalValue* {
        if(args.size() != argc) {
            return NULL;
        }

        vector<double> params;

        VoxalValueConst *cnst = NULL;

        for(VoxalValue *i : args) {
            VoxalValue *resolved = eval_value(i, context);

            if(resolved->reportType() == VX_VTYPE_REF) {
                resolved = eval_value(context[((VoxalValueRef*)i)->str_val].body, context);
            }

            if(resolved->reportType() != VX_VTYPE_LCONST) {
                return NULL;
            }

            cnst = (VoxalValueConst*)resolved;

            if(!cnst) {
                return NULL;
            }

            params.push_back(cnst->const_val);
        }

        double res = callback(params);

        return new VoxalValueConst(res);
    };

    return new_builtin;
}