#ifndef __TREE_H__
#define __TREE_H__

#include <vector>
#include <string>

enum VoxalValueType {
    VX_VTYPE_LCONST,
    VX_VTYPE_LSTR,
    VX_VTYPE_REF,
    VX_VTYPE_FUNCTION,
    VX_VTYPE_INVALID
};

enum VoxalFunctionType {
    VX_FTYPE_DEFINITION,
    VX_FTYPE_CALL
};

struct VoxalNodeTrait {};

struct VoxalValue : VoxalNodeTrait {
    virtual VoxalValueType reportType() {
        return VX_VTYPE_INVALID;
    }
};

struct VoxalFunction : VoxalNodeTrait {
    VoxalFunctionType type = VX_FTYPE_CALL;

    std::string ident;
    std::vector<VoxalValue*> params;
};

struct VoxalProgram : VoxalNodeTrait {
    std::vector<VoxalValue*> statements;
};

struct VoxalValueConst : VoxalValue {
    double const_val;

    VoxalValueConst(double cval) : VoxalValue(), const_val(cval) {}

    VoxalValueType reportType() override {
        return VX_VTYPE_LCONST;
    }
};

struct VoxalValueString : VoxalValue {
    std::string str_val;

    VoxalValueString(std::string sval) : VoxalValue(), str_val(sval) {}

    VoxalValueType reportType() override {
        return VX_VTYPE_LSTR;
    }
};

struct VoxalValueRef : VoxalValueString {
    VoxalValueRef(std::string sval) : VoxalValueString(sval) {}

    VoxalValueType reportType() override {
        return VX_VTYPE_REF;
    }
};

struct VoxalValueFunc : VoxalValue {
    VoxalFunction func;

    VoxalValueFunc(VoxalFunction cl) : VoxalValue(), func(cl) {}

    VoxalValueType reportType() override {
        return VX_VTYPE_FUNCTION;
    }
};

#endif //__TREE_H__