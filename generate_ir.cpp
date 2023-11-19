//
// Created by 杨贺森 on 2023/11/16.
//

#include "generate_ir.h"
#include "llvm_ir.h"
#include "utils.h"
#include <fstream>

using namespace std;

extern unordered_map<int, Register> registerList;
extern ofstream fir;
extern int registers;
extern bool ir;
extern bool debug;

void generateIR() {
    init();
    generateCompUnit();
}

void init() {
    if (ir) {
        fir << "declare i32 @getint()          ; 读取一个整数" << endl;
        fir << "declare void @putint(i32)      ; 输出一个整数" << endl;
        fir << "declare void @putch(i32)       ; 输出一个字符" << endl;
        fir << "declare void @putstr(i8*)      ; 输出字符串" << endl;
        fir << endl;
        if (debug) {
            cout << "declare i32 @getint()          ; 读取一个整数" << endl;
            cout << "declare void @putint(i32)      ; 输出一个整数" << endl;
            cout << "declare void @putch(i32)       ; 输出一个字符" << endl;
            cout << "declare void @putstr(i8*)      ; 输出字符串" << endl;
            cout << endl;
        }
    }
}

void printllvm(const string& str) {
    if (ir) {
        fir << str;
        if (debug) {
            cout << str;
        }
    }
}

int calculate(int left, int right, const string& op) {
    if (op == "PLUS") {
        return left + right;
    } else if (op == "MINU") {
        return left - right;
    } else if (op == "MULT") {
        return left * right;
    } else if (op == "DIV") {
        return left / right;
    } else if (op == "MOD") {
        return left % right;
    } else if (op == "AND"){
        return left && right;
    } else if (op == "OR") {
        return left || right;
    } else if (op == "NOT") {
        return !right;
    } else {
        return -1;
    }
}

int compare(int left, int right, const string& op) {
    if (op == "LSS") {
        return left < right;
    } else if (op == "LEQ") {
        return left <= right;
    } else if (op == "GRE") {
        return left > right;
    } else if (op == "GEQ") {
        return left >= right;
    } else {
        return -1;
    }
}

void printllvmCompare(Register& result, const Register& left, const Register& right, const string& op) {
    Type type = Type(0, 0);
    if (left.hasValue && right.hasValue) {
        result.hasValue = true;
        int value = compare(left.value, right.value, op);
        result.value = value;
        result.name = to_string(value);
        return;
    } else {
        if (op == "GRE") {
            string str = "    " + result.name + " = icmp " + "sgt" + " " + typeToString(type) + " " + left.name +", " + right.name + "\n";
            printllvm(str);
        } else if (op == "GEQ") {
            string str = "    " + result.name + " = icmp " + "sge" + " " + typeToString(type) + " " + left.name +", " + right.name + "\n";
            printllvm(str);
        } else if (op == "LSS") {
            string str = "    " + result.name + " = icmp " + "slt" + " " + typeToString(type) + " " + left.name +", " + right.name + "\n";
            printllvm(str);
        } else if (op == "LEQ") {
            string str = "    " + result.name + " = icmp " + "sle" + " " + typeToString(type) + " " + left.name +", " + right.name + "\n";
            printllvm(str);
        } else if (op == "EQL") {
            string str = "    " + result.name + " = icmp " + "eq" + " " + typeToString(type) + " " + left.name +", " + right.name + "\n";
            printllvm(str);
        } else if (op == "NEQ") {
            string str = "    " + result.name + " = icmp " + "ne" + " " + typeToString(type) + " " + left.name +", " + right.name + "\n";
            printllvm(str);
        } else if (op == "NOT") {
            string str = "    " + result.name + " = icmp " + "ne" + " " + typeToString(type) + " " + left.name +", " + right.name + "\n";
            printllvm(str);
        }
    }
}

