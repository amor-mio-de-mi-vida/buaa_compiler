//
// Created by 杨贺森 on 2023/11/21.
//

#ifndef BUAA_COMPILER_IR_H
#define BUAA_COMPILER_IR_H


class Value {

};

class Use {

};


class User : Value {

};


class BasicBlock : Value {

};

class Constant : User {
    
};

class Instruction : User {

};


#endif //BUAA_COMPILER_IR_H
