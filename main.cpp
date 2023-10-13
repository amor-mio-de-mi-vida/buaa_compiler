#include <iostream>
#include <fstream>
#include <string>
#include "token.h"
using namespace std;

int rowNo = 1;  // 当前解析的行号
int colNo = 0;  // 当前解析的列号
bool comment = false; // 处理可换行的注释

int main() {
    const char* filePath = "../testfile.txt";
    ifstream file;
    file.open(filePath, ios::in);
    if (!file.is_open())
        printf("can't open the file!\n");

    std::string strLine;
    while(getline(file, strLine)) {
        // printf("get new line! rowNo is %d and colNo is %d\n", rowNo, colNo);
        if (strLine.empty()) {
            colNo = 0;
            rowNo++;
            continue;
        }

        if (!comment) {
            getTokens(strLine, strLine.length());
        } else {
            size_t r = strLine.find("*/");
            if (r != string::npos) {
                comment = false;
                colNo = r + 2; // 小心溢出
                getTokens(strLine.substr(colNo,strLine.length() - colNo), strLine.length());
            }
        }
        // cout << strLine << endl;
        colNo = 0;
        rowNo++;
    }

    printTokens();

    return 0;
}