void printllvmcalculate(Register& result, const Register& left, const Register& right, const string& op) {
    Type type = Type(0,0);
    if (left.hasValue && right.hasValue) {
        result.hasValue = true;
        int value = calculate(left.value, right.value, op);
        result.value = value;
        result.name = to_string(value);
        return;
    } else {
        if (op == "PLUS") {
            string str = "    " + result.name + " = " + "add" + " i32 " + left.name + ", " + right.name + "\n";
            printllvm(str);
        } else if (op == "MINU") {
            string str = "    " + result.name + " = " + "sub" + " i32 " + left.name + ", " + right.name + "\n";
            printllvm(str);
        } else if (op == "MULT") {
            string str = "    " + result.name + " = " + "mul" + " i32 " + left.name + ", " + right.name + "\n";
            printllvm(str);
        } else if (op == "DIV") {
            string str = "    " + result.name + " = " + "sdiv" + " i32 " + left.name + ", " + right.name + "\n";
            printllvm(str);
        } else if (op == "MOD") {
            Register temp1 = getNewRegister(false, 0, false, false, type.id, type.dim);
            string div = "    " + temp1.name + " = " + "sdiv" + " i32 " + left.name + ", " + right.name + "\n";
            printllvm(div);
            Register temp2 = getNewRegister(false, 0, false, false, type.id, type.dim);
            string mul = "    " + temp2.name + " = " + "mul" + " i32 " + temp1.name + ", " + right.name + "\n";
            printllvm(mul);
            string mod = "    " + result.name + " = " + "sub" + " i32 " + left.name + ", " + temp2.name + "\n";
            printllvm(mod);
        } else if (op == "AND") {
            string str = "    " + result.name + " = " + "and" + " i32 " + left.name + ", " + right.name + "\n";
            printllvm(str);
        } else if (op == "OR") {
            string str = "    " + result.name + " = " + "or" + " i32 " + left.name + ", " + right.name + "\n";
            printllvm(str);
        }
    }
}

void printllvmReturnStmt(int retype, const Register& name) {
    string str;
    string type;
    if (retype == 1) {
        type = "i32";
    }

    if (retype == 0) {
        str = "    ret void\n";
    } else {
        str = "    ret " + type + " " + name.to_string() + "\n";
    }

    printllvm(str);
}

void printllvmFuncDef(int retype, const string& name, const vector<Register>& params) {
    string type;
    vector<Register> values;

    if (retype == 0) {
        type = "void";
    } else {
        type = "i32";
    }

    const string& str1 = "define dso_local " + type + " @" + name + "(";
    string str2;
    for (int i = 0; i < params.size(); i++) {
        Register value = getNewRegister(false, 0, false, false,params.at(i).type.id, params.at(i).type.dim);
        value.type.boundary = params.at(i).type.boundary;
        values.push_back(value);
        str2.append(typeToString(value.type) + " " + value.name);
        if (i + 1 < params.size()) {
            str2.append(", ");
        }
    }
    const string& str3 = ") {\n";
    printllvm(str1 + str2 + str3);
    for (int i = params.size() - 1; i >= 0 ; i--) {
        allocRegister(params.at(i));
        storeRegister(params.at(i), values.at(i));
    }
}

void printllvmGlobalConst(const string& name, const Type& type) {
    string str;
    str = "@" + name + " = dso_local constant " + typeToString(type) + " ";
    printllvm(str);
}

void printllvmGlobalAssign(const string& name, const Type& type, const vector<Register>& value) {
    string str;
    int boundary;
    string type_str = typeToString(value.at(0).type);

    if (type.dim == 0) {
        str = to_string(value.at(0).value) + "\n";
    } if (type.dim == 1) {
        boundary = type.boundary.at(0);
        bool flag = true; // 判断这一组是否都为0
        string str0;
        for (int i = 0; i < boundary; i++) {
            str0 += type_str + " " + to_string(value.at(i).value);
            if (value.at(i).value != 0) {
                flag = false;
            }
            if (i + 1 < boundary) {
                str0 += ", ";
            }
        }
        if (flag) {
            str = "zeroinitializer\n";
        } else {
            str = "[" + str0 + "]\n";
        }
    } else if (type.dim == 2) {
        boundary = type.boundary.at(0) * type.boundary.at(1);
        string str1;
        int flag0 = true;
        for (int i = 0; i < boundary; i += type.boundary.at(1)) {
            int flag = true; // 判断这一组是否都为0
            string str0 = "[" + to_string(type.boundary.at(1)) + " x " + type_str + "] ["; // TODO 增加可扩展性？
            for (int j = 0; j < type.boundary.at(1); j++) {
                if (i < value.size()) {
                    str0 += type_str + " " + to_string(value.at(i+j).value);
                    if (value.at(i).value != 0) {
                        flag = false;
                    }
                } else {
                    str0 += type_str + " 0";
                }
                if (j + 1 < type.boundary.at(1)) {
                    str0 += ", ";
                }
            }
            flag0 = flag0 && flag;
            if (flag) {
                str1 += "[" + to_string(type.boundary.at(1)) + " x " + type_str + "] zeroinitializer";
            } else {
                str1 += str0;
            }
            if (i + type.boundary.at(1) < boundary) {
                if (!flag)
                    str1 += "], ";
            } else {
                if (!flag)
                    str1 += "]";
            }
        }
        if (flag0) {
            str = "zeroinitializer\n";
        } else {
            str = "[" + str1 + "]\n";
        }
    }
    printllvm(str);
}

