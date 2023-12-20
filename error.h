//
// Created by 杨贺森 on 2023/10/1.
//

#ifndef HW2_ERROR_H
#define HW2_ERROR_H
extern int rowNo;
extern int colNo;

void getErrorMessage(const std::string& str) {
    std::cout << str << " at row:" << rowNo << " column: "<< colNo <<std::endl;
}

#endif //HW2_ERROR_H
