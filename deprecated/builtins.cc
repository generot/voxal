// #define DEF_BUILTIN(id, func) {                  \
//     sym.body.ident = (id);                       \
//     sym.builtin_body = (func);                   \
//     symtable[(id)] = RuntimeSymbol(sym);         \
// }

// VoxalFunctionModel voxalize_binary_op(function<int(int, int)> op) {
//     return [=](VoxalParams p) {
//         VoxalValueConst *rt = new VoxalValueConst(0.0);

//         if(p[0]->reportType() != VX_LITERAL_CONST) {

//         }
//     }
// }

// void define_builtins(SymbolTable& symtable) {
//     RuntimeSymbol sym;
//     sym.type = SYMBOL_BUILTIN;

//     DEF_BUILTIN("+", ([=](VoxalParams p) { return a + b; }));
//     DEF_BUILTIN("-", ([=](int a, int b) { return a - b; }));
//     DEF_BUILTIN("*", ([=](int a, int b) { return a * b; }));
//     DEF_BUILTIN("/", ([=](int a, int b) { return a / b; }));
//     DEF_BUILTIN("<", ([=](int a, int b) { return a < b; }));
//     DEF_BUILTIN(">", ([=](int a, int b) { return a > b; }));
//     DEF_BUILTIN("<=", ([=](int a, int b) { return a <= b; }));
//     DEF_BUILTIN(">=", ([=](int a, int b) { return a >= b; }));
//     DEF_BUILTIN("&&", ([=](int a, int b) { return a && b; }));
//     DEF_BUILTIN("||", ([=](int a, int b) { return a || b; }));
//     DEF_BUILTIN("!", ([=](int a, int b) { return !a; }));
//     //DEF_BUILTIN("if", ([=](int a, int b) { return a || b; }));
// }

// #undef DEF_BUILTIN