// TODO
void printllvmGlobalVar(const string& name, const Type& type) {
    string str;
    str = "@" + name + " = dso_local global "+ typeToString(type)+" ";
    printllvm(str);
}

void printllvmGetintStmt(const Register& left) {
    Register temp = getNewRegister(false, 0, false, false, 0, 0);
    string str1 = "    " + temp.name + " = call i32 @getint()\n";
    printllvm(str1);
    string str2 = "    store i32 " + temp.name + ", i32* " + left.addr + "\n";
    printllvm(str2);
}

void printllvmPrintfStmt(const string& str, const vector<Register>& params) {
    int j = 0;
    for (int i = 1; i < str.length() - 1; i++) {
        if (str.at(i) != '%') {
            if (str.at(i) != '\\') {
            string str1 = "    call void @putch(i32 " + to_string((int) (str.at(i))) + ")\n";
            printllvm(str1);
            } else {
                i++;
                string str1 = "    call void @putch(i32 10)\n";
                printllvm(str1);
            }
        } else {
            i++;
            string str2 = "    call void @putint(" + typeToString(params.at(j).type) + " " + params.at(j).name + ")\n";
            j++;
            printllvm(str2);
        }
    }
}

void printllvmCallFunc(Register& result, const string& name, const vector<Register>& params) {
    Register func = searchRegister(name);
    string str = "    ";
    if (func.type.id != -1)
        str = "    " + result.name + " = ";
    string str1 = str + "call " + typeToString(func.type) + " "+func.addr+"(";
    string str2;
    for (int i = 0; i < params.size(); i++) {
        str2.append(typeToString(params.at(i).type) + " " + params.at(i).name);
        if (i+1 < params.size())
            str2.append(", ");
    }
    string str3 = ")\n";
    if (func.hasValue) {
        result.hasValue = true;
        result.value = func.value;
    }
    printllvm(str1 + str2 + str3);
}

Register printllvmGetElementPtr(const Register& basePtr) {
    if (basePtr.type.dim == 0) {
        return basePtr;
    } else if (basePtr.type.dim == 1) {
        Register result = getNewRegister(false, 0, false, false, basePtr.type.id, basePtr.type.dim);
        string str = "    "+result.name+" = getelementptr "+ typeToString(basePtr.type)+", "+typeToString(basePtr.type)+"* " + basePtr.addr + ", i32 0, i32 0\n";
        printllvm(str);
        return result;
    } else if (basePtr.type.dim == 2) {
        Register result = getNewRegister(false, 0, false, false, basePtr.type.id, basePtr.type.dim);
        result.type.boundary = basePtr.type.boundary;
        result.type.boundary.pop_back();
        string str = "    "+result.name+" = getelementptr "+ typeToString(basePtr.type)+", "+typeToString(basePtr.type)+"* " + basePtr.addr + ", i32 0, i32 0 \n";
        printllvm(str);
        return result;
    }
    return *new Register(0);
}

