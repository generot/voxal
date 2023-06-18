#ifndef __TREE_H__
#define __TREE_H__

#include <vector>
#include <string>

enum VoxalValueType {
    VX_LITERAL_CONST,
    VX_LITERAL_STR,
    VX_FUNC_REF,
    VX_FUNCTION,
    VX_INVALID
};

enum VoxalFunctionType {
    VX_FUNC_DEFINITION,
    VX_FUNC_CALL
};

struct VoxalNodeTrait {};

struct VoxalValue : VoxalNodeTrait {
    virtual VoxalValueType reportType() {
        return VX_INVALID;
    }
};

struct VoxalFunction : VoxalNodeTrait {
    VoxalFunctionType type = VX_FUNC_CALL;

    std::string ident;
    std::vector<VoxalValue> params;
};

struct VoxalProgram : VoxalNodeTrait {
    std::vector<VoxalFunction> calls;
};

struct VoxalValueConst : VoxalValue {
    double const_val;

    VoxalValueConst(double cval) : VoxalValue(), const_val(cval) {}

    VoxalValueType reportType() override {
        return VX_LITERAL_CONST;
    }
};

struct VoxalValueString : VoxalValue {
    std::string str_val;

    VoxalValueString(std::string sval) : VoxalValue(), str_val(sval) {}

    VoxalValueType reportType() override {
        return VX_LITERAL_STR;
    }
};

struct VoxalValueRef : VoxalValueString {
    VoxalValueRef(std::string sval) : VoxalValueString(sval) {}

    VoxalValueType reportType() override {
        return VX_FUNC_REF;
    }
};

struct VoxalValueCall : VoxalValue {
    VoxalFunction call;

    VoxalValueCall(VoxalFunction cl) : VoxalValue(), call(cl) {}

    VoxalValueType reportType() override {
        return VX_FUNCTION;
    }
};

#endif //__TREE_H__