Register printllvmGetElementPtr(const Register& basePtr, int i) {
    if(basePtr.type.dim == 1 && basePtr.type.boundary.empty()) {
        Register result = getNewRegister(false, 0, false, false, basePtr.type.id, basePtr.type.dim - 1);
        string str = "    " + result.name + " = getelementptr " + typeToString(result.type) + ", " +
                     typeToString(result.type) + "* " + basePtr.addr + ", i32 " +
                     to_string(i) + "\n";
        printllvm(str);
        return result;
    }  else if (basePtr.type.dim == 1 && basePtr.type.boundary.size() == 1) {
        Register result = getNewRegister(false, 0, false, false, basePtr.type.id, basePtr.type.dim - 1);
        string str = "    " + result.name + " = getelementptr " + typeToString(basePtr.type) + ", " +
                     typeToString(basePtr.type) + "* " + basePtr.addr + ", i32 0, i32 " +
                     to_string(i) + "\n";
        printllvm(str);
        return result;
    } else if (basePtr.type.dim == 2 && basePtr.type.boundary.size() == 1 ) {
        Register result = getNewRegister(false, 0, false, false, basePtr.type.id, basePtr.type.dim - 1);
        result.type.boundary = basePtr.type.boundary;
        result.type.boundary.pop_back();
        string str = "    " + result.name + " = getelementptr " + typeToString(basePtr.type) + ", " +
                     typeToString(basePtr.type) + "* " + basePtr.addr + ", i32 0, i32 " +
                     to_string(i) + "\n";
        printllvm(str);
        return result;
    } else if (basePtr.type.dim == 2 && basePtr.type.boundary.size() == 2) {
        Register result = getNewRegister(false, 0, false, false, basePtr.type.id, basePtr.type.dim - 1);
        result.type.boundary = basePtr.type.boundary;
        result.type.boundary.pop_back();
        string str = "    " + result.name + " = getelementptr " + typeToString(result.type) + ", " +
                typeToString(result.type) + "* " + basePtr.addr + ", i32 0, i32 " + to_string(i*basePtr.type.boundary.at(1)) + "\n";
        printllvm(str);
        result.type.boundary.pop_back();
        return result;
    }
    return *new Register(0);
}

Register printllvmGetElementPtr(const Register& basePtr, int i, int j) {
    if (basePtr.type.dim == 2 && basePtr.type.boundary.size() == 1) {
        Register result = getNewRegister(false, 0, false, false, basePtr.type.id, basePtr.type.dim - 1);
        result.type.boundary = basePtr.type.boundary;
        string str = "    " + result.name + " = getelementptr " + typeToString(result.type) + ", " +
                     typeToString(result.type) + "* " + basePtr.addr + ", i32 " +
                     to_string(i) + "\n";
        printllvm(str);
        Register result2 = getNewRegister(false, 0, false, false, result.type.id, result.type.dim - 1);
        string str2 = "    " + result2.name + " = getelementptr " + typeToString(result.type) + ", " +
                      typeToString(result.type) + "* " + result.name + ", i32 0, i32 " + to_string(j) + "\n";
        printllvm(str2);
        return result2;
    } else if (basePtr.type.dim == 2 && basePtr.type.boundary.size() == 2) {
        Register result = getNewRegister(false, 0, false, false, basePtr.type.id, basePtr.type.dim - 2);
        result.type.boundary = basePtr.type.boundary;
        result.type.boundary.pop_back();
        result.type.boundary.pop_back();
        string str = "    "+result.name+" = getelementptr " + typeToString(basePtr.type) + ", "+typeToString(basePtr.type)+"*" + basePtr.addr +", i32 0, i32 " +
                                                                                                                                               to_string(i) + ", i32 "+to_string(j)+"\n";
        printllvm(str);
        return result;
    }
    return *new Register(0);
}

void printllvmLabel(const Register& label) {
    string str = label.name.substr(1) + ":\n";
    printllvm(str);
}

void printllvmBranch(const Register& cond, const Register& stmt1, const Register& stmt2) {
    string str = "    br i1 " + cond.name + ", label " + stmt1.name + ", label " + stmt2.name + "\n\n";
    printllvm(str);
}

void printllvmBranch(const string& name) {
    string str = "    br label " + name + "\n\n";
    printllvm(str);
}

void printllvmBranch(const Register& label) {
    string str = "    br label " + label.name + "\n\n";
    printllvm(str);
}

Register printllvmTruncCond(const Register& cond) {
    Type type = Type (-2, 0);
    Register result = getNewRegister(false, 0, false, false, type.id, type.dim);
    string str = "    " + result.name + " = trunc " + typeToString(cond.type) + " " + cond.name + " to " + typeToString(type) + "\n";
    printllvm(str);
    return result;
}

Register printllvmZext(const Register& right) {
    Type type = Type(0,0);
    Register result = getNewRegister(false, 0, false, false, type.id, type.dim);
    string str = "    " + result.name + " = zext " + typeToString(right.type) + " " + right.name + " to " + typeToString(type) + "\n";
    printllvm(str);
    return result;
}